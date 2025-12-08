//二叉搜索树功能的实现文件
#include "BST.h"
#include <iostream>
#include <cstring>
#include <algorithm>

#define MAX_STATS_BUFFER 50000  //分配一个巨大的缓冲区确保STATS命令不会失效

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

//迭代方法删除整个树
void BST::destroyTree(BSTNode* node){
    if(node == nullptr) return;

    BSTNode** stack = new BSTNode*[MAX_STATS_BUFFER]; //BST私有栈，避免使用静态数组
    int top = -1;

    stack[++top] = node;

    //根-右-左删除
    while (top != -1)
    {
        BSTNode* current = stack[top--];
        
        if (current->left) {
            stack[++top] = current->left;
        }
        if (current->right) {
            stack[++top] = current->right;
        }
        // 删除节点
        delete current;
    }
    delete[] stack;
}

//找到最小值的节点，在二叉搜索树中排在最左边
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


//辅助函数，按字典序排序
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
        node -> left = updateCountHelper(node -> left , module  ,count_num);
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

//辅助计数函数
void BST::updateCount(const char* module,int count_num){
    if (module == nullptr || module[0] == '\0') 
    {
        return;
    }
    root = updateCountHelper(root,module,count_num);
}

//利用保留站控制状态
void BST::collectStats(BSTNode* node,StatsEntry results[],int& index) const {
    if (node == nullptr)
    {
        return;
    }

    BSTNode** stack = new BSTNode*[MAX_STATS_BUFFER]; 
    int top = -1; // 栈顶指针，-1 表示空栈
    
    BSTNode* current = node;

    while (current != nullptr || top != -1) {
        while (current != nullptr) {
            if (top >= MAX_STATS_BUFFER - 1) { 
                return;
            }
            stack[++top] = current;
            current = current->left;
        }

        // 当前中序遍历要处理的节点，通过栈顶出私有栈
        if (top != -1) {
            current = stack[top--];
            
            // 中序操作收集统计数据
            if (current->error_count > 0 && index < MAX_STATS_BUFFER) {
                strncpy(results[index].module_name, current->module_name, MAX_MODULE - 1);
                results[index].module_name[MAX_MODULE - 1] = '\0';
                results[index].count = current->error_count;
                index++;
            }
            current = current->right;
        }
    }
    delete[] stack;
}

//比较状态并按字典序排序
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

//获取统计信息，STATS输出
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

//深拷贝函数接口和其辅助函数
void BST::copyFrom(const BST& other) {
    this->clear(); 
    if (other.root != nullptr) {
        this->root = copyTreeHelper(other.root);
    }
}

BSTNode* BST::copyTreeHelper(BSTNode* node) {
    if (node == nullptr) return nullptr;
    
    // 创建新节点，复制数据
    BSTNode* newNode = new BSTNode(node->module_name, node->error_count);
    
    // 递归复制左右子树，因为撤回操作不会太频繁，递归不会导致栈溢出
    newNode->left = copyTreeHelper(node->left);
    newNode->right = copyTreeHelper(node->right);
    
    return newNode;
}