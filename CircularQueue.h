//循环队列的头文件
#ifndef CIRCULARQUEUE_H

#define CIRCULARQUEUE_H

#include "LinkedList.h"

#define MAX_QUEUE_SIZE 1000  //定义循环队列的最大容量为1000

class CircularQueue {
private:
    ListNode* data[MAX_QUEUE_SIZE]; // 队列存储指向双向链表的指针
    int front;//队头
    int rear; //队尾
    int current_size;//队列中当前元素个数

public:
    //构造函数和析构函数
    CircularQueue();
    ~CircularQueue();
    
    //查询操作
    bool isFull() const {return (current_size == MAX_QUEUE_SIZE);};
    bool isEmpty() const {return (current_size == 0);};
    int size() const {return current_size;};
    
    //清空队列
    void clear();

    //入队和出队操作
    int enqueue(ListNode* node); //入队
    ListNode* dequeue(); //出队

    //从队头到队尾遍历
    void traverseQueue(int n) const;

    //删除操作，用于delete命令
    int removeAt(ListNode* target);
};

#endif