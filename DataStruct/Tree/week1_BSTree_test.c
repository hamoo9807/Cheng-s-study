#include <stdio.h>
#include <stdlib.h>
//闭卷手写的BST二叉树core代码
typedef struct TreeNode {
	int data;
	struct TreeNode* left;
	struct TreeNode* right;
}TreeNode;
TreeNode* createNode(int data)
{
	TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
	if (newNode == NULL)
	{
		printf("No memory!\n");
		return NULL;
	}
	newNode->data = data;
	newNode->left = NULL;
	newNode->right = NULL;
	return newNode;
}
TreeNode* insertBST(TreeNode* root, int data)
{
	if (root == NULL)
	{
		return createNode(data);
	}
	if (data == root->data) return root;
	else if (data < root->data)
	{
		root->left = insertBST(root->left, data);
	}
	else
	{
		root->right = insertBST(root->right, data);
	}
	return root;
}
void PreTravel(TreeNode* root)
{
	if (root == NULL)
	{
		return;
	}
	printf("data:%d\n", root->data);
	PreTravel(root->left);
	PreTravel(root->right);
}
void InTravel(TreeNode* root)
{
	if (root == NULL)
	{
		return;
	}
	InTravel(root->left);
	printf("data:%d\n", root->data);
	InTravel(root->right);
}
void PostTravel(TreeNode* root)
{
	if (root == NULL)
	{
		return;
	}
	PostTravel(root->left);
	PostTravel(root->right);
	printf("data:%d\n", root->data);
}
void freeTree(TreeNode* root)
{
	if (root == NULL)
		return;
	freeTree(root->left);
	freeTree(root->right);
	free(root);	
}
TreeNode* findMinNode(TreeNode* root)
{
	if (root == NULL) return NULL;
	TreeNode* cur = root;
	while (cur->left != NULL)
	{
		cur = cur->left;
	}
	return cur;
}
TreeNode* deleteNode(TreeNode* root, int data)
{
	if (root == NULL) return NULL; //边界条件，回溯条件，防止传NULL树
	if (root->data > data)
	{
		root->left = deleteNode(root->left, data);//递归调用，调用分两层含义：一直往左侧寻路，直到寻到NULL回溯，并在回溯中连接
	}
	else if (root->data < data)
	{
		root->right = deleteNode(root->right, data);//同理，一直往右侧寻路，直到寻到NULL回溯，并在回溯中连接
	}
	else//已左右寻路定位到目标点，开始删除
	{
		if (root->left == NULL && root->right == NULL)//情况1：根节点，直接删除root并置空为NULL（父节点成为新叶子节点）
		{
			free(root);
			root = NULL;
		}
		else if (root->left == NULL && root->right != NULL)//情况2：只有一子树，进行链表操作：断链，删除，后重连
		{
			TreeNode* temp = root;
			root = root->right;
			free(temp);
		}
		else if (root->right == NULL && root->left != NULL)//情况2：同上。
		{
			TreeNode* temp = root;
			root = root->left;
			free(temp);
		}
		else//情况3：遇到无法处理的特异点，左右子树同时存在
		{
			TreeNode* temp = findMinNode(root->right);//核心思想：将右子树的最小值直接替换当前节点值
			root->data = temp->data;
			root->right = deleteNode(root->right, temp->data);//再次递归调用，实现删除替换掉值的节点，并在回溯中重连
		}
	}
	return root;//完成单次寻路（碰壁NULL或找到目标点），回溯并返回指针
}
int main()
{
	TreeNode* newNode = createNode(2);
	newNode->left = createNode(3);
	newNode->right = createNode(10);
	newNode->left->left = createNode(11);
	printf("pretravel:\n");
	PreTravel(newNode);
	printf("inratevl:\n");
	InTravel(newNode);
	printf("posttravel:\n");
	PostTravel(newNode);
	return 0;
}