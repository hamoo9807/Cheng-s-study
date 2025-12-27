#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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
TreeNode* insertBSTNode(TreeNode* root,int data)
{
	if (root == NULL)
	{
		return createNode(data);
	}
	if (root->data == data) return root;
	else if (root->data > data) root->left=insertBSTNode(root->left, data);
	else { 
		root->right = insertBSTNode(root->right, data);
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
	{
		return;
	}
	freeTree(root->left);
	freeTree(root->right);
	free(root);
}
TreeNode* findNode(TreeNode* root,int data)
{
	if (root == NULL || root->data == data)
	{
		return root;
	}
	if (data < root->data) return findNode(root->left, data);
	else {
		return findNode(root->right, data);
	}
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
TreeNode* DeleteNode(TreeNode* root, int data)
{
	if (root == NULL) return NULL;
	if (data < root->data)
	{
		root->left = DeleteNode(root->left, data);
	}
	else if (data > root->data)
	{
		root->right = DeleteNode(root->right, data);
	}
	else
	{
		if (root->left == NULL)
		{
			TreeNode* temp = root->right;
			free(root);
			return temp;
		}
		else if (root->right == NULL)
		{
			TreeNode* temp = root->left;
			free(root);
			return temp;
		}
		TreeNode* temp = findMinNode(root->right);
		root->data = temp->data;
		root->right = DeleteNode(root->right, temp->data);
	}
	return root;
}
int main()
{
	TreeNode* root = createNode(50);
	srand((unsigned int)time(NULL));
	for (int i = 0; i < 1000; i++)
	{
		insertBSTNode(root, rand() % 1000);
	}

	printf("intratevl:\n");
	InTravel(root);

	freeTree(root);
	return 0;
}