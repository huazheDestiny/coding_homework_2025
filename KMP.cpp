#include "KMP.h"
#include <iostream>
#include <cstring>
#include <cstdio>

/**
 * @brief 计算 Next 数组
 * 逻辑：next[i] 表示 pattern[0...i] 这个子串中，
 * 前缀和后缀相等的最大长度。
 */
void KMP::computeNext(const char* pattern, int* next, int len) {
    if (len == 0) return;

    next[0] = 0; // 第一个字符的 next 值总是 0
    int k = 0;   // k 代表当前共同前后缀的长度

    // q 从 1 开始遍历模式串
    for (int q = 1; q < len; q++) {
        // 如果字符不匹配，回退 k
        while (k > 0 && pattern[k] != pattern[q]) {
            k = next[k - 1];
        }
        // 如果字符匹配，共同长度 +1
        if (pattern[k] == pattern[q]) {
            k++;
        }
        next[q] = k;
    }
}

/**
 * @brief KMP 匹配逻辑
 * 使用预计算的 next 数组在 text 中查找 pattern
 */
bool KMP::kmpMatch(const char* text, const char* pattern, const int* next, int patLen) {
    if (!text || !pattern || patLen == 0) return false;

    int n = strlen(text);
    int q = 0; // q 代表目前已经匹配了模式串的多少个字符

    for (int i = 0; i < n; i++) {
        // 如果 text[i] 和 pattern[q] 不匹配，根据 next 数组回退 q
        while (q > 0 && pattern[q] != text[i]) {
            q = next[q - 1];
        }
        
        // 如果匹配，匹配长度 +1
        if (pattern[q] == text[i]) {
            q++;
        }

        // 如果 q 等于模式串长度，说明完全匹配成功
        if (q == patLen) {
            return true; // 找到匹配，返回 true
            // 如果需要找所有出现位置，这里可以让 q = next[q-1] 继续找
        }
    }
    return false;
}

/**
 * @brief 搜索整个链表
 */
void KMP::searchLogs(const LinkedList& list, const char* keyword) {
    if (keyword == nullptr || strlen(keyword) == 0) {
        std::cout << "Found 0 match(es)" << std::endl;
        return;
    }

    int patLen = strlen(keyword);
    
    // 1. 在堆上分配 next 数组，避免栈溢出（如果 keyword 非常长）
    int* next = new int[patLen];

    // 2. 预计算 Next 数组 (只需要计算一次！)
    computeNext(keyword, next, patLen);

    int matchCount = 0;

    // 3. 遍历链表进行搜索
    ListNode* current = list.getHead();
    
    // --- 第一遍：计数 ---
    current = list.getHead();
    while (current != nullptr) {
        // 搜索目标是 max_message (消息内容)
        if (kmpMatch(current->data.max_message, keyword, next, patLen)) {
            matchCount++;
        }
        current = current->next;
    }

    std::cout << "Found " << matchCount << " match(es)" << std::endl;

    // --- 第二遍：打印 (仅当有匹配时) ---
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