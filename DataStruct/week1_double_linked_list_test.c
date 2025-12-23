#include <stdio.h>
#include <stdlib.h>
// 第一周 双向链表 - 闭卷手写版本（核心逻辑）
// 练习目标：闭卷快速实现双向链表的增删查改核心功能
typedef struct DoubleNode
{
	int data;
	struct DoubleNode* prev;
	struct DoubleNode* next;
}DoubleNode;
typedef struct DoubleList
{
	DoubleNode* head;
	DoubleNode* tail;
	int size;
}DoubleList;
DoubleNode* createNode(int data)
{
	DoubleNode* newNode = (DoubleNode*)malloc(sizeof(DoubleNode));
	if (newNode == NULL)
	{
		printf("error!failed to create node");
		return NULL;
	}
	newNode->data = data;
	newNode->next = NULL;
	newNode->prev = NULL;
	return newNode;
}
DoubleList* initList()
{
	DoubleList* newList = (DoubleList*)malloc(sizeof(DoubleList));
	if (newList == NULL)
	{
		printf("error!failed to create newList");
		return NULL;
	}
	newList->head = NULL;
	newList->size = 0;
	newList->tail = NULL;
	return newList;
}
int insertNode(DoubleList* list,int data)
{
	if (list == NULL)
	{
		printf("list is NULL!");
		return -1;
	}
	DoubleNode* newNode = createNode(data);
	if (newNode == NULL)
	{
		return -1;
	}
	if (list->head == NULL)
	{
		list->head = newNode;
		list->tail = newNode;
	}
	else
	{
		newNode->prev = list->tail;
		list->tail->next = newNode;
		list->tail = newNode;
	}
	return 1;
}
DoubleNode* findNode(DoubleList* list, int data)
{
	if (list == NULL)
	{
		printf("error!list is NULL!\n");
		return NULL;
	}
	DoubleNode* cur = list->head;
	while (cur != NULL)
	{
		if (cur->data == data)
		{
			return cur;
		}
		cur = cur->next;
	}
	printf("have not found the data!\n");
	return cur;
}
int deleteNode(DoubleList* list, int data)
{
	if (list == NULL)
	{
		printf("error!list is NULL!\n");
		return -1;
	}
	if (list->head == NULL)
	{
		printf("error!isNoNode!\n");
		return -1;
	}
	DoubleNode* target =findNode(list,data);
	if (target == NULL)
	{
		return 0;
	}
	if (target == list->head)
	{
		// 先把head指针移到下一个节点（核心）
		list->head = target->next;
		// 只有当新head不为空时，才清空它的prev（避免空指针）
		if (list->head != NULL)
		{
			list->head->prev = NULL;
		}
		// 如果删除的是最后一个节点（head==tail），同步清空tail
		if (list->tail == target)
		{
			list->tail = NULL;
		}
	}
	else if (target == list->tail)
	{
		list->tail->prev->next =NULL;
		list->tail->prev = NULL;
	}
	else
	{
		target->next->prev = target->prev;
		target->prev->next = target->next;
	}
	free(target);
	printf("have deleted the Node!\n");
	return 1;
}
int destroyList(DoubleList* list)
{
	if (list == NULL)
	{
		printf("list is NULL!\n");
		return -1;
	}

	if (list->head == NULL)
	{
		printf("list is empty!");
		return 0;
	}
	DoubleNode* cur = list->head;
	while (cur != NULL)
	{
		DoubleNode* temp = cur->next;
		free(cur);
		cur = temp;
	}
	free(list);
	printf("have destroyed the list!\n");
	return 1;
}
int main()
{
	return 0;
}