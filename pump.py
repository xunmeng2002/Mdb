#!/usr/bin/env python
# -*- coding: utf-8 -*-
import xml.etree.cElementTree as ET
import sys
import re
import os

#sys.argv = ["pump.py", "DBIndexInterface.h", "DBIndexInterface.h.tpl", "mdb.xml"]

#自定义的异常
class MyException(Exception):
    def __init__(self, value):
        self.value = value
    def __str__(self):
        return repr(self.value)

#删掉旁路产物。产物的正式落点在 <目标>.pumptmp，只有成功替换后才消失
def discard_staged_output(staged_output_name):
    if os.path.exists(staged_output_name):
        os.remove(staged_output_name)
 
 
 #处理换行，换行时需要考虑缩进
def new_line():
    global _indent_cnt
    out_line = "\n"
    for i in range(_indent_cnt):
        out_line += "    "
    return out_line
 
 #处理!!entry!!标记
def handle_entry(in_str, entry_list):
    global _indent_cnt
    out_str = ""
    entry_list.append("entry")
    out_str += new_line()
    out_str += "entry_name = \"%s\"" % in_str[:-2].split(' ').pop()
    out_str += new_line()
    out_str += "parent%d = curr_node" % len(entry_list)
    out_str += new_line()
    out_str += "curr_node = curr_node.find(entry_name)"
    out_str += new_line()
    out_str += "parent_map[curr_node] = parent%d" % len(entry_list)
    out_str += new_line()
    return out_str
        
#处理!!leave!!
def handle_leave(in_str,entry_list):
    global _indent_cnt
    out_str = ""
    out_str += new_line()
    last_entry = entry_list.pop()
    if last_entry == "travel":
        _indent_cnt -= 1
        out_str += new_line()
        out_str += "pumpid = parentpumpid"
        out_str += new_line()
        out_str += "if curr_node != parent%d:" % (len(entry_list) + 1)
        _indent_cnt += 1
        out_str += new_line()
        out_str += "curr_node = parent_map[curr_node]"
        _indent_cnt -= 1
    if last_entry == "entry":
        out_str += new_line()
        out_str += "curr_node = parent_map[curr_node]"
    elif last_entry == "inc indent":
        #todo 抛出异常
        return ""            
    return out_str
        
#处理!!inc indent!!
def handle_inc_indent(in_str, entry_list):
    global _indent_cnt
    out_str = ""
    _indent_cnt += 1
    entry_list.append("inc indent")
    return out_str

#处理!!dec indent!!
def handle_dec_indent(in_str, entry_list):
    global _indent_cnt
    out_str = ""
    _indent_cnt -= 1
    entry_list.pop()
    #todo 检查配对，抛出异常
    return out_str

#处理!!travel!!
def handle_travel(in_str, entry_list):
    global _indent_cnt
    out_str=""
    entry_list.append("travel")
    out_str += new_line()    
    out_str += "parentpumpid = pumpid"
    out_str += new_line()
    out_str += "pumpid = -1"    #此处需要从-1开始，因为在遍历的时候，会先+1，这样才能保证第一次循环的pumpid为0
    out_str += new_line()
    out_str += "parent%d = curr_node" % len(entry_list)
    out_str += new_line()
    out_str += "for node%d in curr_node:" % len(entry_list)
    _indent_cnt += 1
    out_str += new_line()           
    out_str += "curr_node = node%d" % len(entry_list)
    out_str += new_line()
    out_str += "parent_map[curr_node] = parent%d" % len(entry_list)
    out_str += new_line()
    out_str += "pumpid += 1"
    return out_str  
        
#获取当前节点上指定属性名称的属性值 
def get_node_value(node, name):
    if node.attrib.get(name):
        return node.attrib.get(name)
    else:
        raise MyException("Invalid property: %s" % name)
                
#生成缩进
def gen_indent():
    out = ""
    for i in range(_indent_cnt):
        out += "    "
    return out
        
        
if __name__ == "__main__":
    #缩进量
    _indent_cnt = 0                
        
    if len(sys.argv) < 5:
        print("usage: pump destFile templateFile target modelFile")
        sys.exit(2)

    #第一个参数：输出文件名
    #第二个参数：模板文件 
    #第三个参数：目标
    #第四个参数：xml文件
    out_file_name = sys.argv[1]
    staged_output_name = out_file_name + ".pumptmp"
    tpl_file_name = sys.argv[2]
    target = sys.argv[3]
    
    #tpl文件中的python代码嵌入标记
    expr = "!!.*?!!"
    #临时python文件
    pump_file = open("pumptemp.py", "w+", encoding="UTF-8-SIG", newline='\n')

    #entry列表
    entry_list = []

    #打开模板文件
    pattern = re.compile(expr)        
    tpl_content = ""
    for line in open(tpl_file_name, "r", encoding="UTF-8-SIG"):
        word = line[:-1].split("!!")
        if len(word) < 3:
            tpl_content += line
        else:
            is_py = True
            for i in range(0, len(word)):
                if i % 2 == 0:
                    if word[i] != "":
                        tpl_content += line
                        is_py = False
                        break
            if is_py == True:
                tpl_content += line[:-1]

    #需要输出到dump_temp中的内容
    out_content = ""
    #写入基本信息
    out_content += "#coding:utf-8\n" 
    out_content += "import xml.etree.cElementTree as ET\n"
    out_content += "import sys\n\n"
    out_content += "out_file = open('%s', 'w+', encoding='UTF-8-SIG', newline='\\n')\n\n" % staged_output_name
    #每个生成物顶部写入「勿手改」头注释。注释符按扩展名白名单取；白名单外一律失败——
    #写错前缀会让产物变成语法非法的文件，而且退出码还是 0，事后极难发现
    comment_prefix_by_extension = {".c": "// ", ".cpp": "// ", ".h": "// ", ".hpp": "// ", ".cs": "// ", ".sql": "-- "}
    extension = os.path.splitext(out_file_name)[1].lower()
    if extension not in comment_prefix_by_extension:
        raise MyException("Unknown extension for comment prefix:%s" % extension)
    comment_prefix = comment_prefix_by_extension[extension]
    header = "%s本文件由 %s 生成；请勿手改，改动请改模板后重跑 pumpall.py" % (comment_prefix, tpl_file_name)
    out_content += "out_file.write(%s)\n" % repr(header + "\n")
    #xml文件可以大于1，如果有多个xml文件，添加一个根节点，把每个xml文件的根节点挂在新加的根节点下面，新加的根节点作为当前节点
    out_content += "curr_node = ET.Element(\"root\")\n"
    for i in range(4, len(sys.argv)):
        out_content += "curr_node.append(ET.parse(\"%s\").getroot())\n" % sys.argv[i]
    out_content += "parent_map = {}\n"
    out_content += "pumpid = 0\n"
    out_content += "parentpumpid = 0\n"
    out_content += "target = '%s'\n" % target
    
    #定义获取属性的方法，如果在当前节点无法取得，就去父节点取，知道根节点
    out_content += "def get_attr(node, name):\n"
    out_content += "    while node != None:\n"
    out_content += "        if node.get(name) == None:\n"
    out_content += "            if node in parent_map:\n"
    out_content += "                node = parent_map[node]\n"
    out_content += "            else:\n"
    out_content += "                break\n"
    out_content += "        else:\n"
    out_content += "            return node.get(name)\n"
    out_content += "    return \"\"\n"

    #查找匹配项
    itr = pattern.finditer(tpl_content)
    last_pos = 0
    for match in itr:
        #print(match.group())
        #从上一个match到这个match之间的内容
        common_str = tpl_content[last_pos:match.start()]
        last_pos = match.end()
        
        #不为空才处理
        if common_str != "":
            common_str = common_str.replace("\\", "\\\\")
            common_str = common_str.replace("\"", "\\\"")
                        
            #查找是否存在换行符
            start = 0
            pos = common_str.find('\n', start)
            while pos != -1:
                out_content += new_line()
                out_content += "out_file.write(\"%s\\n\")" % common_str[start:pos]
                start = pos + 1
                pos = common_str.find('\n', pos + 1)
            out_content += new_line()
            out_content += "out_file.write(\"%s\")" % common_str[start:]
        
        if match.group().find("!!entry") != -1:
            out_content += handle_entry(match.group(),  entry_list)
        elif match.group().find("!!leave!!") != -1:
            out_content += handle_leave(match.group(), entry_list)
        elif match.group().find("!!inc indent") != -1:
            out_content += handle_inc_indent(match.group(), entry_list)
        elif match.group().find("!!dec indent") != -1:
            out_content += handle_dec_indent(match.group(), entry_list)
        elif match.group().find("!!travel") != -1:
            out_content += handle_travel(match.group(), entry_list)
        #!!@*!!的情况 只包含一个@标签
        elif match.group().find("!!@") != -1:
            #以!!@开头，表示字符串，直接打印，不带引号
            s = match.group()
            p = s.find("@")
            begin = p = p + 1
            while s[p].isalpha() or s[p] == "_":
                p += 1
            end = p
            attr = s[begin:end]
            if attr == "tag":
                out_content += new_line()
                out_content += "out_file.write(\"%s\" %"
                out_content += " curr_node.tag "
                out_content += ")"
            else:
                out_content += new_line()
                out_content += "out_file.write(\"%s\" %"
                out_content += " get_attr(curr_node, \"%s\")" % attr
                #out_content += " curr_node.get(\"%s\")" % attr 
                out_content += ")"
        #!!$*!!的情况 只包含一个$标签
        elif match.group().find("!!$") != -1:
            #以!!$开头，表示字符串，直接打印，不带引号
            s = match.group()
            p = s.find("$")
            begin = p = p + 1
            while s[p].isalpha() or s[p] == "_":
                p += 1
            end = p
            var = s[begin:end]
            out_content += new_line()
            out_content += "out_file.write(\"%s\" %"
            out_content += " str(%s)" % var
            out_content += ")"
        #以!!*@*!! 与 !!*$*!!的情况  可能会有多个@与$
        elif match.group().find("@") != -1 or match.group().find("$") != -1:
            s = match.group()
            s1 = ""
            length = len(s)
            p = 2
            while p < length - 2:
                #处理!!*@*!!的情况
                if s[p] == "@":
                    begin = p = p + 1
                    while s[p].isalpha() or s[p] == "_":
                        p += 1
                    end = p
                    attr = s[begin:end]
                    if attr == "tag":
                        s1 += " curr_node.tag"
                    else:
                        #s1 += " curr_node.get(\"%s\")" % attr   
                        s1 += " get_attr(curr_node, \"%s\")" % attr
                #处理!!*$*!!的情况
                elif s[p] == "$":
                    begin = p = p + 1
                    while s[p].isalpha() or s[p] == "_":
                        p += 1
                    end = p
                    var = s[begin:end]
                    s1 += "str(%s)" % var
                else:
                    s1 += s[p]
                    p += 1
            out_content += new_line()
            out_content += s1
        else:
            out_content += new_line()
            out_content += match.group()[2:-2]
    #从最后一个match结束到文件尾
    common_str = tpl_content[last_pos:]
    #不为空才处理
    if common_str != "":
        common_str = common_str.replace("\\", "\\\\")
        common_str = common_str.replace("\"", "\\\"")
        #作用域到达不了这里last_pos = match.end()
        #查找是否存在换行符
        start = 0
        pos = common_str.find('\n')
        while pos != -1:
            out_content += new_line()
            out_content += "out_file.write(\"%s\\n\")" % common_str[start:pos]
            start = pos + 1
            pos = common_str.find('\n', pos + 1)
        out_content += new_line()
        out_content += "out_file.write(\"%s\")" % common_str[start:]
    #结束关闭文件
    out_content += new_line()
    out_content += "out_file.close()\n"
    #写入临时文件
    pump_file.write(out_content)
    pump_file.close()
    
    if len(entry_list) != 0:
        raise MyException("Tag Not Match:%s" % str(entry_list))
    
    #执行临时文件：产物先写旁路文件，成功才替换目标；失败保留原产物并响亮退出
    if os.system("python pumptemp.py") != 0:
        discard_staged_output(staged_output_name)
        print("pump failed: %s <- %s" % (out_file_name, tpl_file_name))
        sys.exit(1)

    #替换目标：能原子替换就替换；替换被别的进程挡下（Windows 上目标被打开着时
    #os.replace 需要 DELETE 权限，而旧的就地重写只需要写权限）则退回就地重写
    try:
        os.replace(staged_output_name, out_file_name)
    except OSError as replace_error:
        try:
            with open(staged_output_name, "rb") as staged_file:
                staged_bytes = staged_file.read()
            with open(out_file_name, "wb") as out_file:
                out_file.write(staged_bytes)
        except OSError:
            discard_staged_output(staged_output_name)
            print("pump failed: %s <- %s: %s" % (out_file_name, tpl_file_name, replace_error))
            sys.exit(1)
        print("pump warning: %s replaced in place: %s" % (out_file_name, replace_error))
    discard_staged_output(staged_output_name)

    #执行完毕后删除临时文件
    #os.remove("pumptemp.py")

