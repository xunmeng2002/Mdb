#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import sys
from pathlib import Path

def detect_line_ending(content):
    """检测文件的行尾符类型"""
    if b'\r\n' in content:
        return '\r\n'  # Windows CRLF
    elif b'\r' in content:
        return '\r'    # 老式 Mac CR
    else:
        return '\n'    # Unix/Linux LF

def convert_to_utf8_bom(file_path, dry_run=False):
    """将文件转换为 UTF-8 with BOM 格式，保持原始行尾符"""
    try:
        # 读取文件内容（二进制模式）
        with open(file_path, 'rb') as f:
            content = f.read()
        
        # 检查是否已有 BOM
        has_bom = content.startswith(b'\xef\xbb\xbf')
        
        if has_bom:
            return False, "已有 BOM，跳过"
        
        # 检测原始行尾符
        original_line_ending = detect_line_ending(content)
        
        # 尝试解码文件（处理不同编码）
        decoded = False
        text = None
        
        try:
            # 尝试 UTF-8
            text = content.decode('utf-8')
            decoded = True
        except UnicodeDecodeError:
            try:
                # 尝试 GBK/CP936（中文系统）
                text = content.decode('gbk')
                decoded = True
                print(f"  ⚠ 使用 GBK 编码解码: {file_path.name}")
            except UnicodeDecodeError:
                try:
                    # 尝试 GB18030（更全的中文编码）
                    text = content.decode('gb18030')
                    decoded = True
                    print(f"  ⚠ 使用 GB18030 编码解码: {file_path.name}")
                except UnicodeDecodeError:
                    try:
                        # 尝试系统默认编码
                        text = content.decode('latin-1')
                        decoded = True
                        print(f"  ⚠ 使用 Latin-1 编码解码: {file_path.name}")
                    except:
                        pass
        
        if not decoded:
            return False, "无法解码文件"
        
        # 规范化行尾符：将各种行尾符统一为原始格式
        # 先将所有行尾符标准化为 \n
        text = text.replace('\r\n', '\n').replace('\r', '\n')
        # 再转换回原始行尾符格式
        if original_line_ending != '\n':
            text = text.replace('\n', original_line_ending)
        
        if not dry_run:
            # 写入 UTF-8 with BOM，使用二进制模式以精确控制
            encoded_content = text.encode('utf-8-sig')
            with open(file_path, 'wb') as f:
                f.write(encoded_content)
        
        return True, "已转换"
    
    except Exception as e:
        return False, f"转换失败: {str(e)}"

def should_exclude(file_path, exclude_dirs, exclude_patterns):
    """检查文件是否应该被排除"""
    file_str = str(file_path)
    parts = file_str.split(os.sep)
    
    # 检查排除的目录
    for exclude_dir in exclude_dirs:
        if exclude_dir in parts:
            return True
    
    # 检查排除的模式
    for pattern in exclude_patterns:
        if pattern in file_str:
            return True
    
    return False

def main():
    # 配置参数
    root_dir = Path(".")  # 当前目录，或修改为具体路径
    extensions = [".h", ".cpp", ".cu", ".cuh"]  # 要处理的文件扩展名
    
    # 排除的文件夹名称（完全匹配）
    exclude_folders = [
        ".git", ".svn", ".hg",  # 版本控制
        "build", "Debug", "Release",  # 编译输出
        "x64", "x86", "Win32",
        "lib", "Lib", "LIB",  # 第三方库
        "3rdparty", "third_party", "ThirdParty", "external",
        "vcpkg", "packages",
        "node_modules", "venv", "env",  # Python
        ".vs", ".vscode", "temp", "tmp"  # IDE和临时文件
    ]
    
    # 排除的路径模式（部分匹配）
    exclude_patterns = [
        "cmake-build", "CMakeFiles",
        "out", "output", "bin", "obj"
    ]
    
    # 是否只预览（不实际修改文件）
    dry_run = False
    if len(sys.argv) > 1 and sys.argv[1] == "--dry-run":
        dry_run = True
        print("=== 预览模式（不会修改文件）===\n")
    
    print(f"开始处理目录: {root_dir.absolute()}")
    print(f"文件类型: {', '.join(extensions)}")
    print(f"排除文件夹: {', '.join(exclude_folders)}")
    print("-" * 60)
    
    converted = 0
    skipped = 0
    errors = 0
    
    # 递归查找文件
    for ext in extensions:
        for file_path in root_dir.rglob(f"*{ext}"):
            # 检查是否应该排除
            if should_exclude(file_path, exclude_folders, exclude_patterns):
                print(f"⏭ 排除: {file_path}")
                skipped += 1
                continue
            
            # 转换文件
            success, message = convert_to_utf8_bom(file_path, dry_run)
            
            if success:
                if not dry_run:
                    print(f"✓ {file_path.relative_to(root_dir)} - {message}")
                else:
                    print(f"[预览] ✓ {file_path.relative_to(root_dir)}")
                converted += 1
            elif "已有 BOM" in message:
                print(f"○ {file_path.relative_to(root_dir)} - {message}")
                skipped += 1
            else:
                print(f"✗ {file_path.relative_to(root_dir)} - {message}")
                errors += 1
    
    print("-" * 60)
    print(f"完成！转换: {converted}, 跳过: {skipped}, 错误: {errors}")
    if dry_run:
        print("\n提示: 去掉 --dry-run 参数运行以实际修改文件")

if __name__ == "__main__":
    main()
    
    