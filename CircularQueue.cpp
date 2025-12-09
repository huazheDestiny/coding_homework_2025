//循环队列的实现文件
#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>

#include "CircularQueue.h"

//含有最大容量的构造函数
CircularQueue::CircularQueue(int cap) {
    front = 0;
    rear = 0;
    current_size = 0;
    capacity = cap;
    data = new ListNode*[capacity];
    for (int i = 0; i < capacity; i++)
    {
        data[i] = nullptr;
    }
}
//析构函数
CircularQueue::~CircularQueue() {
    clear();
    delete[] data;
}

//清空队列
void CircularQueue::clear() {
    front = 0;
    rear = 0;
    current_size = 0;
    for (int i = 0; i < capacity; i++)
    {
        data[i] = nullptr;
    }
}

//入队
int CircularQueue::enqueue(ListNode* node) {
    if(node == nullptr) {return 0;}; //空指针不入队
    
    data[rear] = node;

    if (isFull()){
        front = (front + 1) % capacity; //覆盖最早的元素
    } else {
        current_size++;
    }
    rear = (rear + 1) % capacity;
    return 1;
}

//出队
ListNode* CircularQueue::dequeue() {
    if (isEmpty()){
        return nullptr;
    }
    ListNode* dequeued_data = data[front];
    data[front] = nullptr; 
    front = (front + 1) % capacity;
    current_size--;
    return dequeued_data;
}

//从队头到队尾遍历，打印前n个元素
void CircularQueue::traverseQueue(int n) const {
    if (isEmpty()){
        return;
    }

    int count = std::min(n, current_size);
    int start_j = (front + current_size - count) % capacity;
    for (int i = 0; i < count; i++)
    {
        int j = (start_j + i) % capacity;
        ListNode* node = data[j];
        if (node != nullptr){
            std::cout << "[" << node->data.max_time << "] "
                      << node->data.max_level << " "
                      << node->data.max_module << " "
                      << node->data.max_message << std::endl;
        }
    }
}

//删除指定位置的元素
int CircularQueue::removeLine(ListNode* target) {
    if (isEmpty() || target == nullptr) {
        return 0;
    }

    bool found = false;
    for (int i = 0; i < current_size; ++i) {
        int j = (front + i) % capacity;
        
        if (data[j] == target) {
            data[j] = nullptr;
            found = true;
            break; 
        }
    }
    return found ? 1 : 0;
}

//深拷贝函数
void CircularQueue::deepcopy(const CircularQueue& other, const LinkedList& newList) {
    delete[] this->data; //释放原有内存
    //this->clear();
    this->capacity = other.capacity;
    this->data = new ListNode*[this->capacity];
    this->front = other.front;
    this->rear = other.rear;
    this->current_size = other.current_size;


    for (int i = 0; i < capacity; ++i) {
        ListNode* oldNode = other.data[i];
        if (oldNode != nullptr) {
            this->data[i] = newList.getNode(oldNode->line_number);
        } else {
            this->data[i] = nullptr;
        }
    }
}