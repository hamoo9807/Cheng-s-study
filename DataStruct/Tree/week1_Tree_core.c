#include <stdio.h>
#include <stdlib.h>
//闭卷手写的普通二叉树core代码
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