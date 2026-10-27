# 单写多读内存表（RCU 形态）设计与原型验证

> 记录 B 方案（无锁读 + 原子换指针 + 代数回收）与 B2 变体（稳定行地址 + 行内版本号）的设计、讨论结论与原型验证。整理日期 2026-10-22。
> 原型代码：[DemoRcu](../test/DemoRcu/DemoRcu.cpp)；上层设计背景见 `D:\Gitlab\Trading\docs\C++-套利策略资管系统设计.md` §1。

---

## 目录

- [单写多读内存表（RCU 形态）设计与原型验证](#单写多读内存表rcu-形态设计与原型验证)
  - [1. 背景与结论](#1-背景与结论)
  - [2. 方案](#2-方案)
    - [2.1 三条规则](#21-三条规则)
    - [2.2 读路径](#22-读路径)
    - [2.3 写路径](#23-写路径)
    - [2.4 索引结构](#24-索引结构)
    - [2.5 扩容](#25-扩容)
    - [2.6 范围读](#26-范围读)
  - [3. 讨论纪要](#3-讨论纪要)
  - [4. 单写者约束](#4-单写者约束)
  - [5. 原型验证](#5-原型验证)
  - [6. 风险与未覆盖](#6-风险与未覆盖)
  - [7. 结论](#7-结论)
  - [8. B2 变体](#8-b2-变体)
  - [参考](#参考)

---

## 1. 背景与结论

现状（A 方案）每张表一把 `std::shared_mutex`：`Select` / `SelectAll` / `ForEach` 取共享锁，`Insert` / `Update` / `Erase` 取独占锁。
它没有撕裂读问题——`Update` 的整行 `memcpy` 在独占锁内完成，读者无法观察到中间态。
B 方案要解决的也不是撕裂，而是 §1.1 列出的三项读侧代价：共享锁的原子争用、读者优先导致写者饥饿、以及整表粒度的锁。

原型验证的结论：

- **B 在单写者前提下成立**，读者完全无锁，写者不被读者阻塞。
- **`Update` 不需要「先插入再删除」**，它只做同槽位的原子指针替换，不会出现同主键两条记录。
- **延迟回收不是引用计数**，而是按读者登记代数整批回收，保守但读侧没有逐记录的原子操作。
- **B 不支持多写者**。并发写会丢更新、二次退休、破坏退休队列；需要写者互斥或完整无锁改写。
- **A 仍然安全**。是否切换只看 §1.6 的压测判据，原型本身不构成切换理由。

---

## 2. 方案

### 2.1 三条规则

| 规则 | 内容 |
| :--- | :--- |
| 行不可变 | 表内记录一经发布不再原地修改；`Update` 构造新行，换指针 |
| 索引可原子发布 | 每个槽位是一个 `std::atomic<Record*>`；整张槽数组由根指针原子发布 |
| 延迟回收 | 旧行与旧根进入退休队列，等所有读者离开旧代数后归还 |

读者拿不到表内指针的长期所有权：`Select` 在登记代数内拷贝整行，返回即脱钩，与 Mdb 现有的值语义读一致。

### 2.2 读路径

```cpp
std::optional<TradingDayRow> AtomicTradingDayTable::Select(std::int32_t pk)
{
    ReaderEpoch::Guard guard(epoch_, CurrentReaderIndex());
    const TradingDayRow* liveRecord = Find(pk);
    return liveRecord == nullptr ? std::nullopt : std::optional<TradingDayRow>(*liveRecord);
}
```

`ReaderEpoch::Guard` 的登记采用 seq_cst 双检：

```cpp
const std::uint64_t epoch = owner.currentEpoch_.load(std::memory_order_seq_cst);
slot_->store(epoch, std::memory_order_seq_cst);
if (owner.currentEpoch_.load(std::memory_order_seq_cst) == epoch)
{
    return;
}
```

双检用于堵住「读者读到旧代数、写者扫描槽位时还没看到它」的窗口。这是整个回收协议最容易写错的一步。

### 2.3 写路径

`Update` 的骨架：

```cpp
TradingDayRow* replacement = TradingDayRow::Allocate();
*replacement = newRecord;
// 探测到旧行所在槽位
root->slots[position].record.store(replacement, std::memory_order_release);
epoch_.Retire(current, ReclaimRow);
epoch_.AdvanceAndReclaim();
```

回收判据只有一条：

```text
退休记录的退休代数 < 所有活跃读者中最小的登记代数  →  可以回收
```

`AdvanceAndReclaim` 推进代数后扫描退休队列，满足条件的整批回收。`Erase` 则是把槽位写成墓碑指针，再把旧行退休；`Insert` 的返回值与主键唯一性仍由写者保证。

### 2.4 索引结构

原型用**开放寻址**（线性探测）：

- 表就是一个 `Slot` 数组，每个 `Slot` 含一个 `std::atomic<TradingDayRow*>`；
- `Hash(pk) % capacity` 只是 home 槽位，冲突时顺序探测下一个槽位；
- 查找遇到空槽即不存在；遇到墓碑继续探测；
- 删除写墓碑，不置空，否则会截断后续键的探测链。

选择开放寻址而不是链地址法的原因：

| 原因 | 说明 |
| :--- | :--- |
| 读侧不跟随容器内部指针 | 读者只做「读根 → 读槽 → 比较」，不碰 `next` 或迭代器 |
| 写操作是一次原子 store | 换指针、置墓碑都是单个槽位上的原子写 |
| 扩容可整体发布 | 新建槽数组、重新散列、原子换根，旧根整体退休 |

### 2.5 扩容

开放寻址**没有省掉 rehash**，反而更依赖重建：墓碑会累积，必须定期清理。

```cpp
void RebuildForInsertIfNeeded()
{
    Root* root = root_.load(std::memory_order_relaxed);
    if ((usedSlots_ + 1) * 4 <= root->capacity * 3)
    {
        return;
    }
    Root* newRoot = CreateRoot(root->capacity * 2);
    // 把所有活记录重新散列进新数组
    root_.store(newRoot, std::memory_order_release);
    epoch_.Retire(root, ReclaimRoot);
    usedSlots_ = rowCount_;
}
```

与链地址法的差别是「搬什么」：原型槽位里存的是行指针，rehash 只搬指针并重新探测落位，**行本身不移动**。因此读者即使在旧根上遍历，旧根中的指针仍指向有效行，旧根只需按代数延迟释放。若记录内联在槽数组中，扩容会搬动行本身，读者就会读到已搬走的记录，B 的意义随之消失。

### 2.6 范围读

`ForEach` 在登记代数内取根快照后遍历。它的语义是**弱一致**：

- 每条记录的字段是完整的（行不可变）；
- 遍历期间写者照常工作，集合成员可能变化；
- 不再像 A 的 `TableRangeLease` 那样阻塞写者。

若业务需要冻结的集合视图，应使用周期快照（C 方案），而不是 B。

---

## 3. 讨论纪要

### 3.1 现状 `Update` 是独占锁，不是共享锁

`Update` 取的是 `std::lock_guard guard(SharedMutex)`，对 `std::shared_mutex` 即独占锁（`src/Mdb/MdbTables.cpp:117` 等，模板 `MdbTables.cpp.tpl:202` 同）。
`Select` / `SelectAll` / `ForEach` / `Dump` 才是共享锁。独占与共享互斥，`memcpy` 期间不存在持锁读者，所以现状没有撕裂读。

上层设计文档 §1.2 那句「`Update` 整行 `memcpy`，并发读者会读到撕裂值」是**形态 B 的前置条件说明**：一旦读者不拿锁，行就不能原地改，必须走「新行 + 换指针 + 延迟回收」。

### 3.2 延迟回收不是引用计数

| | 引用计数 | 代数回收 |
| :--- | :--- | :--- |
| 计数对象 | 每条记录一个原子计数 | 每个读者线程一个槽位 |
| 每次访问成本 | 拷贝/析构都要原子加减 | 临界区进入时写一次自己的槽位 |
| 回收精度 | 精确，最后一个引用释放即回收 | 保守，等旧代数读者全部离开后整批回收 |
| 跨核争用 | 同一条记录被多线程读时计数行争用 | 槽位按线程分开，无争用 |

它是 RCU 风格的宽限期回收，不是追踪式 GC，也不是 `shared_ptr`。代价是保守：一个长范围扫描会延迟它之后的整批退休记录，哪怕它没碰过那些记录。

### 3.3 `Update` 不会产生同主键两条记录

写者在旧行所在槽位上做一次原子 store：

```text
换之前：  slots[7] ──► R_old（主键 100）
换之后：  slots[7] ──► R_new（主键 100）
          R_old 只挂在退休队列里，索引中已无它
```

索引可见性是即时切换的，延迟的只是内存归池。单写者下「探测到旧行」与「换指针」之间没有其他写者，因此不需要 CAS，也不会插入第二条同主键记录。

### 3.4 开放寻址与 rehash

「碰撞后占其他位置」正是开放寻址的定义：`Hash(pk)` 给出的是探测起点，冲突时顺延到下一个空槽。删除必须写墓碑，否则会截断探测链。负载因子（原型取 75%）和墓碑数量共同决定何时扩容；扩容时把活记录重新散列到新数组，墓碑随之清零。开放寻址省掉的是每元素一次节点分配和链指针跳转，不是扩容。

---

## 4. 单写者约束

原型的所有写路径都假设同时只有一个写者。`RebuildForInsertIfNeeded` 最明显：它读旧根、基于旧根建新根、再直接 `store` 覆盖，没有 CAS。多写者并发时：

| 并发场景 | 后果 |
| :--- | :--- |
| 两个 `Insert` 探到同一空槽 | 后写覆盖先写，丢一行，`rowCount_` 多算 |
| 两个 `Update` 改同一主键 | 都读到旧行、都 `Retire` 旧行 → 二次释放；一个替换行泄漏 |
| 两个 `Erase` 同一主键 | 二次退休 |
| 两个写者同时触发扩容 | 后发布的新根覆盖先发布的，先者中的插入丢失，旧根可能被退休两次 |
| 并发 `Retire` | 同时 `push_back` 同一个 `std::vector`，未定义行为 |
| 并发写 `rowCount_` / `usedSlots_` | 普通 `size_t`，数据竞争 |

三条可选路径：

| 方案 | 改动 | 代价 |
| :--- | :--- | :--- |
| 写者互斥锁 | 写路径与 `CollectRetired` 外加一把 `std::mutex` | 写者之间串行，读者完全不受影响；实现最简单 |
| 完全无锁多写 | 槽位 CAS、扩容协调、每写者退休队列 | 等同重写工业级无锁哈希表 |
| 维持单写 + 断言 | 检测并发写并立即失败 | 只暴露误用，不提供能力 |

上层设计文档 §1.2 的前提是「每张表只有一个写者线程，由组件拆分保证，不靠锁」。若该前提成立，原型当前形态即可；若不成立，优先加写者互斥锁，而不是直接上无锁多写。

---

## 5. 原型验证

原型目标 `DemoRcu` 已接入根 `CMakeLists.txt`，与 `TestMdb` 共用 `add_application`：

```text
cmake --build out/build/x64-Debug --target DemoRcu
out/build/x64-Debug/bin/Debug/DemoRcu.exe
```

覆盖用例：插入与查询、副本脱钩、重复主键拒绝、`Update` 后行地址不变、索引无需重指即可读到新值、索引键变化后重排、删除与索引同步、`ForEach`、扩容重建、4 主键读者 + 2 索引读者 + 1 写者并发。

2026-10-22 在本机 Debug 构建的实测输出（数字随机器与调度变化，不作为性能基准）：

```text
concurrentNoTornReads=1
concurrentPrimaryReadersMadeProgress=1
concurrentIndexReadersMadeProgress=1
concurrentSingleLiveRow=1
concurrentUpdatesRetiredNothing=1
primaryReads=6173183 indexReads=152233 torn=0 updates=20000 elapsedMs=337
failures=0
```

撕裂检测方式：写者每轮把 `CurrTradingDay` 与 `PreTradingDay` 写成同一个值并切换 `ExchangeId`，读者拷完整行后比较；索引读者另校验返回行的 `ExchangeId` 与查询键一致。

---

## 6. 风险与未覆盖

| 项 | 说明 |
| :--- | :--- |
| 读者槽位上限 | `MaxReaders = 64`；槽位按线程分配，无回收 |
| 嵌套登记 | 原型未处理同线程嵌套 `Guard`，真实实现需嵌套计数或按临界区分槽 |
| 只有主键索引 | 唯一键与二级索引未接入；跨索引只能做到逐行原子、跨索引弱一致 |
| 分配压力 | 每次 `Update` 新分配一行，对象池吞吐与内存峰值会成为新热点 |
| 回收延迟 | 长范围扫描会推迟整批退休记录的回收 |
| 生产化 | 原型未接入 `Mdb::TradingDay`、事件订阅、对象池与 `DbAdapters` 写库路径 |
| 多写者 | 见 §4，当前不支持 |

---

## 7. 结论

- A 方案当前是安全的，没有撕裂读；切换 B 的唯一依据应是 §1.6 的压测数据显示写 p99 被读压拖坏。
- 若决定切换，B 的业务接口可以保持不变（`Select` 返回值副本、`Update` / `Erase` 按主键自定位），改动集中在模板、索引容器与回收协议。
- 切换前必须先确认「每张表只有一个写者线程」这条前提；否则要么加写者互斥锁，要么重新评估 B 的收益。
- 范围查询的一致性会从「冻结快照」降为「弱一致遍历」，对账类场景需要 C 方案补位。

---

## 8. B2 变体

`DemoRcu` 当前实现的是 B2，而不是 §2 的换指针 B。§2 的原形态保留在 `test/DemoRcuB`（只有主键、没有二级索引），不需要索引的场景可直接照它实现：`cmake --build <构建树> --target DemoRcuB`。两者都让主键查询无锁，区别在行的地址：

| | B（§2） | B2（DemoRcu 现形态） |
| :--- | :--- | :--- |
| 行地址 | 每次 `Update` 换新行 | 始终不变 |
| 行内容 | 不可变 | seqlock：写者推进版本号，读者拷贝后校验 |
| 主键索引 | 原子槽位 | 原子槽位 |
| 唯一键 / 二级索引 | 每次 `Update` 都要重指或重排 | 指针不变；仅索引键变化时重排节点 |
| `Update` 分配 | 每轮新分配一行 | 不分配 |
| `Update` 退休 | 旧行进退休队列 | 不退休 |
| 索引读者 | 要无锁就得重写每个索引 | 仍取共享锁；长读仍会阻塞写者 |

B2 的代价是 seqlock 的读侧重试：写者正在写同一行时，读者拷到一半会重来。单行写入很短，实际重试次数很低。
另一个代价是无锁读的字段访问在 C++ 标准层面属于数据竞争（Linux 内核式的 seqlock 惯例），生产实现若要求严格可移植，应改为逐字段 `atomic_ref` 或平台屏障。

二级索引仍是一把读写锁：Linux 默认读者优先，连续的长索引读仍可能饿死写者。B2 只把主键查询这条最热的路径从锁里拿了出来，不改变索引路径的这一问题。

唯一键没有单独加入 demo：它与主键同为不可变键，更新逻辑就是一个额外的哈希索引，不引入新的并发路径。二级索引才是 B2 里需要额外处理的唯一结构。

---

## 参考

- 原型源码：[DemoRcu](../test/DemoRcu/DemoRcu.cpp)
- 上层设计：`D:\Gitlab\Trading\docs\C++-套利策略资管系统设计.md` §1（内存数据库：从读写锁到一写多读）
- 现有读接口与租约语义：[README](../README.md)
- 环境准备：[环境准备指南](environment-setup.md)
