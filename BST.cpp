//二叉搜索树功能的实现文件
#include "BST.h"
#include <iostream>
#include <cstring>
#include <algorithm>

BSTNode::BSTNode(const char* module,int count):error_count(count),left(nullptr),right(nullptr)
{
    strncpy(this -> module_name,module,MAX_MODULE - 1);
    this -> module_name[MAX_MODULE - 1] = '\0'; 
}

BST::BST():root(nullptr){

}

BST::~BST(){
    clear();
}

void BST::clear(){
    destroyTree(root);
    root = nullptr;
}

void BST::destroyTree(BSTNode* node){
    if(node){
        destroyTree(node -> left);
        destroyTree(node -> right);
        delete node;
    }
}


BSTNode* BST::findMin(BSTNode* node) const{
    if(node == nullptr){
        return nullptr;
    }
    while (node -> left != nullptr)
    {
        node = node -> left;
    }
    return node;
}





BSTNode* BST::updateCountHelper(BSTNode* node,const char* module,int count_num){
    if (node == nullptr)
    {
        if (count_num > 0)
        {
            return new BSTNode(module,count_num);
        }
        return nullptr;   
    }
    
    int cmp = strcmp(module,node -> module_name);
    
    if (cmp < 0){
        node -> left = updateCountHelper(node -> left,module,count_num);
    } else if (cmp > 0)
    {
        node -> right = updateCountHelper(node -> right,module,count_num);
    } else {
        node -> error_count += count_num;
        
        if (node -> error_count <= 0) //物理删除确保没有内存泄漏
        {
            if (node -> left == nullptr)
            {
                BSTNode* tmp = node -> right;
                delete node;
                return tmp;
            } else if (node -> right == nullptr)
            {
                BSTNode* tmp = node -> left;
                delete node;
                return tmp;
            }
        BSTNode* successor = findMin(node -> right);
            
        strncpy(node -> module_name,successor -> module_name,MAX_MODULE - 1);
        node -> module_name[MAX_MODULE - 1] = '\0';
        node -> error_count = successor -> error_count;
        
        node -> right = updateCountHelper(node -> right,successor -> module_name,0);

        BSTNode* tmp = findMin(node -> right);

        strncpy(node -> module_name,tmp -> module_name,MAX_MODULE - 1);
        node -> module_name[MAX_MODULE - 1] = '\0';
        node -> error_count = tmp -> error_count;

        node -> right = updateCountHelper(node -> right,tmp -> module_name,0 - tmp -> error_count);
        }
    }
    
    return node;
}

void BST::updateCount(const char* module,int count_num){
    if (module == nullptr || module[0] == '\0') 
    {
        return;
    }
    root = updateCountHelper(root,module,count_num);
}

void BST::collectStats(BSTNode* node,StatsEntry results[],int& index) const {
    if (node == nullptr)
    {
        return;
    }

    collectStats(node -> left,results,index);

    if(node -> error_count > 0){
        strncpy(results[index].module_name,node -> module_name,MAX_MODULE - 1);
        results[index].module_name[MAX_MODULE - 1] = '\0';
        results[index].count = node -> error_count;
        index++;
    }

    collectStats(node -> right,results,index);
}

bool compareStats(const StatsEntry& a,const StatsEntry& b){
    if (a.count != b.count)
    {
        return a.count > b.count; //错误计数降序排列
    }
    if (strcmp(a.module_name,b.module_name) < 0)
    {
        return true;
    }else{
        return false;
    }
}

void BST::getStats(StatsEntry stats_array[],int& size) const{
    size = 0;
    collectStats(root,stats_array,size);
    std::sort(stats_array,stats_array + size,compareStats);
    //打印结果
    for (int i = 0; i < size; i++)
    {
        std::cout << stats_array[i].module_name << ": " << stats_array[i].count <<" errors" << std::endl;
    }
}