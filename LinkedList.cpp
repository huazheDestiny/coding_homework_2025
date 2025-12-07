//双向链表的实现文件
#include "LinkedList.h"
#include <iostream>
#include <cstring>
#include <cstdio>

// ListNode 结构体

ListNode::ListNode(const LogEntry& entry, int number) {
    this->data = entry; 
    this->line_number = number;
    this->next = nullptr;
    this->prev = nullptr;
}

// LinkedList 类实现

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
int LinkedList::insertHead(const LogEntry& entry) {
    ListNode* newNode = new ListNode(entry, 1); // 头节点行号是 1

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
int LinkedList::deleteAt(int line_number) {
    if (line_number < 1 || line_number > size) {
        return 0; // 删除失败，行号无效
    }

    // 判断是从头找还是从尾找
    ListNode* target = nullptr;
    if (line_number < size / 2) {
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

    if (target == nullptr) return 0;

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
        if (head != nullptr) head->prev = nullptr;
    } else if (target == tail) {
        // 删除尾节点
        tail = target->prev;
        if (tail != nullptr) tail->next = nullptr;
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
void LinkedList::traverseForward() const {
    ListNode* current = head;
    while (current != nullptr) { //每一条日志消息都是‘\0’结尾
        std::cout << "[" << current->data.max_time << "] "
                  << current->data.max_level << " "
                  << current->data.max_module << " "
                  << current->data.max_message << std::endl;   
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

//深拷贝函数
void LinkedList::copyFrom(const LinkedList& other) {
    this->clear(); // 先清空自己

    ListNode* current = other.getHead();
    while (current != nullptr) {
        this->insertTail(current->data); // 使用尾插法插入节点
        current = current->next;
    }
}