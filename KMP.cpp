#include "KMP.h"
#include <iostream>
#include <cstring>
#include <cstdio>

//计算next数组
void KMP::computeNext(const char* pattern, int* next, int len) {
    if (len == 0) return;

    next[0] = 0; // 第一个字符的 next 值总是 0
    int k = 0;   // k 代表当前共同前后缀的长度
    for (int q = 1; q < len; q++) {
        // 字符不匹配回退 k
        while (k > 0 && pattern[k] != pattern[q]) {
            k = next[k - 1];
        }
        // 字符匹配，共同长度 +1
        if (pattern[k] == pattern[q]) {
            k++;
        }
        next[q] = k;
    }
}

//kmp匹配函数
bool KMP::kmpMatch(const char* text, const char* pattern, const int* next, int patLen) {
    if (!text || !pattern || patLen == 0) return false;

    int n = strlen(text);
    int q = 0; // q 表示目前已经匹配了模式串的多少个字符

    for (int i = 0; i < n; i++) {
        while (q > 0 && pattern[q] != text[i]) {
            q = next[q - 1];
        }
        
        if (pattern[q] == text[i]) {
            q++;
        }

        if (q == patLen) {
            return true; // 找到匹配，返回 true
        }
    }
    return false;
}

//遍历日志条目搜索关键词
void KMP::searchLogs(const LinkedList& list, const char* keyword) {
    if (keyword == nullptr || strlen(keyword) == 0) {
        std::cout << "Found 0 match(es)" << std::endl;
        return;
    }

    int patLen = strlen(keyword);
    //分配next动态数组
    int* next = new int[patLen];

    //计算next数组
    computeNext(keyword, next, patLen);

    int matchCount = 0;

    //遍历链表进行搜索
    ListNode* current = list.getHead();
    
    //计数
    current = list.getHead();
    while (current != nullptr) {
        // 搜索message中内容
        if (kmpMatch(current->data.max_message, keyword, next, patLen)) {
            matchCount++;
        }
        current = current->next;
    }

    std::cout << "Found " << matchCount << " match(es)" << std::endl;

    //打印
    if (matchCount > 0) {
        current = list.getHead();
        while (current != nullptr) {
            if (kmpMatch(current->data.max_message, keyword, next, patLen)) {
                std::cout << current->data.max_line << std::endl;
            }
            current = current->next;
        }
    }

    // 释放内存
    delete[] next;
}