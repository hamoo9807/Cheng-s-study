#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct DoubleNode {
	int data;
	struct DoubleNode *prev;
	struct DoubleNode *next;
}DoubleNode;

typedef struct DoubleList {
	int size;
	DoubleNode* head;
	DoubleNode* tail;
}DoubleList;

DoubleList* InitList()
{
	DoubleList* newList = (DoubleList*)malloc(sizeof(DoubleList));
	if (newList == NULL)
	{
		printf("There're no memory!\n");
		return NULL;
	}
		newList->head = NULL;
	newList->tail = NULL;
	newList->size = 0;
	return newList;
}
DoubleNode* CreateNode(int data)
{
	DoubleNode* newNode = (DoubleNode*)malloc(sizeof(DoubleNode));
	if (newNode == NULL)
	{
		printf("error!\n");
		return NULL;
	}
	newNode->data = data;
	newNode->next = NULL;
	newNode->prev = NULL;
	return newNode;
}
int InsertNode(DoubleList* list, int data)
{
	if (list == NULL)
	{
		printf("List is empty!\n");
		return -1;
	}
	DoubleNode* newNode = CreateNode(data);
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
	list->size++;
	return 0;
}
int DeleteNode(DoubleList* list,int data){ //查询到首个符合data的点并删除
	//（核心差异：双向链表不需要单向链表那样找前驱节点）
	if (list == NULL)
	{
		printf("isEmpty!\n");
		return -1;
	}
	DoubleNode* cur = findNode(list, data);
	if(cur != NULL)
	{
		if (cur == list->head)
		{
			if (cur == list->tail)
			{
				list->head = NULL;
				list->tail = NULL;
			}
			else
			{
				cur->next->prev = NULL;
				list->head = cur->next;
			}
		}
		else if (cur == list->tail)
		{
			cur->prev->next = NULL;
			list->tail = cur->prev;
		}
		else
		{
			cur->prev->next = cur->next;
			cur->next->prev = cur->prev;
		}
		free(cur);
		list->size--;
		return 0;
	}
	printf("isNodata\n");
	return -1;
}
void travelFrd(DoubleList *list)
{
	if (list == NULL)
	{
		printf("isEmpty!error!\n");
		return;
	}
	DoubleNode* cur = list->head;
	printf("tvlfrd:");
	while (cur != NULL)
	{
		printf("%d ", cur->data);
		cur = cur->next;
	}
	printf("\n");
}

void travelBck(DoubleList* list)
{
	if (list == NULL)
	{
		printf("list Empty!error!\n");
		return;
	}
	DoubleNode* cur = list->tail;
	printf("tvlbck:");
	while (cur != NULL)
	{
		printf("%d ", cur->data);
		cur = cur->prev;
	}
	printf("\n");
}
void destroyList(DoubleList* list)
{
	if (list == NULL)
	{
		return;
	}
	DoubleNode* cur = list->head;
	while (cur != NULL)
	{
		DoubleNode* temp = cur;
		cur = cur->next;
		free(temp);
	}
	free(list);
}
DoubleNode* findNode(DoubleList* list,int data)
{
	if (list == NULL)
	{
		printf("error!\n");
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
	return NULL;
}
int main()
{
    DoubleList* newlist = InitList();
    if (newlist == NULL)
    {
        return -1;
    }

    // 1. 10万次尾插操作及计时
    printf("开始执行10万次尾插操作...\n");
    clock_t insert_start = clock();
    for (int i = 0; i < 100000; i++)
    {
        InsertNode(newlist, i);
    }
    clock_t insert_end = clock();
    double insert_time = (double)(insert_end - insert_start) / CLOCKS_PER_SEC;
    DoubleNode* cur = newlist->head;
    for (int i = 0; i < 10 && cur != NULL; i++)
    {
        printf("%d ", cur->data);
        cur = cur->next;
    }
    printf("...\n");

    // 3. 10万次删除操作及计时
    printf("开始执行10万次删除操作...\n");
    clock_t delete_start = clock();
    for (int i = 0; i < 100000; i++)
    {
        DeleteNode(newlist, i);
    }
    clock_t delete_end = clock();
    double delete_time = (double)(delete_end - delete_start) / CLOCKS_PER_SEC;

    // 4. 输出统计结果
    printf("\n===== 操作统计结果 =====");
    printf("\n10万次尾插耗时：%.6f秒", insert_time);
    printf("\n尾插后链表长度：%d", newlist->size);
    printf("\n10万次删除耗时：%.6f秒", delete_time);
    printf("\n删除后链表长度：%d\n", newlist->size);

    destroyList(newlist);
    return 0;
}