//循环队列的实现文件
#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>

#include "CircularQueue.h"

CircularQueue::CircularQueue() { //构造函数的实现
    front = 0;
    rear = 0;
    current_size = 0;
    for (int i = 0; i < MAX_QUEUE_SIZE; ++i)
    {
        data[i] = nullptr;
    }
}

CircularQueue::~CircularQueue() { //析构函数的实现
    clear();
}

void CircularQueue::clear() { //清空队列的实现
    front = 0;
    rear = 0;
    current_size = 0;
    for (int i = 0; i < MAX_QUEUE_SIZE; i++)
    {
        data[i] = nullptr;
    }
}

int CircularQueue::enqueue(ListNode* node) { //入队操作的实现，失败返回0成功返回1
    if(node == nullptr) {return 0;}; //空指针不入队
    
    data[rear] = node;

    if (isFull()){
        front = (front + 1) % MAX_QUEUE_SIZE; //覆盖最早的元素
    } else {
        current_size++;
    }
    rear = (rear + 1) % MAX_QUEUE_SIZE;
    return 1;
}

ListNode* CircularQueue::dequeue() { //出队操作的实现，队空返回nullptr
    if (isEmpty()){
        return nullptr;
    }
    ListNode* dequeued_data = data[front];
    data[front] = nullptr; 
    front = (front + 1) % MAX_QUEUE_SIZE;
    current_size--;
    return dequeued_data;
}

void CircularQueue::traverseQueue(int n) const { //从队头到队尾遍历队列的实现，打印前n个元素
    if (isEmpty()){
        return;
    }

    int count = std::min(n, current_size);
    int start_index = (front + current_size - count) % MAX_QUEUE_SIZE;
    for (int i = 0; i < count; i++)
    {
        int index = (start_index + i) % MAX_QUEUE_SIZE;
        ListNode* node = data[index];
        if (node != nullptr){
            std::cout << "[" << node->data.max_time << "] "
                      << node->data.max_level << " "
                      << node->data.max_module << " "
                      << node->data.max_message << std::endl;
        }
    }
}

int CircularQueue::removeAt(ListNode* target) { //删除指定位置的元素
    if (isEmpty() || target == nullptr) {
        return 0;
    }

    bool found = false;
    for (int i = 0; i < current_size; ++i) {
        int index = (front + i) % MAX_QUEUE_SIZE;
        
        if (data[index] == target) {
            // 找到匹配的指针，将其设为 nullptr 逻辑移除
            data[index] = nullptr;
            found = true;
            break; 
        }
    }
    return found ? 1 : 0;
}

void CircularQueue::copyFrom(const CircularQueue& other, const LinkedList& newList) {
    this->clear();
    
    this->front = other.front;
    this->rear = other.rear;
    this->current_size = other.current_size;

    // 复制 data 数组
    for (int i = 0; i < MAX_QUEUE_SIZE; ++i) {
        ListNode* oldNode = other.data[i];
        if (oldNode != nullptr) {
            this->data[i] = newList.getNode(oldNode->line_number);
        } else {
            this->data[i] = nullptr;
        }
    }
}