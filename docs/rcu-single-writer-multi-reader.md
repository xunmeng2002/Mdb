# 单写多读内存表（RCU 形态）设计与原型验证

> 记录 B 方案（无锁读 + 原子换指针 + 代数回收）与 B2 变体（稳定行地址 + 行内版本号）的设计、讨论结论与原型验证。整理日期 2026-10-22，2026-10-27 补第 9 节（接入 Mdb 表接口）与第 10 节（三种实现的并发对照，操作间隔 0–1 ms 梯度）。
> 实现代码：[StableRowTables](../src/Mdb/Rcu/StableRow/StableRowTables.h)、[PointerSwapTables](../src/Mdb/Rcu/PointerSwap/PointerSwapTables.h)（原型 demo `DemoRcu` / `DemoRcuB` 已于 2026-10-29 移除，用例并入 [test/TestTables](../test/TestTables)）；上层设计背景见 `D:\Gitlab\Trading\docs\C++-套利策略资管系统设计.md` §1。

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
  - [9. 接入 Mdb 表接口](#9-接入-mdb-表接口)
  - [10. 三种实现的并发对照](#10-三种实现的并发对照)
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

原型 demo（`test/DemoRcu`、`test/DemoRcuB`）已于 2026-10-29 移除：它们本就只依赖标准库、用来演示两种形态，而同样的用例已由 `TestTables` 覆盖到三个实现上。

```text
cmake --build out/build/x64-Debug --target TestTables
out/build/x64-Debug/bin/Debug/TestTables.exe
```

覆盖用例：插入与查询、副本脱钩、重复主键拒绝、`Update` 后行地址不变、索引无需重指即可读到新值、索引键变化后重排、删除与索引同步、`ForEach`、扩容重建、4 主键读者 + 2 索引读者 + 1 写者并发。

2026-10-22 原型 demo 在本机 Debug 构建的实测输出（记录保留；数字随机器与调度变化，不作为性能基准）：

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
| 索引不全 | B2 有二级索引与唯一键（`Order.ClientOrderId`），仍由 `SharedMutex` 保护；B 两样都没有，`Order` 在 B 下会静默接受重复的唯一键 |
| 分配压力 | 每次 `Update` 新分配一行，对象池吞吐与内存峰值会成为新热点 |
| 回收延迟 | 长范围扫描会推迟整批退休记录的回收 |
| 生产化 | 表接口、事件订阅与 `Mdb` 聚合已接入（§9）；写库路径（`AsyncDbWriter`）与耗时/内存基线的对拍尚未做 |
| 多写者 | 见 §4，当前不支持 |

---

## 7. 结论

- A 方案当前是安全的，没有撕裂读；切换 B 的唯一依据应是 §1.6 的压测数据显示写 p99 被读压拖坏。
- 若决定切换，B 的业务接口可以保持不变（`Select` 返回值副本、`Update` / `Erase` 按主键自定位），改动集中在模板、索引容器与回收协议。
- 切换前必须先确认「每张表只有一个写者线程」这条前提；否则要么加写者互斥锁，要么重新评估 B 的收益。
- 范围查询的一致性会从「冻结快照」降为「弱一致遍历」，对账类场景需要 C 方案补位。

---

## 8. B2 变体

Mdb 里两种形态各有实现：`Mdb::Rcu::StableRow` 是 B2（`src/Mdb/Rcu/StableRow`），`Mdb::Rcu::PointerSwap` 是 §2 的换指针 B（`src/Mdb/Rcu/PointerSwap`，只有主键、没有二级索引）。两者都让主键查询无锁，区别在行的地址：

| | B（§2，`Mdb::Rcu::PointerSwap`） | B2（`Mdb::Rcu::StableRow`） |
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

唯一键没有单独加入 demo，但 B2 的表类里已按 Mdb 的形式生成（见 §9）：它与主键同为不可变键，落地就是一个额外的 `unordered_set`，`Select` 取 `SharedMutex` 共享锁，`Update` 遇键变化即拒绝，
不引入新的并发路径。B 形态按设计不维护任何辅助结构，唯一键与二级索引一样不生成。

---

## 9. 接入 Mdb 表接口

两个 RCU 形态已按 Mdb 生成表的方式重做了一遍，与读写锁版一起按「一个库 = 一个文件夹」重排；全部由模板生成，勿手改。

| 目录 | 命名空间 | 内容 |
| --- | --- | --- |
| `Common/` | `Mdb` | 数据契约：记录结构、`MdbTableBase`、`TableRangeLease`、哈希/比较器、`TableList` |
| `LockBased/` | `Mdb` | 读写锁实现：`MdbTables` / `MdbPrimaryKeys` / `MdbIndexes` + `Mdb` 聚合、`MdbTableRegistry`、`InitMdbFrom*` |
| `Rcu/` | `Mdb::Rcu` | 读者登记 + 代数回收，两形态共用 |
| `Rcu/PointerSwap/` | `Mdb::Rcu::PointerSwap` | B 形态：`PointerSwapTables` + `PointerSwapPrimaryKeys` + 同名 `Mdb` 聚合与 `InitMdbFrom*`（换指针 + 退休旧行） |
| `Rcu/StableRow/` | `Mdb::Rcu::StableRow` | B2 形态：`StableRowTables` + `StableRowPrimaryKeys` + `StableRowIndexes` + 同名 `Mdb` 聚合与 `InitMdbFrom*`（行地址稳定 + seqlock） |

`Common/` 与 `LockBased/` 沿用 `Mdb` 根命名空间（公共类型名不冲突）；两个 RCU 目录才有子命名空间，因为它们的表类与读写锁版同名。
索引序号挂在记录结构上（`PrimaryAccount::IndexOfferIdId`，与 `TableId` 并列），schema 与索引类都引用它，跨目录依赖只有 LockBased / 两个 RCU 库 → Common 一个方向。

索引与唯一键按形态分工（2026-10-27 裁定）：**B 一律忽略 `indexes` 与 `uniquekeys` 节点**，不生成索引类、唯一键类与 `EraseBy<键>Index`，
使用方只能按键访问；缺的是编译期可见的接口，不会退化成运行期慢路径。
`StableRow` 与读写锁版按 Mdb 的形式生成 `<表>TableIndex<键>* <键>Index`、`<表>TableIndex<键>::Range` / `EqualRange` 与 `EraseBy<键>Index`
（如 `PrimaryAccount` 的 `OfferIdIndex`），以及 `<表>TableUniqueKey<键>* <键>UniqueKey`（当前只有 `Order` 用到，`Select` 六个键字段全参）；
两者的更新都走 `SharedMutex`，与 §8 的结论一致。
两个 RCU 变体都生成 `Tables.xml` 里的全部 11 张表（2026-10-27 去掉 `supportedTables` 白名单），B2 为每张表按其 `indexes` 与 `uniquekeys` 节点生成对应类。
schema 的 `IndexDefinition` 始终完整：它描述物理库该建哪些索引，与哪个内存实现承载读路径无关。

目标是三个可选的库（读写锁 / B / B2），**允许同进程共存**，以便性能对拍与灰度替换。`CMakeLists.txt` 已按此拆开：

| 目标 | 源目录 | 内容 |
| --- | --- | --- |
| `MdbCommonStatic` | `src/Mdb/Common` | 数据契约 |
| `MdbLockBasedStatic` | `src/Mdb/LockBased` | 读写锁实现 |
| `MdbRcuCoreStatic` | `src/Mdb/Rcu` | `RcuEpoch`，两个 RCU 库共用一份，避免同进程链接时符号重复 |
| `MdbRcuPointerSwapStatic` | `src/Mdb/Rcu/PointerSwap` | B 形态表、`Mdb` 聚合、`InitMdbFrom*`（链接 `Spark::Serialization`） |
| `MdbRcuStableRowStatic` | `src/Mdb/Rcu/StableRow` | B2 形态表、`Mdb` 聚合、`InitMdbFrom*`（链接 `Spark::Serialization`） |
| `MdbStatic` | — | INTERFACE 聚合（`MdbCommonStatic` + `MdbLockBasedStatic`），既有消费方不改 |

每个目标用 `file(GLOB CONFIGURE_DEPENDS "<目录>/*.cpp")` 非递归取源文件，往目录里加 `.cpp` 不用改 CMake。
文件切分与 `LockBased/` 一致：表类与主键各一个 TU，`<表>Slot` / `SlotArray` 归主键头，`Tables.h` 单向包含 `PrimaryKeys.h`；
B 没有索引类；B2 的索引类另立 `StableRowIndexes`，与 `StableRowTables` 同目录、同命名空间。
表类与索引类（含订阅/发布层）各库一份，不抽公共基类——两个 RCU 变体的表级代码各 268 行、仅差 2 行
（`Replace` 与 `Overwrite`），复制的成本低于耦合。
共存要求命名空间独立：读写锁版仍占 `Mdb::`，若要与另两库对称改名为 `Mdb::LockBased`，需另批处理（会动本仓调用点与 QuantTrading）。

模板与目录同构：`../Templates/Cpp/Mdb/{Model,LockBased,Rcu}/`，`pumplist.xml` 逐文件登记；两个 RCU 目录各有 `Mdb` / `InitMdbFromCsv` / `InitMdbFromDb`，
与 `LockBased/` 的同名模板只差命名空间和 `Tables.h` 的包含路径，与读写锁版形成三选一的等价实现。

表接口与 `Mdb::TradingDayTable` 一致：构造/析构、`GetTableId`、`Subscribe`/`SubscribeReplica`/`UnSubscribe`、`InitDb`、`ForEach`、
`Insert`/`BatchInsert`/`Erase`/`Update`/`TruncateTable(s)`/`Dump`，公开 `SharedMutex` 与 `PrimaryKey`，`Select`/`SelectAll` 仍挂在主键上。
写路径同样带每表 `publishMutex_` 发布屏障，锁序 `publishMutex_` → `SharedMutex`，事件在锁外分发。

三处按 Mdb 语义做的取舍：

- **范围读仍走锁（方案 2）**：只有单键 `Select` 无锁，`SelectAll` 取 `SharedMutex` 共享锁并随租约走，读者遍历期间写者被挡。`ForEach` / `Dump` / `InitDb` 同理，都靠共享锁挡住写者与回收。
- **`Erase` 的事件是副本**：B 形态下离表的活记录要等登记读者走完才回收，直接把指针交给订阅者会悬空，故先 `memcpy` 一份再建事件。B2 的活记录虽然地址稳定，也照此办理以保持两形态行为一致。
- **回收时机**：每次写操作结束调用一次 `AdvanceAndReclaim`，退休队列在「无读者持有更早代数」时立刻回收；析构时走 `ReclaimAllWithoutReaders`，不再等读者。

代价与边界：单写者前提不变（多写者会打乱 `rowCount_`/`usedSlotCount_` 与槽位表重排）；`ReaderEpoch::MaxReaders = 64`，
每线程一个 `thread_local` 槽位且线程退出不回收，超限直接 `abort`；B 与 B2 的订阅/发布管道与 `MdbTables` 生成码重复，
后续若要抽公共基类，属跨形态重构，需单独评估。

验证由 `test/TestTables`（doctest 2.5.3，与兄弟仓共用 `../Libs/doctest`）承担：通用场景用 `TEST_CASE_TEMPLATE` 以模板参数分别跑
`Mdb::Rcu::PointerSwap::TradingDayTable`、`Mdb::Rcu::StableRow::TradingDayTable` 与 `Mdb::TradingDayTable`，覆盖单行增删改查、
2000 行批量插入与扩容、墓碑后可达性、`SelectAll` 阻塞写者、订阅事件（含 `Erase` 副本）、`BatchInsert` 的所有权移交与单次批量事件、
副本订阅的快照与增量顺序、3 读者 × 20000 次 `Update` 的撕裂读检测、`TruncateTable`。
另有一组只跑 B2 的二级索引场景（`PrimaryAccount`）：重复索引键计数、`Range` 双边界、`Update` 改索引键后的重排、
改非索引字段不重排、`EraseByOfferIdIndex` 的返回数与 `EraseByIndex` 事件（含 `IndexId`）、`TruncateTable` 清空索引。
唯一键另有一组只跑 B2 的场景（`Order.ClientOrderId`）：插入后按键查得到、主键不同而唯一键相同被拒、`Update` 改唯一键被拒且原登记保留、
改非键字段后仍能按键查到新值、`Erase` 后键失效、同一唯一键可重新插入。
`Mdb` 聚合冒烟同样以模板参数跑两个 RCU 形态：只申请 `TradingDay` 的 `TableList` 建出的聚合，该表指针非空、未申请的表保持空指针。
同一可执行文件另跑读写锁版的表级直连场景：`Product` 复合字符串主键、`Position` 五列混合主键配合
`AccountIndex` / `TradingDayIndex` 与 `EraseByAccountIndex`、`PrimaryAccount` 索引列改值后的重排、`TruncateTable` 清三份登记、
`OrderTable::Dump` 的表头与行数，以及两种实现的 `BatchInsert` 索引 / 唯一键登记。
实测 34 个用例、256 条断言全过（`--test-case=` 可单跑，失败输出带文件与行号）。

---

## 10. 三种实现的并发对照

`test/TestConcurrency` 把三个库链进同一个进程（`MdbLockBasedStatic` + `MdbRcuPointerSwapStatic` + `MdbRcuStableRowStatic`），
不接物理库、不建 `AsyncDbWriter`，用同一套负载跑 3 种实现 × 读线程 2 / 4 / 8，写线程恒为一个。
读写两端每次操作后可忙等一个命令行给定的间隔（微秒），用来对照「满载自旋」与「真实业务节奏」两种情形。

```text
cmake --build out/build/x64-Debug --target TestConcurrency
out/build/x64-Debug/bin/Debug/TestConcurrency.exe [操作间隔微秒数] [doctest 选项]
```

第一个裸数字参数被当作操作间隔，以 `-` 开头的参数原样交给 doctest；两者可同时给。例如
`TestConcurrency.exe 0 --test-case="*2 读者*"` 只跑读者数为 2 的三种实现且不设间隔。
每配置仍先打印一行读数（实现名、读者数、读写次数、撕裂读与缺失行计数、耗时），随后断言。

设计要点：

| 项 | 做法 |
| :--- | :--- |
| 建表 | 表对象直接构造，先插 10 万行 `TradingDay`（约 3.6 MB 净数据，读侧开始出现缓存未命中）；全程不删行，读不到即为缺陷 |
| 读负载 | 每个读者按自己的 LCG 序列挑主键，调真实读接口 `PrimaryKey->Select`，返回副本后校验 `CurrTradingDay == PreTradingDay` |
| 写负载 | 写者在主线程用另一条 LCG 挑行调真实写接口 `Update`（顺序循环只会碰前几千行，与十万行扫读几乎不重叠），把两个日期字段写成同一个新值 |
| 操作间隔 | 两端每完成一次操作后忙等该间隔，模拟两次操作之间的计算与 IO。命令行第一个参数给微秒数，缺省 10；给 0 即无间隔的饱和最坏情况 |
| 时长 | 读写两端各跑固定 1 s 窗口；窗口内完成多少条 `Update` 只作观测量，不作断言 |
| 收尾 | 读者除 `stopSignal` 外另有 300 ms 自超时，兜住读写锁版写者被读者挡住的情形，使总时长有界 |
| 断言 | 每配置 7 条：建行成功、无撕裂读、无缺失行、无失败 `Update`、写者有进展、每个读者有进展、终态与写者最后写入一致 |

间隔用忙等而不是 `sleep_for`：Windows 的睡眠精度是毫秒级且偏往上，1 ms 的间隔会被撑成数毫秒，端到端次数就不成比例了。
忙等线程仍是真实接口的紧循环，`yield` 只出现在「等发令」和 `StableRow` 的 seqlock 重试里，两处都不持表锁。

读者在间隔为 0 时是**零思考时间**的：两次读之间只有每 1024 次一次的时钟检查，共享锁几乎 100% 时间被占用，写者能进独占锁的窗口只有
「某个读者释放到下一个读者加锁」之间那几十到几百纳秒。实测读写锁版的写者每秒只进几十到几百次（8 读者时甚至只有个位数），量级上等于
「操作系统抢占某个自旋读者」的频率，这也是同一配置多次运行能差 16 倍的原因。
真实业务读者两次读之间有计算与 IO、锁大部分时间是空的，写者饥饿不会出现；因此 §10.1 是**饱和最坏情况**的度量，
用于在最大并发下暴露正确性缺陷与公平性性质，不用于预测生产吞吐。按真实节奏加间隔后的结果见 §10.2。

撕裂判据用「成对日期」而不是逐字段比对：写者把 `CurrTradingDay` 与 `PreTradingDay` 同时改成同一个值，
读者只要读到两者不同，就说明 `Update` 的写入被读侧观察到了一半。这在读写锁版不可能发生（`Update` 持独占锁），
在 B 形态不可能发生（行不可变，换指针），在 B2 上只有 seqlock 校验漏掉才会出现，所以三种实现共用同一判据。

### 10.1 无间隔（`interval=0`，饱和最坏情况）

2026-10-27 本机 Debug 实测（单次运行，数字随机器与调度变化，不作性能基准）：

| 实现 | 读线程 | 读次数 | 完成 `Update` | 耗时 ms |
| :--- | ---: | ---: | ---: | ---: |
| 读写锁版 | 2 | 1489920 | 503 | 1025 |
| 读写锁版 | 4 | 799744 | 43 | 1303 |
| 读写锁版 | 8 | 531456 | 158 | 1308 |
| `PointerSwap` | 2 | 8930304 | 353362 | 1000 |
| `PointerSwap` | 4 | 17260544 | 297877 | 1000 |
| `PointerSwap` | 8 | 18905088 | 214076 | 1000 |
| `StableRow` | 2 | 9382912 | 1332471 | 1000 |
| `StableRow` | 4 | 13216768 | 1188990 | 1000 |
| `StableRow` | 8 | 20473856 | 1010122 | 1000 |

63 条断言全为 1、`failures=0`，总时长约 13 s。读数只能同机同构建对拍：

- **读写配比是结果，不是设计量**：两边各自跑满同一窗口，次数 = 可达速率 × 窗口。读侧有 N 个线程、单次又比写便宜，
  所以天然读多写少：10 万行下 `StableRow` 2 读者约 7:1、`PointerSwap` 约 25:1，读写锁版 3000–19000:1（写者被挡）。
  要固定配比就得改成定工作量闭环（写者 W 条、每个读者 R 次，比总耗时）。
- **读写锁版写者的数字不稳定**：同一配置 1 s 窗口三次运行分别完成 503 / 974 / 8055 条；4 读线程 19–100 条，
  8 读线程 9–158 条。写者拿不到独占锁时会被长期挂起，能拿到多少取决于调度运气，加长或缩短窗口都消不掉这层方差。
  因此对读写锁版应看「能否进展」，而不是把它的数字当吞吐读。
- **短窗口会高估读写锁版**：64 行、300 ms 时 2 读者测到 976 条（约 3250/s），同一配置跑 3 s 只有 926–1609 条
  （约 300–540/s）——写者的进展几乎全发生在读者刚启动、还没形成持续读压的头几百毫秒里。
- 两种 RCU 形态的写者吞吐稳定且不随读者数崩塌，但 `StableRow` 明显高于只支持主键的 `PointerSwap`，差距随读线程数扩大
  （`StableRow` / `PointerSwap` 分别为 3.8× / 4.0× / 4.7×）。原因不在索引：B 的 `Update` 要做「`Allocate` 新行 → 发布指针 →
  `Retire` 旧行 → `AdvanceAndReclaim`（seq_cst 推进代数 + 逐槽扫 64 个读者登记 + 回收）」，
  B2 只是在原行上写两个版本号 + 两次 16 字节 `memcpy`。读者越多，B 扫到的读者槽位中有越多脏缓存行要跨核取，
  所以差距随读线程数增长——B 省掉的是索引结构，代价挪到了写侧。
- 本例是 Debug 构建，而 `ObjectPool` 的所有权登记（全局 `std::mutex` + `unordered_set` 增删）只在 `NDEBUG` 未定义时启用：
  `PointerSwap` 每次 `Update` 各付一次 `Allocate` / `Deallocate`，`StableRow` 一次都不付。这一项会放大差距，
  比较实现本身应跑 Release；本仓库当前只有 Debug 构建树，Release 数据待补。
- 两种 RCU 形态的读吞吐都在 9–20 M 次/秒（读线程 2→8，10 万行含缓存未命中），读侧都是「登记代数 + 哈希探测 + 拷贝」，
  成本同构；B2 多的 seqlock 校验在随机打点下几乎不触发重试。

### 10.2 加间隔（`interval` > 0，真实业务节奏）

真实业务里两次读写之间隔着计算与 IO，锁大部分时间是空的。两端每次操作后忙等命令行给出的微秒数，再跑同一个窗口。
以下四张表是 2026-10-27 本机 Debug 在 1 ms / 100 µs / 10 µs / 1 µs 四档的实测（1 s 窗口、10 万行）；
同一档位多次运行时取干净的一次，1 µs 档波动大，8 读线程给多次运行的区间。四档各 63 条断言全为 1、`failures=0`。

**1 ms（缺省档）**

| 实现 | 读线程 | 读次数 | 完成 `Update` | 耗时 ms |
| :--- | ---: | ---: | ---: | ---: |
| 读写锁版 | 2 | 1988 | 997 | 1001 |
| 读写锁版 | 4 | 3996 | 998 | 1001 |
| 读写锁版 | 8 | 7960 | 987 | 1001 |
| `PointerSwap` | 2 | 2000 | 995 | 1001 |
| `PointerSwap` | 4 | 3999 | 996 | 1001 |
| `PointerSwap` | 8 | 7888 | 980 | 1002 |
| `StableRow` | 2 | 2000 | 992 | 1001 |
| `StableRow` | 4 | 4000 | 998 | 1001 |
| `StableRow` | 8 | 7914 | 998 | 1001 |

两端都恰好达成设定的节拍：读者 N×1000 次/秒、写者约 1000 次/秒，读写锁版与两种 RCU 形态无差别。

**100 µs**

| 实现 | 读线程 | 读次数 | 完成 `Update` | 耗时 ms |
| :--- | ---: | ---: | ---: | ---: |
| 读写锁版 | 2 | 19770 | 9860 | 1000 |
| 读写锁版 | 4 | 39496 | 9835 | 1000 |
| 读写锁版 | 8 | 78686 | 9773 | 1000 |
| `PointerSwap` | 2 | 19863 | 9679 | 1000 |
| `PointerSwap` | 4 | 39680 | 9706 | 1000 |
| `PointerSwap` | 8 | 78931 | 9562 | 1000 |
| `StableRow` | 2 | 19885 | 9890 | 1000 |
| `StableRow` | 4 | 39745 | 9878 | 1000 |
| `StableRow` | 8 | 78891 | 9854 | 1000 |

三种实现仍各自跑满约 10 倍的节拍：写约 1 万次/秒、读约 N×1 万次/秒。

**10 µs**

| 实现 | 读线程 | 读次数 | 完成 `Update` | 耗时 ms |
| :--- | ---: | ---: | ---: | ---: |
| 读写锁版 | 2 | 179998 | 89569 | 1000 |
| 读写锁版 | 4 | 355429 | 88472 | 1000 |
| 读写锁版 | 8 | 338124 | 28627 | 1006 |
| `PointerSwap` | 2 | 192349 | 79980 | 1000 |
| `PointerSwap` | 4 | 385206 | 79085 | 1000 |
| `PointerSwap` | 8 | 326373 | 28679 | 1000 |
| `StableRow` | 2 | 192796 | 92666 | 1000 |
| `StableRow` | 4 | 384784 | 92191 | 1000 |
| `StableRow` | 8 | 327238 | 41666 | 1003 |

2 / 4 读线程仍跑满约 8–9 万次写；8 读线程三种实现**一起**掉到 2.9–4.2 万次，低于 10 万节拍的 1/3。

**1 µs**

| 实现 | 读线程 | 读次数 | 完成 `Update` | 耗时 ms |
| :--- | ---: | ---: | ---: | ---: |
| 读写锁版 | 2 | 852855 | 320699 | 1000 |
| 读写锁版 | 4 | 798895 | 827 | 1004 |
| 读写锁版 | 8 | 0.55–0.76 M | 56–1602 | 1030–1301 |
| `PointerSwap` | 2 | 1518176 | 286665 | 1000 |
| `PointerSwap` | 4 | 3064903 | 273690 | 1000 |
| `PointerSwap` | 8 | 2.37–5.73 M | 38204–216999 | 1000–1014 |
| `StableRow` | 2 | 1532188 | 581864 | 1000 |
| `StableRow` | 4 | 3038480 | 558416 | 1000 |
| `StableRow` | 8 | 2.18–5.69 M | 206827–493194 | 1000–1046 |

读写锁版从 4 读线程起崩：写者几十到一千六百次，每读者的读次数也掉到 2 读线程时的一半；8 读线程还要靠读者的
300 ms 自超时才能收场（耗时 1030–1301 ms）。两种 RCU 形态不受影响：2 / 4 读线程有 27–58 万次写、1.5–3.1 M 次读，
8 读线程 `PointerSwap` 仍有 3.8–21.7 万次写、`StableRow` 20.7–49.3 万次写。

**8 读线程的间隔梯度（写 / 读）**

| 操作间隔 | 读写锁版 写 / 读 | `PointerSwap` 写 / 读 | `StableRow` 写 / 读 |
| ---: | ---: | ---: | ---: |
| 1000 µs | 987 / 7960 | 980 / 7888 | 998 / 7914 |
| 500 µs | 1987 / 15754 | 1975 / 15829 | 1983 / 15929 |
| 100 µs | 9773 / 78686 | 9562 / 78931 | 9854 / 78891 |
| 10 µs | 28627 / 338124 | 28679 / 326373 | 41666 / 327238 |
| 1 µs | 56–1602 / 0.55–0.76 M | 38204–216999 / 2.37–5.73 M | 206827–493194 / 2.18–5.69 M |
| 0 µs | 7 / 529408 | 200746 / 18578432 | 931513 / 21532672 |

与 §10.1 的饱和结论互补，要点如下：

- **毫秒级到 100 µs 档都没有写者饥饿**。三种实现的写者与读者都跑满设定节拍，没有一个 `Update` 因拿不到锁而滞后。
  读写锁版在这些档位与两种 RCU 形态无差别：共享锁的空闲窗口远大于写者持锁时间（微秒级），读者之间也不再重叠。
- **10 µs 是分界的前一格**：8 读线程下三种实现一起掉到 2.9–4.2 万次写，说明这一档撞上的是本机调度上限
  （8 个忙等线程 + 1 个写者），而不是某一实现的锁问题。
- **1 µs 才分出高下**：读写锁版崩到几十到一千六百次写，且读者自身被共享锁的获取/释放争用拖慢一半
  （4 读线程每读者约 20 万次/秒，2 读线程时是 42 万次/秒）；两种 RCU 形态不受影响。分水岭在 10 µs 与 1 µs 之间，
  即读间隔接近锁本身的开销时。
- 1 µs 档已接近饱和而非「间隔」：RCU 单线程约 75 万次读/秒，即一次读连同忙等约 1.3 µs，忙等只占其中一部分，
  所以该档的极端值（`PointerSwap` 写者 3.8–21.7 万）与 0 µs 档同源，只能同次运行内比较。
- 间隔是**下限而非精确节拍**：忙等线程偶尔被系统抢占，整组读数会偏低（扫描间隔时 1 ms 的 8 读线程曾只测到
  3113 次读、446 次 `Update`，重跑两次都是约 7900 次读、约 990 次 `Update`），断言只要求「有进展」，不受影响。

测试只对正确性下断言，性能差异只打印不判定：不同机器、Debug/Release、CPU 核数都会改变绝对值，
把它写成断言就等于把调度噪声写进测试。两个测试程序共用的辅助（成对日期写入、断言记录类）在 `test/TestSupport/TestSupport.h`。

---

## 参考

- 表接口版本：[PointerSwapTables](../src/Mdb/Rcu/PointerSwap/PointerSwapTables.h)、[StableRowTables](../src/Mdb/Rcu/StableRow/StableRowTables.h)、
  [对照测试](../test/TestTables/TestTables.cpp)
- 上层设计：`D:\Gitlab\Trading\docs\C++-套利策略资管系统设计.md` §1（内存数据库：从读写锁到一写多读）
- 现有读接口与租约语义：[README](../README.md)
- 环境准备：[环境准备指南](environment-setup.md)
