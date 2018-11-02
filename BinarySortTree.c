/**************************************二叉查找树c源文件**************************************/
//
//总结1：用函数InsertBST来创建一个新的二叉树时，这棵树的构造随输入的关键字顺序有关，第一个输入的关键字会成为树的根结点，但不管怎么以什么顺序输入，最后中序遍历
//	这棵二叉树时得到的结果都是一样的，从小到大！！！！
//
//总结2:求某结点的后继的三种情况————1)如果结点T存在右孩子，则"T的后继结点"为 "以其右孩子为根的子树的最小结点"。
//       		       如果T没有右孩子。则T有以下两种可能：
//		               2)结点T是"一个左孩子"，则"T的后继结点"为 "它的父结点"。
//			*******3)结点T是"一个右孩子"，则查找"T的最低的父结点，并且结点T是包含在该父节点的左子树内，而不是包含在右子树内"，第一个满足这个条件的父节点即为结点T的后继
//
//总结3：求某结点的前驱的三种情况————1)如果结点x存在左孩子，则"x的前驱结点"为 "以其左孩子为根的子树的最大结点"。
//       		       如果T没有左孩子。则T有以下两种可能：
//		               2) x是"一个右孩子"，则"x的前驱结点"为 "它的父结点"。
//			*******3)结点x是一个左孩子，则结点x的最低父节点，且结点x包含在该父节点的右子树内，而不是包含在左子树内，那么第一个满足该条件的父节点就是结点T的前驱结点
//
//总结4*****：在DeleteBST函数中，每种情况处理好后，在每种情况处理语句最后直接return，因为不可能出现同时满足3种情况中的2种情况的可能。 
//	     这是因为如果某种情况处理好后意味着要删除的结点p已经释放并赋值为NULL了，如果继续进行下面的判断，对已经是NULL的指针p进行任何取值的操作都会导致Segmentation fault
//
//总结5：删除结点的三种情况。这里假设要删除的结点为*p，其双亲结点为*f，可设*p是*f的左孩子
// 	1)若*p为叶子结点，则只需修改其双亲结点的指针即可
// 	2)若*p结点只有左子树PL或只有右子树PR，只要令PL或PR直接成为其双亲结点*f的左子树即可
// 	3)若*p结点的左子树、右子树均不空，此时有两种做法。
//		法一：令*p的左子树为*f的左子树，而*p的右子树为*s的右子树（这里的*s是结点*p的前驱结点）
//		法二：令*p的直接前驱(或直接后继)替代*p，然后再从二叉排序树中删去*p的直接前驱(或直接后继)，这里删除*p的直接前驱(或直接后继)的操作与上述讨论的删除结点一致，按情况处理
//
//总结6：在找最大最小值的函数里循环的条件必须是“T->lchild!=NULL” 即while(T->lchild != NULL) T = T->lchild; , 不能是“T != NULL”, 因为最后返回的是T，
//	这样的话最后返回的T永远是NULL了，之后再在这个返回值(即NULL)上操作进行取值之类的肯定报错！！！！


#include "BinarySortTree.h"

static BSTree Parent = NULL; //用于求解某结点的前序、后继结点时使用。如果在那两个函数中定义的话，就成了局部变量，返回时可能会有问题！

void PreOrder_bstree(BSTree T)  //前序遍历二叉查找树
{
	if(T)
	{
		printf("%d ", T->key);
		PreOrder_bstree(T->lchild);
		PreOrder_bstree(T->rchild);
	}
}

void InOrder_bstree(BSTree T)  //中序遍历二叉查找树
{
	if(T)
	{
		InOrder_bstree(T->lchild);
		printf("%d ", T->key);
		InOrder_bstree(T->rchild);
	}
}

void PostOrder_bstree(BSTree T)  //后序遍历二叉查找树
{
	if(T)
	{
		PostOrder_bstree(T->lchild);
		PostOrder_bstree(T->rchild);
		printf("%d ", T->key);
	}
}


/**********(非递归实现)查找"二叉树T"中键值为key的节点**********/
//功能：非递归搜索关键字key
//参数： T：开始搜索的那个树节点， key：要搜索的关键字
//返回值：若找到关键字，则返回那个结点；没找到的话返回NULL
BSTree SearchBST_iterative(BSTree T, KeyType key)
{
	while(T != NULL && !EQ(key, T->key))
//	while(T != NULL && (T->key != key))
	{
		if(LT(key, T->key))
		{
			T = T->lchild;
		}
		else
			T = T->rchild;
	}
//	printf("In search iterative, key:%d\n", T->key);
	return T;
}


/**********(递归实现)查找"二叉树T"中键值为key的节点**********/
//参数：
//	T:传入的二叉查找树根节点   key:欲查找的关键字(即树结点)   f:指向匹配的结点的双亲，未找到时指向查找路径上访问的最后一个结点
//	p:查找成功时，p指向该数据元素结点，并返回true；查找失败时，p指向查找路径上访问的最后一个结点并返回False
//返回值： 找到的话返回true，p指向那个找到的结点；未找到返回false(此时p就是指向双亲，插入结点时根据大小插到p的左或右孩子上)
//******注：这里的p是二级指针，因为要改变指针p的指向的地址，而不是单单改变p指向地址的内容
bool SearchBST(BSTree T, KeyType key, BSTree f, BSTree *p)
{
	if(!T)
	{
		*p = f;  //********************这句话就是在未找到时返回双亲！！！*************************
		return false;
	}
	if(EQ(key, T->key))
	{
		*p = T;  //********注：要改变传进来的指针指向的地址就要用二级，然后对*f赋新的地址！！！！！
		return true;
	}
	else if(LT(key,T->key))  //关键字小于该结点
	{
		return SearchBST(T->lchild, key, T, p);
	}
	else //关键字大于该结点
	{
		return SearchBST(T->rchild, key, T, p);
	}
}


/***************************插入关键字到二叉查找树中****************************/
//功能：当二叉查找树T中不存在关键字等于e的数据元素时，插入e并返回true，否则返回false
//参数：T是二叉查找树， e是待插入关键字
//返回值：当二叉树中不存在关键字e，则插入成功时返回true；若二叉树中存在关键字e，则直接返回false
//
bool InsertBST(BSTree *T, KeyType e)
{
	BSTree p = NULL; //若可插入，则插入在结点p的左孩子或右孩子上
	if(!SearchBST(*T, e, NULL, &p))
	{
		BSTree s = (BSTree)malloc(sizeof(BSTNode));
		s->key = e;
		s->lchild = NULL;
		s->rchild = NULL;
		s->parent = p;  //*********这句话,就是在新建二叉树的时候设置好parent，就算是插入的是第一个结点(即根结点)，这里的p也是成立的，因为此时p为NULL

		if(NULL == p)  //二叉树为空，即被插入结点*s为根结点
			*T = s;
		else if(LT(e, p->key))
		{
			p->lchild = s;  //被插入结点*s为左孩子
		}
		else
		{
			p->rchild = s;  //被插入结点*s为右孩子
		}

		return true;
	}
	else
	{
		return false;
	}
}

/************查找最小结点************/
//返回值：以T为根结点的二叉树的最小结点
BSTree Minimum_BSTree(BSTree T)
{
	if(T == NULL)
	{
		printf("In Minimum_BSTree, T is NULL\n");
		return NULL;
	}
//	while(NULL != T)  //这句话出的错，这样最后返回的T是NULL啊！！！
	while(T->lchild != NULL)
	{
		T = T->lchild;
	}
//	printf("In minimum, key:%d\n", T->key);
	return T;
}

/************查找最大结点************/
//返回值：以T为根结点的二叉树的最大结点
BSTree Maximum_BSTree(BSTree T)
{
	while(NULL != T->rchild)
	{
		T = T->rchild;
	}
	return T;
}


//注：这里所谓的后继、前驱，指的是在中序遍历二叉树后的结点顺序中的某结点的前、后结点分别是该结点的前驱、后继结点

/****************找结点T的后继结点****************/
//功能:查找“二叉树中数据大于该结点”的“最小结点”
//参数：开始找的那个结点
//返回值：找到的结点T的后继节点
BSTree BSTree_Successor(BSTree T)  //采用静态全局变量作为返回值
{
	if(NULL == T)
		return NULL;

	// 如果T存在右孩子，则"x的后继结点"为 "以其右孩子为根的子树的最小结点"。    可以这么理解：后继就是中序遍历时的，中序遍历是：左子树→根节点→右子树，所以如果结点T有右孩子，
	if(T->rchild != NULL)							  //那后继肯定就在T的右子树里，并且是右子树中左下角那个！
	{
//		return Minimum_BSTree(T->rchild);
		BSTree p = Minimum_BSTree(T->rchild);
		return p;
	}

	// 如果T没有右孩子。则T有以下两种可能：
	// 1) T是“一个左孩子”，则“T的后继结点”为“它的父结点”。  中序遍历是：左子树→根节点→右子树
	// 2) T是“一个右孩子”，则查找"T的最低(最下面)的父结点，并且该父结点要具有左孩子(**********！！！重点:这里的“具有左孩子”指的是结点T是在这个父节点的左子树里的而不是右子树里的)"，
	//    找到的这个第一个满足以上条件的结点就是"x的后继结点。

//	BSTree p = T->parent;  //如果返回这个p的话会有问题么？？？它是局部变量啊
	Parent = T->parent;
	while(Parent != NULL && Parent->rchild == T)  //若T是左孩子，那么这个循环一次都不会进入
	{
		T = Parent;
		Parent = Parent->parent;
	}
	return Parent;

//以下是测试局部变量p是否可行
//结论：是可行的，因为局部指针变量返回时是返回副本，只要不返回局部指针变量本身的地址就没问题
/*	BSTree p = T->parent;
	while(p != NULL && p->rchild == T)
	{
		T = p;
		p = p->parent;
	}
	return p;*/
}

/****************找结点T的前驱结点****************/
//功能:查找“二叉树中数据小于该结点”的“最大结点”
//参数：开始找的那个结点
//返回值：找到的结点T的前驱节点
BSTree BSTree_Predecessor(BSTree T)   //采用局部指针变量作为返回值(对比求后继的函数)
{
	if(NULL == T)
		return NULL;

	//如果结点T有左孩子，那么结点T的左子树内，值最大的结点就是结点T的前驱结点
	if(T->lchild != NULL)  //中序遍历:左子树→根节点→右子树
	{
		return Maximum_BSTree(T->lchild);
	}

	//如果结点T没有左孩子，分以下两种情况：
	//1)结点T是一个右孩子，则结点T的父节点就是结点T的前驱
	//2)结点T是一个左孩子，则结点T的最低父节点，且结点T包含在该父节点的右子树内，而不是包含在左子树内，那么第一个满足该条件的父节点就是结点T的前驱结点
	BSTree p = T->parent;
	while(NULL != p && p->lchild == T)
	{
		T = p;
		p = p->parent;
	}
	return p;
}


/*****************删除结点****************/
//功能：从二叉树中删除关键字为e的结点
//参数： T:操作的二叉树  e:要删除的结点的关键字
//返回值：删除成功返回true,失败返回false
//
// 分析：删除结点分为三种情况讨论。这里假设要删除的结点为*p，其双亲结点为*f，可设*p是*f的左孩子
//
// 1)若*p为叶子结点，则只需修改其双亲结点的指针即可
// 2)若*p结点只有左子树PL或只有右子树PR，只要令PL或PR直接成为其双亲结点*f的左子树即可
// 3)若*p结点的左子树、右子树均不空，此时有两种做法。
//	法一：令*p的左子树为*f的左子树，而*p的右子树为*s的右子树（这里的*s是结点*p的前驱结点）
//	法二：令*p的直接前驱(或直接后继)替代*p，然后再从二叉排序树中删去*p的直接前驱(或直接后继)，这里删除*p的直接前驱(或直接后继)的操作与上述讨论的删除结点一致，按情况处理
bool DeleteBST(BSTree *T, KeyType e)
{
	KeyType key_tmp;
	BSTree p = NULL;
	BSTree tmp = NULL;
	BSTree tmp2 = NULL;
	if(!SearchBST(*T, e, NULL, &p))
	{
		printf("No key %d, delete error\n", e);
		return false;
	}

	//情况1：*p为叶子结点，则只需修改其双亲结点的指针即可
	if(p->rchild == NULL && p->lchild == NULL)
	{
		//printf("case 1，欲删除结点是叶子结点\n");
		if(p->parent == NULL) //这种情况是整棵树就只有一个根节点
		{
			printf("1.1，欲删除结点%d是叶子结点，也是唯一的根结点\n", e);
			free(p);
			p = NULL;
			*T = NULL;
			printf("\n树中所有结点均已删除");
			return true;
		}
		if(p == p->parent->rchild)
		{
			p->parent->rchild = NULL;
			printf("1.2，欲删除结点%d是叶子结点，且是右孩子\n", e);
		}
		else if(p == p->parent->lchild)
		{
			p->parent->lchild = NULL;
			printf("1.3，欲删除结点%d是叶子结点，且是左孩子\n", e);
		}
		free(p);
		p = NULL;
		printf("end of case 1\n");

		return true;
	}

	//情况2：*p结点只有左子树PL或只有右子树PR，只要令PL或PR直接成为其双亲结点*f的左子树即可
	if(p->rchild != NULL && p->lchild == NULL) //*p结点只有右子树
	{
		printf("case 2.1，欲删除结点%d只有右子树\n", e);
		if(p->parent == NULL)  //要删除的结点为根结点的情况，改变根节点指向
		{
			//先让一个临时指针指向根节点的右孩子，然后释放根节点，之后再让根节点指向前面的临时指针指向的根节点的右孩子，这样就行了~
			tmp = p->rchild;  
			free(p);
			p = NULL;
			*T = tmp;
			(*T)->parent = NULL;

/*			p->rchild->parent = NULL;
			free(p);
			p = NULL; */
			return true;
		}
		if(p == p->parent->rchild)
		{
			p->parent->rchild = p->rchild;
			p->rchild->parent = p->parent;  //**************少了这句修改孩子双亲的语句！！！*************
		}
		if(p == p->parent->lchild)
		{
			p->parent->lchild = p->rchild;
			p->rchild->parent = p->parent;  //**************少了这句修改孩子双亲的语句！！！*************
		}
		free(p);
		p = NULL;

		return true;
	}
	if(p->lchild != NULL && p->rchild == NULL)  //*p结点只有左子树
	{
		printf("case 2.2，欲删除结点%d只有左子树\n", e);
		if(p->parent == NULL)  //要删除的结点为根结点的情况，改变根节点指向
		{
			tmp = p->lchild;  //这里应该是lchild而不是rchild，不然会出错，因为这里的p->rchild是NULL
			free(p);
			p = NULL;
			*T = tmp;
			(*T)->parent = NULL;

/*			p->lchild->parent = NULL;
			free(p);
			p = NULL; */
			return true;
		}
		if(p == p->parent->rchild)  //欲删除结点是右孩子
		{
			p->parent->rchild = p->lchild;  
			p->lchild->parent = p->parent;  //**************少了这句修改孩子双亲的语句！！！*************
		}
		if(p == p->parent->lchild)  //欲删除结点是左孩子
		{
			p->parent->lchild = p->lchild;
			p->lchild->parent = p->parent;  //**************少了这句修改孩子双亲的语句！！！*************
		}
		free(p);
		p = NULL;

		return true;
	}

	//情况3：*p结点的左子树、右子树均不空
	if(p->rchild != NULL && p->lchild != NULL)
	{
		if(p->parent == NULL)  //情况3.1：p为根节点的情况，让根节点的左子树成为根节点右子树中关键字值最小的那个结点的左子树
		{
			printf("case 3.1，欲删除结点%d有左、右子树，并且是根结点\n", e);
			tmp = p->rchild;  //用来临时保存新的根节点(原根节点的右子树根节点)
			tmp2 = Minimum_BSTree(p->rchild); //原先根节点的右子树中关键字的值最小的结点

			//下面这两个代码是把原先根节点的左、右子树连接起来
			tmp2->lchild = p->lchild;
			p->lchild->parent = tmp2;

			free(p);  //释放原先的根节点
			p = NULL;
			*T = tmp; //设置新的根节点
			(*T)->parent = NULL;

			return true;
		}

		if(p == p->parent->lchild)   //情况3.2：要删除的结点是父节点的左孩子.也就是说要删除的结点p的左、右子树均是p的父节点f的左子树中的一部分(即都比f结点的关键字小)
		{	printf("case 3.2，欲删除结点%d有左、右子树，且是父结点的左孩子\n", e);

			/************************************法一************************************/
			//令*p的左子树为p的父节点*f的左子树，而令*p的右子树为*s的右子树（这里的*s是结点*p的前驱结点）
			p->parent->lchild = p->lchild; //这两行就是令*p的左子树为p的父节点*f的左子树
			p->lchild->parent = p->parent;

			tmp = BSTree_Predecessor(p); //这里的tmp是欲删除结点p的前驱结点s
			if(NULL != tmp)  //下面这两行是令*p的右子树为*s的右子树 ，可以这么理解：p的右子树都比p大，也比p的前驱大，但是比p的父节点小。
			{
				tmp->rchild = p->rchild;
				p->rchild->parent = tmp;
			}
			else
			{
				printf("In delete node case 3.2, find predecessor error\n");
				return false;
			}
			free(p);
			p = NULL;
			return true;
			/****************************************************************************/


			/************************************法二(经验证，可行)************************************/
			//令*p的直接前驱(或直接后继)替代*p，然后再从二叉排序树中删去*p的直接前驱(或直接后继)，这里删除*p的直接前驱(或直接后继)的操作与上述讨论的删除结点一致，按情况处理
/*			tmp = BSTree_Predecessor(p);  //******注：欲删除结点p的前驱结点s没有右子树！且前驱结点一定是在p的左子树中(可自行证明)
			if(NULL != tmp)  //其实做法很简单！！！就是保存前驱结点s的key，然后删除前驱结点s，最后把欲删除结点p的key换作前驱结点s的key
			{
				key_tmp = tmp->key;
				if(!DeleteBST(T, tmp->key)) //删除p的前驱结点s
				{	
					printf("In delete node case 3.2, delete predecessor error\n");
					return false;
				}
				p->key = key_tmp;
				return true;
			}
			else
			{
				printf("In delete node case 3.2, find predecessor error\n");
				return false;
			}
*/			/****************************************************************************/
		}
		else if(p == p->parent->rchild)   //情况3.3：要删除的结点是父节点的右孩子. 也就是说要删除的结点p的左、右子树均是p的父节点f的右子树中的一部分(即都比f结点的关键字大)
		{	printf("case 3.3欲删除结点%d有左、右子树，且是父结点的右孩子3\n", e);

			/************************************法一************************************/
			//令*p的左子树为p的父节点*f的右子树，而令*p的右子树为*s的右子树(这里的*s是结点*p的前驱结点)
			p->parent->rchild = p->lchild; //这两行是令*p的左子树为p的父节点*f的右子树
			p->lchild->parent = p->parent;

			tmp = BSTree_Predecessor(p);
			if(NULL != tmp)
			{
				tmp->rchild = p->rchild;
				p->rchild->parent = tmp;
			}
			else
			{
				printf("In delete node case 3.3, find predecessor error\n");
				return false;
			}
			free(p);
			p = NULL;
			return true;
			/****************************************************************************/


			/************************************法二(经验证，可行)************************************/
			//令*p的直接前驱(或直接后继)替代*p，然后再从二叉排序树中删去*p的直接前驱(或直接后继)，这里删除*p的直接前驱(或直接后继)的操作与上述讨论的删除结点一致，按情况处理
/*			tmp = BSTree_Predecessor(p);  
			if(NULL != tmp)  //其实做法很简单！！！就是保存前驱结点s的key，然后删除前驱结点s，最后把欲删除结点p的key换作前驱结点s的key
			{
				key_tmp = tmp->key;
				if(!DeleteBST(T, tmp->key)) //删除p的前驱结点s
				{	
					printf("In delete node case 3.3, delete predecessor error\n");
					return false;
				}
				p->key = key_tmp;
				return true;
			}
			else
			{
				printf("In delete node case 3.3, find predecessor error\n");
				return false;
			}
*/			/****************************************************************************/		
		}
	}
}


/*******************打印二叉树*******************/
//功能：打印二叉树，可以表明父子结点关系
//参数： T:二叉树的结点   key:结点的键值    
//	direction: 0代表该结点是根结点、1代表该结点是它父结点的右孩子、-1代表该结点是它父结点的左孩子
void Print_BST(BSTree T, KeyType key, int direction)
{
	if(NULL != T)
	{
		if(direction == 0)
		{
			printf("%3d is root\n", T->key);
		}
		else
			printf("%3d is %2d's %6s child\n", T->key, key, (direction==1)?"right":"left");
		Print_BST(T->lchild, T->key, -1);
		Print_BST(T->rchild, T->key, 1);
	}
}

/**************销毁二叉树***************/
void Destroy_BST(BSTree T)
{
	if(NULL == T)
		return;
	if(NULL != T->rchild)
		Destroy_BST(T->rchild);
	if(NULL != T->lchild)
		Destroy_BST(T->lchild);

	free(T);
}


int main()  //测试通过插入节点这个函数来生成二叉查找树
{
	BSTree T = NULL;
	int a, n, key;
	printf("输入结点个数:\n");
	scanf("%d", &n);

	printf("输入结点关键字:\n");
	for(int i = 0; i < n; ++i)
	{
		scanf("%d", &a);
		if(!InsertBST(&T, a))
			printf("Insert error\n");
	}

//	printf("T's key:%d\n", T->key);

	printf("前序遍历：\n");
	PreOrder_bstree(T);
	printf("\n");

	printf("中序遍历：\n");
	InOrder_bstree(T);
	printf("\n");

	printf("后序遍历：\n");
	PostOrder_bstree(T);
	printf("\n\n");

	Print_BST(T,0,0);
	printf("\n");
	
	int num;
	BSTree tmp = NULL;
	BSTree suc = NULL;
	
	while(1)
	{
		printf("输入要搜索其后继的结点关键字:(输入-1结束)\n");
		scanf("%d", &num);
		if(num == -1)
			break;
		tmp = SearchBST_iterative(T, num);
		if(NULL == tmp)
		{
			printf("No key %d\n", num);
			continue;
		}
		printf("Search result:%d\n", tmp->key);
		suc = BSTree_Successor(tmp);
		if(suc == NULL)
			printf("key %d hasn't successor\n", num);
		else
			printf("key %d's successor:%d\n", num, suc->key);
	}

	while(1)
	{
		printf("输入要搜索其前驱的结点关键字:(输入-1结束)\n");
		scanf("%d", &num);
		if(num == -1)
			break;
		tmp = SearchBST_iterative(T, num);
		if(NULL == tmp)
		{
			printf("No key %d\n", num);
			continue;
		}
		printf("Search result:%d\n", tmp->key);
		suc = BSTree_Predecessor(tmp);
		if(suc == NULL)
			printf("key %d hasn't predecessor\n", num);
		else
			printf("key %d's predecessor:%d\n", num, suc->key);
	}

	int i = 0;  //用来记录删除的结点个数
	while(i < n)
	{
		printf("输入要删除结点的关键字:(输入-1结束)\n");
		scanf("%d", &key);
		if(key == -1)
			break;
		if(DeleteBST(&T, key))
		{
//			printf("中序遍历：\n");
//			InOrder_bstree(T);
			Print_BST(T,0,0);
			printf("\n");
			++i;
		}
		else
		{
			printf("delete error\n");
		}
	}

	return 0;
}










