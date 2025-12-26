#include <stdio.h>
#include <stdlib.h>
#include <time.h>
typedef struct Node
{
	int data;
	struct Node* next;
}Node;

typedef struct List
{
	Node* head;
	Node* tail;
	int size;
}List;
List* initlist()
{
	List* newlist = (List*)malloc(sizeof(List));
	if (newlist == NULL)
	{
		printf("error!\n");
		return NULL;
	}
	newlist->head = NULL;
	newlist->tail = NULL;
	newlist->size = 0;
	return newlist;
}
Node* createNode(int data)
{
	Node* newNode = (Node*)malloc(sizeof(Node));
	if (newNode == NULL)
	{
		printf("error!");
			return NULL;
	}
	newNode->data = data;
	newNode->next = NULL;
	return newNode;
}
int tailinsert(List *list, int data)
{
	if (list == NULL)
	{
		printf("error!");
		return -1;
	}
	Node* newNode = createNode(data);
	if (list->head == NULL)
	{
		list->head = newNode;
		list->tail = newNode;
	}
	else
	{
		list->tail->next = newNode;
		list->tail = newNode;
	}
	list->size++;
	return 0;
}
int headinsert(List* list, int data)
{
	if (list == NULL)
	{
		printf("error!\n"); 
		return -1;
	}
	Node* newNode = createNode(data);
	if (list->head == NULL)
	{
		list->head = newNode;
		list->tail = newNode;
	}
	else
	{
		newNode->next = list->head;
		list->head = newNode;
	}
	list->size++;
	return 0;
}
void travelList(List* list)
{
	if (list == NULL)
	{
		printf("error!");
		return;
	}
	Node *cur = list->head;
	while (cur != NULL)
	{
		printf("%d ", cur->data);
		cur = cur->next;
	}
	printf("\n");
}
void FreeList(List* list)
{
	if (list == NULL)
	{
		return;
	}
	Node* cur = list->head;
	while (cur != NULL)
	{
		Node* temp = cur->next;
		free(cur);
		cur = temp;
	}
	printf("FreeTree succeed!\n");
	list->head = NULL;
	list->tail = NULL;
	list->size = 0;
	free(list);
}
int DeleteNode(List* list,int data)
{
	if (list == NULL)
	{
		printf("error!empty list");
		return -1;
	}
	Node* target = findNode(list, data);
	if (target == NULL)
	{
		return -1;
	}
	Node* prev = NULL;
	if (target != list->head) { 
		prev = list->head;
		while (prev->next != NULL && prev->next != target) {
			prev = prev->next;
		}
		if (prev->next != target) {
			printf("delete error: target node not in list!\n");
			return -1;
		}
	}
	if (prev == NULL) { // target是头节点
		list->head = target->next;
		if (list->head == NULL) {
			list->tail = NULL;
		}
	}
	else { // target是中间/尾节点
		prev->next = target->next;
		if (target == list->tail) {
			list->tail = prev;
		}
	}

	free(target);
	list->size--;
	return 0;
}
Node* findNode(List* list, int data) {
	if (list == NULL || list->head == NULL) {
		printf("findNode error: 链表未初始化或为空！\n"); 
		return NULL;
	}
	Node* cur = list->head;
	while (cur != NULL) {
		if (cur->data == data) {
			printf("data exists: %d!\n", cur->data);
			return cur;
		}
		cur = cur->next;
	}
	printf("have not found the data: %d!\n", data); 
	return NULL;
}
int main()
{
    // 初始化链表
    List* newList = initlist();
    if (newList == NULL)
    {
        printf("链表初始化失败！\n");
        return -1;
    }
    const int TEST_COUNT = 100000; // 10万次测试，契合计划要求
    clock_t start, end;

    // ========== 10万次头插计时 ==========
    printf("=== 开始单向链表10万次头插测试 ===\n");
    start = clock();
    for (int i = 0; i < TEST_COUNT; i++)
    {
        headinsert(newList, i);
    }
    end = clock();
    double insert_cost = (double)(end - start) / CLOCKS_PER_SEC;

    // 输出插入量化结果
    printf("10万次头插完成！\n");
    printf("插入总耗时：%.6f 秒\n", insert_cost);
    printf("预期插入数量：%d 个\n", TEST_COUNT);
    printf("实际链表长度：%d 个\n", newList->size);
    printf("插入有效性：%s\n\n", (newList->size == TEST_COUNT) ? "✅ 正常" : "❌ 异常");

    // ========== 10万次删除计时（新增核心逻辑） ==========
    printf("=== 开始单向链表10万次删除测试 ===\n");
    start = clock();
    for (int i = 0; i < TEST_COUNT; i++)
    {
        DeleteNode(newList, i); // 删除所有插入的10万条数据
    }
    end = clock();
    double delete_cost = (double)(end - start) / CLOCKS_PER_SEC;

    // 输出删除量化结果
    printf("10万次删除完成！\n");
    printf("删除总耗时：%.6f 秒\n", delete_cost);
    printf("删除后预期长度：0 个\n");
    printf("删除后实际长度：%d 个\n", newList->size);
    printf("删除有效性：%s\n\n", (newList->size == 0) ? "✅ 正常" : "❌ 异常");

 

    // 释放内存
    FreeList(newList);
    return 0;
}