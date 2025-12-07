//双向链表的头文件
#ifndef LINKEDLIST_H

#define LINKEDLIST_H

#define MAX_TIME 20
#define MAX_LEVEL 10
#define MAX_MODULE 20
#define MAX_MASSAGE 200
#define MAX_LINE_LENGTH 300

struct LogEntry{   //每个日志消息的条目
    char max_time[MAX_TIME];
    char max_level[MAX_LEVEL];
    char max_module[MAX_MODULE];
    char max_message[MAX_MASSAGE];
    char max_line[MAX_LINE_LENGTH];  //日志消息每一行的最大长度
};

struct ListNode{
    ListNode* next;
    ListNode* prev;
    LogEntry data;
    int line_number;

    //双向链表节点的构造函数
    ListNode(const LogEntry& entry, int number);
};

class LinkedList
{
private:
    ListNode* head;
    ListNode* tail;
    int size; //双向链表大小  
public:
    LinkedList();
    ~LinkedList();
    int insertHead(const LogEntry& entry);  //头插
    int insertTail(const LogEntry& entry);  //尾插
    int deleteAt(int line_number); //删除指定行号
    void clear(); //清空链表

    //查询操作
    ListNode* getHead() const {return head;};//已实现
    ListNode* getTail() const {return tail;};//已实现
    // bool isFull() const;
    bool isEmpty() const {return size == 0;};//已实现
    ListNode* getNode(int linenumber) const;
    int getSize() const {return size;};//已实现

    //遍历操作
    void traverseForward() const;  //从头到尾遍历
    void traverseBackward() const; //从尾到头遍历

    //深拷贝函数，用于撤销
    void copyFrom(const LinkedList& other); 
};

#endif