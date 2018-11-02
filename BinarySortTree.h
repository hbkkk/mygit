/**************************************二叉查找树c头文件**************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EQ(a, b) ((a) == (b))
#define LT(a, b) ((a) < (b))
#define LQ(a, b) ((a) <= (b))

typedef int bool;
#define true 1
#define false 0

typedef int KeyType;

typedef struct BSTNode{  //二叉查找树结点
	KeyType key;      //关键字(键值)
	struct BSTNode *lchild;
	struct BSTNode *rchild;
	struct BSTNode *parent;
}BSTNode, *BSTree;


void PreOrder_bstree(BSTree T);  //前序遍历二叉查找树
void InOrder_bstree(BSTree T);  //中序遍历二叉查找树
void PostOrder_bstree(BSTree T);  //后序遍历二叉查找树

bool SearchBST(BSTree T, KeyType key, BSTree f, BSTree *p);  //(递归实现)查找"二叉树T"中键值为key的节点
BSTree SearchBST_iterative(BSTree T, KeyType key); //(非递归实现)查找"二叉树T"中键值为key的节点

BSTree Minimum_BSTree(BSTree T);  //查找最小结点：返回tree为根结点的二叉树的最小结点
BSTree Maximum_BSTree(BSTree T);  //查找最大结点：返回tree为根结点的二叉树的最大结点

bool InsertBST(BSTree *T, KeyType e); //插入元素e到二叉查找树中

BSTree BSTree_Successor(BSTree T);   //找结点T的后继结点，即查找“二叉树中数据大于该结点”的“最小结点”
BSTree BSTree_Predecessor(BSTree T); //找结点T的前驱结点，即查找“二叉树中数据小于该结点”的“最大结点”

bool DeleteBST(BSTree *T, KeyType e); //从二叉树中删除关键字为e的结点

void Print_BST(BSTree T, KeyType key, int direction); //打印二叉树

void Destroy_BST(BSTree T); //销毁二叉树








