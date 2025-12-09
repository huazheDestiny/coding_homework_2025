#include "UndoStack.h"
#include <cstring>

// 构造函数
UndoStack::UndoStack(int size) : top(-1), capacity(size) {
    stack = new Undo[size];
}

// 析构函数
UndoStack::~UndoStack() {
    delete[] stack;
}

//压栈记录删除操作
bool UndoStack::push(const LogEntry& entry, int line_number){
    if (isFull())
    {
        return 0;
    }
    top ++;
    stack[top].deleted_entry = entry;
    stack[top].original_line_number = line_number;

    return 1;
}

//出栈撤销删除操作
bool UndoStack::pop(Undo& action){
    if (isEmpty())
    {
        return 0;
    }
    action = stack[top];
    top--;

    return 1;
}