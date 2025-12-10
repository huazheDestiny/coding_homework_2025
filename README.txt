数据结构与算法的大作业简介

1.文件结构 
project_2025
|--LinkedList.h
|--LinkedList.cpp
|--BST.h
|--BST.cpp
|--CircularQueue.h
|--CircularQueue.cpp
|--KMP.h
|--KMP.cpp
|--UndoStack.h
|--UndoStack.cpp
|--Makefile
|--README.txt
|--23307140005.txt

2.编译命令：解压压缩包，在当前目录下执行：
   make，生成可执行文件 loganalyzer.exe，终端输入.\loganalyzer.exe (或者windows下直接输入loganalyzer.exe)运行可执行文件，进入交互模式
   make clean 删除可执行文件loganalyzer.exe
   在Makefile中注释掉CXXFLAGS = -std=c++11 -Wall -g 这一行，并取消掉 # CXXFLAGS = -std=c++11 -Wall -O0 -gdwarf-2 这一行的注释，然后命令提示行运行drmemory -- loganalyzer.exe
   可以使用windows上的Valgrind的替代工具Dr.memory检测内存溢出和栈溢出

3.运行方法：把日志文件.txt放到当前文件夹。生成可执行文件后，当前目录执行.\loganalyzer.exe，即可进入交互模式，一共有8种命令：
           LOAD：读取日志文件.txt，构建数据结构
           FILTER: 格式为FILTER YYYY-MM-DD_HH:MM:SS YYYY-MM-DD_HH:MM:SS，从日志文件筛选指定时间范围内的条目并原样输出
           SEARCH： 搜索关键词，输出匹配日志数目和每条日志原文
           STATS：输出ERROR数目，由多到少，数目相同按字典序
           DELETE：删除指定行号
           UNDO：撤销DELETE，最多撤销5次
           RECENT：输出最近N条日志
           EXIT：删除指定的数据

4.数据结构设计简述：
    双向链表LinkedList:用于管理日志条目。链表中的每个节点包含一个LogEntry结构体，
    该结构体存储了日志的时间戳、级别、模块名和具体消息内容。每个节点维护指向前驱和后继节点的指针，
    以及一个行号字段用于标识其在链表中的位置。链表本身通过head和tail指针维护头尾节点的引用，
    并记录总节点数。实现了标准的链表操作，包括头尾插入、按行号删除、按行号获取节点等功能。
    遍历操作支持正向和反向遍历。还提供了深拷贝和指定位置插入的操作，用于实现撤销重做等功能。

    环形队列CircularQueue:存储指向链表节点的指针，用于缓存最近的日志条目。
    采用动态数组实现，维护队头和队尾指针，当队列满时新元素会覆盖最早的元素。
    环形队列支持入队、出队和指定节点删除操作，可遍历最后n条记录。深拷贝时会根据新链表中的节点重新建立指针映射。

    二叉搜索树BST：用于统计各模块的错误数量。每个节点包含模块名和对应的错误计数，按照模块名的字典序组织。
    支持动态更新错误计数，当某模块的错误数降至零或以下时自动删除该节点。
    通过中序遍历可以获得按错误数降序、同等错误数下按模块名排序的统计结果。提供了深拷贝功能以支持撤销操作。

    KMP算法：用于在日志消息中搜索关键词的SEARCH命令。首先通过计算next数组来优化匹配过程，避免每次都回溯主串。
    遍历整个链表对每条日志的消息内容进行搜索，统计匹配数量并输出包含关键词的完整日志行。

    撤回栈UndoStack：撤回栈是一个固定大小的数组栈，最多记录五次删除操作。每个栈元素存储被删除的日志条目及其删除时的行号，用于实现撤销功能。
    支持压栈保存删除记录和出栈恢复操作，当栈满时新的删除操作无法记录。

5.已知问题说明：
   1.重复load问题：因为大作业要求没有写是否会在一次loganalyzer的执行周期里反复执行load命令，如果所有数据结构不在下一次load
                  的时候清空删除，那么下一次执行load的时候还会有数据结构残留。比如我在测试的时候先执行了load test1.txt，之后
                  没有exit的情况下就在执行load empty.txt，之后stats,会输出3 errors。为提升程序鲁棒性，添加每次load之前调用每个
                  数据结构各自的clear或者delete方法清空数据结构，解决了重复load数据结构残留的问题。

   2.Dr.memory栈溢出问题：执行drmemory -- loganalyzer.exe 的时候会出现栈溢出问题,如下：
                  Error #1: UNADDRESSABLE ACCESS beyond top of stack: reading 0x00000098fbbff6b0-0x00000098fbbff6b8 8 byte(s)
# 0 ___chkstk_ms   
# 1 _pei386_runtime_relocator
# 2 __tmainCRTStartup
# 3 .l_start       
# 4 ntdll.dll!RtlUserThreadStart
Note: @0:00:00.124 in thread 23108
Note: 0x00000098fbbff6b0 refers to 664 byte(s) beyond the top of the stack 0x00000098fbbff948
Note: instruction: or     $0x0000000000000000 (%rcx) -> (%rcx)
   我查询了一下AI，这个问题系drmemory的内部bug，在main执行之前就已经出现，并非栈溢出，没有使用过valgrind检测过，可以试试。如果理解错了，
   希望老师或助教能指导一下，谢谢！

   3.优化排序速度问题：algorithm库种有一个排序函数sort，其采用的是混合排序的方法，时间复杂度大约是O(NlogN)。我使用的是冒泡排序算法，
   BST::getStats中。为了获得更快的加速体验，显著提升RECENT指令的速度，可以使用sort。
