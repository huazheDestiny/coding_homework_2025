#ifndef UNDOSTACK_H
#define UNDOSTACK_H

#include "LinkedList.h"

#define MAX_UNDO_STACK_SIZE 5  //最多可以进行五次撤回操作

struct Undo{
    LogEntry deleted_entry; //被删除的日志条目
    int original_line_number; //被删除时它所在的行号
};

class UndoStack   //撤回栈类，使用动态数组
{
private:
    Undo* stack;
    int top;
    int capacity;  //栈容量

public:
    UndoStack(int size = MAX_UNDO_STACK_SIZE);
    ~UndoStack();
    
    //查询操作
    bool isEmpty() const {return top == -1;};
    
    bool isFull() const  {
        if (top >= capacity - 1)
    {
        return true;
    }
        return false;
    };

    void clear() {top = -1;};

    //出入栈
    bool  push(const LogEntry& entry, int line_number);
    bool  pop(Undo& action);
};



#endif