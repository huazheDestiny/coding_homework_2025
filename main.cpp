//主文件，根据命令选择相应的操作
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cctype>

#include "LinkedList.h"
#include "CircularQueue.h"
#include "BST.h"

// 最大命令长度
#define MAX_CMD_LEN 100
// 定义STATS命令结果缓冲区的最大大小
#define MAX_STATS_BUFFER 50000 

// 辅助函数1：不区分大小写的字符串比较
int strCaseCmp(const char* s1, const char* s2) {
    while (*s1 && *s2) {
        int c1 = tolower((unsigned char)*s1);
        int c2 = tolower((unsigned char)*s2);
        if (c1 != c2) return c1 - c2;
        s1++;
        s2++;
    }
    return tolower((unsigned char)*s1) - tolower((unsigned char)*s2);
}

// 辅助函数：去除字符串两端的空白字符
void trim(char* str) {
    if (!str) return;
    size_t len = strlen(str);
    while (len > 0 && isspace((unsigned char)str[len - 1])) {
        str[--len] = '\0';
    }
}

//LOAD命令的实现
void handleLoad(const char* filename, LinkedList& list, CircularQueue& queue, BST& bst) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        // 打开失败，文件不存在
        std::cout << "Error: Cannot open file " << filename << std::endl;
        return;
    }

    char lineBuffer[MAX_LINE_LENGTH];
    int count = 0;

    while (fgets(lineBuffer, sizeof(lineBuffer), fp)) {
        // 去除行尾换行符
        size_t len = strlen(lineBuffer);
        if (len > 0 && lineBuffer[len - 1] == '\n') {
            lineBuffer[len - 1] = '\0';
        }

        if (len == 0) continue; // 跳过空行

        // 解析日志行
        LogEntry entry;
        strncpy(entry.max_line, lineBuffer, MAX_LINE_LENGTH - 1);
        entry.max_line[MAX_LINE_LENGTH - 1] = '\0';

        char* ptr = lineBuffer;

        //  解析时间戳
        char* startBracket = strchr(ptr, '[');
        char* endBracket = strchr(ptr, ']');
        if (!startBracket || !endBracket || endBracket < startBracket) continue;

        *endBracket = '\0'; 
        strncpy(entry.max_time, startBracket + 1, MAX_TIME - 1);
        entry.max_time[MAX_TIME - 1] = '\0';
        *endBracket = ']'; 
        ptr = endBracket + 1;

        // 跳过空格
        while (*ptr == ' ') ptr++;

        // 解析级别
        char* space1 = strchr(ptr, ' ');
        if (!space1) continue;
        *space1 = '\0';
        strncpy(entry.max_level, ptr, MAX_LEVEL - 1);
        entry.max_level[MAX_LEVEL - 1] = '\0';
        ptr = space1 + 1;

        // 跳过空格
        while (*ptr == ' ') ptr++;

        // 解析模块
        char* space2 = strchr(ptr, ' ');
        if (!space2) continue;
        *space2 = '\0';
        strncpy(entry.max_module, ptr, MAX_MODULE - 1);
        entry.max_module[MAX_MODULE - 1] = '\0';
        ptr = space2 + 1;

        // 跳过空格
        while (*ptr == ' ') ptr++;

        // 4. 解析消息
        strncpy(entry.max_message, ptr, MAX_MASSAGE - 1);
        entry.max_message[MAX_MASSAGE - 1] = '\0';

        // 尾插插入链表
        list.insertTail(entry);

        //节点指针入队
        ListNode* newNode = list.getTail();
        queue.enqueue(newNode);

        //如果是 ERROR 级别，更新 BST
        if (strcmp(entry.max_level, "ERROR") == 0) {
            bst.updateCount(entry.max_module, 1);
        }

        count++;
    }

    fclose(fp);
    std::cout << "Loaded " << count << " entries" << std::endl;
}

//FILTER命令的实现
void handleFilter(const char* startArg, const char* endArg, LinkedList& list) {    
    char startTime[MAX_TIME];
    char endTime[MAX_TIME];

    strncpy(startTime, startArg, MAX_TIME - 1);
    startTime[MAX_TIME - 1] = '\0';
    
    strncpy(endTime, endArg, MAX_TIME - 1);
    endTime[MAX_TIME - 1] = '\0';

    // 替换下划线为空格
    char* s_und = strchr(startTime, '_');
    if (s_und) *s_und = ' ';
    
    char* e_und = strchr(endTime, '_');
    if (e_und) *e_und = ' ';

    // 遍历链表
    ListNode* current = list.getHead();
    while (current != nullptr) {
        if (strcmp(current->data.max_time, startTime) >= 0 &&
            strcmp(current->data.max_time, endTime) <= 0) {
            std::cout << current->data.max_line << std::endl;
        }
        current = current->next;
    }
}

//STATS命令的实现
void handleStats(BST& bst) {
    // 分配足够大的数组来存储统计结果
    // 使用 new 分配在堆上，避免栈溢出
    StatsEntry* statsArray = new StatsEntry[MAX_STATS_BUFFER];
    int size = 0;
    bst.getStats(statsArray, size);

    delete[] statsArray;
}

int main() {
    // 实例化核心数据结构
    LinkedList list;
    CircularQueue queue;
    BST bst;

    char command[MAX_CMD_LEN];

    // 进入交互式循环
    while (true) {
         std::cout << "> " << std::flush; 

        // 读取命令
        if (scanf("%s", command) == EOF) {
            break;
        }

        if (strCaseCmp(command, "LOAD") == 0) {
            char filename[100];
            scanf("%s", filename);
            handleLoad(filename, list, queue, bst);
        } 
        else if (strCaseCmp(command, "FILTER") == 0) {
            char start[MAX_TIME * 2], end[MAX_TIME * 2];
            scanf("%s %s", start, end);
            handleFilter(start, end, list);
        }
        else if (strCaseCmp(command, "STATS") == 0) {
            handleStats(bst);
        }
        else if (strCaseCmp(command, "RECENT") == 0) {
            int n;
            scanf("%d", &n);
            queue.traverseQueue(n);
        }
        else if (strCaseCmp(command, "SEARCH") == 0) {
            // 需要 KMP 算法支持，此处留空
            char keyword[100];
            scanf("%s", keyword);
            std::cout << "Command SEARCH not implemented yet." << std::endl;
        }
        else if (strCaseCmp(command, "DELETE") == 0) {
            // 需要 UndoStack 支持，此处留空
            int lineNum;
            scanf("%d", &lineNum);
            std::cout << "Command DELETE not implemented yet." << std::endl;
        }
        else if (strCaseCmp(command, "UNDO") == 0) {
            // 需要 UndoStack 支持，此处留空
            std::cout << "Command UNDO not implemented yet." << std::endl;
        }
        else if (strCaseCmp(command, "EXIT") == 0) {
            break;
        }
        else {
            // 未知命令处理
            char temp[256];
            fgets(temp, 256, stdin);
            std::cout << "Unknown command" << std::endl;
        }
    }

    return 0;
}
