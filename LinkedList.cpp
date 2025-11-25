//双向链表的实现文件
#include "LinkedList.h"
#include <iostream>
#include <cstring>
#include <cstdio>

// ==========================================
// ListNode 结构体实现
// ==========================================

ListNode::ListNode(const LogEntry& entry, int number) {
    this->data = entry; // 结构体直接赋值，会拷贝内部的 char 数组
    this->line_number = number;
    this->next = nullptr;
    this->prev = nullptr;
}

// ==========================================
// LinkedList 类实现
// ==========================================

// 构造函数
LinkedList::LinkedList() {
    head = nullptr;
    tail = nullptr;
    size = 0;
}

// 析构函数
LinkedList::~LinkedList() {
    clear();
}

// 清空链表
void LinkedList::clear() {
    ListNode* current = head;
    while (current != nullptr) {
        ListNode* nextNode = current->next;
        delete current;
        current = nextNode;
    }
    head = nullptr;
    tail = nullptr;
    size = 0;
}

// 头插法
// 注意：头插会导致所有现有节点的行号 +1，这是一个 O(N) 操作
int LinkedList::insertHead(const LogEntry& entry) {
    ListNode* newNode = new ListNode(entry, 1); // 新头节点行号永远是 1

    if (head == nullptr) {
        head = newNode;
        tail = newNode;
    } else {
        newNode->next = head;
        head->prev = newNode;
        head = newNode;

        // 重新编号：除了新头节点外，后面所有节点行号 +1
        ListNode* current = head->next;
        while (current != nullptr) {
            current->line_number++;
            current = current->next;
        }
    }
    size++;
    return 1; // 返回插入的行号
}

// 尾插法
// 这是最常用的插入方式，行号直接是 size + 1，O(1)
int LinkedList::insertTail(const LogEntry& entry) {
    int newLineNum = size + 1;
    ListNode* newNode = new ListNode(entry, newLineNum);

    if (tail == nullptr) {
        head = newNode;
        tail = newNode;
    } else {
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }
    size++;
    return newLineNum; // 返回插入的行号
}

// 删除指定行号
// 1. 找到节点
// 2. 断开连接
// 3. 释放内存
// 4. 后续节点行号重排 (重要要求)
int LinkedList::deleteAt(int line_number) {
    if (line_number < 1 || line_number > size) {
        return 0; // 删除失败，行号无效
    }

    // 找到要删除的节点
    // 为了提高查找效率，判断是从头找还是从尾找
    ListNode* target = nullptr;
    if (line_number <= size / 2) {
        target = head;
        while (target != nullptr && target->line_number != line_number) {
            target = target->next;
        }
    } else {
        target = tail;
        while (target != nullptr && target->line_number != line_number) {
            target = target->prev;
        }
    }

    if (target == nullptr) return 0; // 防御性编程，理论上不会发生

    // 记录下一个节点，用于后续重编号
    ListNode* nextNode = target->next;

    // 断开连接处理
    if (target == head && target == tail) {
        // 链表只有一个节点
        head = nullptr;
        tail = nullptr;
    } else if (target == head) {
        // 删除头节点
        head = target->next;
        if (head) head->prev = nullptr;
    } else if (target == tail) {
        // 删除尾节点
        tail = target->prev;
        if (tail) tail->next = nullptr;
    } else {
        // 删除中间节点
        target->prev->next = target->next;
        target->next->prev = target->prev;
    }

    delete target;
    size--;

    // 重新编号：被删除节点之后的所有节点行号减 1
    ListNode* current = nextNode;
    while (current != nullptr) {
        current->line_number--;
        current = current->next;
    }

    return 1; // 删除成功
}

// 获取指定行号的节点
ListNode* LinkedList::getNode(int line_number) const {
    if (line_number < 1 || line_number > size) {
        return nullptr;
    }

    ListNode* current;
    // 简单的优化：根据行号位置决定从头还是从尾遍历
    if (line_number <= size / 2) {
        current = head;
        while (current != nullptr) {
            if (current->line_number == line_number) {
                return current;
            }
            current = current->next;
        }
    } else {
        current = tail;
        while (current != nullptr) {
            if (current->line_number == line_number) {
                return current;
            }
            current = current->prev;
        }
    }
    return nullptr;
}

// 从头到尾遍历并打印
// 格式参考 PDF: [时间戳] 级别 模块 消息内容
// 注意：虽然 PDF 示例没显示行号，但作为调试或 FILTER 输出，原样打印通常指打印 LogEntry 内容
// 如果需要打印行号用于调试，可以自行修改 printf
void LinkedList::traverseForward() const {
    ListNode* current = head;
    while (current != nullptr) {
        // 根据 PDF 格式: [2025-01-15 10:32:45] ERROR Database Connection timeout
        // 注意：LogEntry 中的字符串通常已经是 null-terminated 的
        std::cout << "[" << current->data.max_time << "] "
                  << current->data.max_level << " "
                  << current->data.max_module << " "
                  << current->data.max_message << std::endl;
        
        // 或者使用 printf 保持格式控制更精准
        // printf("[%s] %s %s %s\n", current->data.max_time, current->data.max_level, 
        //        current->data.max_module, current->data.max_message);
               
        current = current->next;
    }
}

// 从尾到头遍历
void LinkedList::traverseBackward() const {
    ListNode* current = tail;
    while (current != nullptr) {
        std::cout << "[" << current->data.max_time << "] "
                  << current->data.max_level << " "
                  << current->data.max_module << " "
                  << current->data.max_message << std::endl;
        current = current->prev;
    }
}