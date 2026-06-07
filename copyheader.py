#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import shutil

def CopyFolder(src, dest):
    """拷贝整个文件夹"""
    if os.path.exists(dest):
        print("删除旧目录: %s" % dest)
        shutil.rmtree(dest)
    
    print("拷贝目录: %s -> %s" % (src, dest))
    shutil.copytree(src, dest)

def CopyFiles(src_pattern, dest_folder):
    """拷贝匹配的文件到目标文件夹"""
    # 确保目标文件夹存在
    if not os.path.exists(dest_folder):
        os.makedirs(dest_folder)
    
    # 拷贝所有 .lib 文件
    for file in os.listdir(src_pattern):
        if file.endswith('.lib'):
            src_file = os.path.join(src_pattern, file)
            dest_file = os.path.join(dest_folder, file)
            print("拷贝文件: %s -> %s" % (src_file, dest_file))
            shutil.copy2(src_file, dest_file)

def CopyDlls(src_folder, dest_folder):
    """拷贝所有 dll 文件"""
    if not os.path.exists(dest_folder):
        os.makedirs(dest_folder)
    
    for file in os.listdir(src_folder):
        if file.endswith('.dll'):
            src_file = os.path.join(src_folder, file)
            dest_file = os.path.join(dest_folder, file)
            print("拷贝文件: %s -> %s" % (src_file, dest_file))
            shutil.copy2(src_file, dest_file)

if __name__ == "__main__":
    target_base = "../Libs/PersonalLib"
    
    # 1. 拷贝 include 目录
    CopyFolder("include", os.path.join(target_base, "include"))
    
    # 2. 拷贝 lib/*.lib 到 win64 目录
    CopyFiles("lib", os.path.join(target_base, "win64"))
    
    # 3. 拷贝 bin/*.dll 到 win64 目录
    CopyDlls("bin", os.path.join(target_base, "win64"))
    
    print("完成!")