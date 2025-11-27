//二叉搜索树（BST）的头文件，统计ERROR级别，支持插入、查找、中序遍历
#ifndef BST_H
#define BST_H

#include "LinkedList.h"

//BST的节点
struct BSTNode
{
    char module_name[MAX_MODULE];   //模块名
    int error_count; //错误数目
    BSTNode* left;//根节点指向左右孩子的指针
    BSTNode* Right;

    BSTNode(const char* module,int count = 1);
};

//用于STATS的命令，暂时不知道需不需要保留
struct StatsEntry{
    char module_name[MAX_MODULE];
    int count;
};

class BST
{
private:
    BSTNode* root;

    void destroyTree(BSTNode* node);
    BSTNode* updateCountHelper(BSTNode* node,const char* module,int count_num); // 处理updateCount的递归辅助函数
    void collectStats(BSTNode* node,StatsEntry stats_array[],int& index) const; //中序遍历收集统计信息
public:
    //构造函数和析构函数
    BST();
    ~BST();
    void clear(); //清空树

    void updateCount(const char* module,int count_num = 1); //更新指定模块的错误计数,LOAD/UNDO会增加计数，DELETE会减少计数

    void getStats(StatsEntry stats_array[],int& size) const; //用于实现stats命令
};

bool compareStats(const StatsEntry& a,const StatsEntry& b); //用于排序统计结果的比较函数

#endif