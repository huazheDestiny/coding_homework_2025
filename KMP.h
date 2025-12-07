#ifndef KMP_H
#define KMP_H

#include "LinkedList.h"

class KMP {
private:
    //计算next数组
    static void computeNext(const char* pattern , int* next,int len);
    //kmp匹配函数
    static bool kmpMatch(const char* text,const char* pattern,const int* next,int patlen);

public:
    //搜索每一条日志是否包含关键词
    static void searchLogs(const LinkedList& list,const char* keyword);
};

#endif