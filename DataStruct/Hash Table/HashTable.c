#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef struct HashNode
{
	int key;
	int data;
	struct HashNode* next;
}HashNode;

typedef struct HashTable {
	HashNode** buckets; //链表指针数组
	int buckets_num; //数组下标
}HashTable;

int hash_func(int key, int buckets_num)
{
	return key % buckets_num;
}
HashTable* initTable(int bucket_num)//开几个链表
{
	if (bucket_num < 5)
	{
		printf("开链数过少！强制设置为10");
		bucket_num = 10;
	}
	HashTable* table = (HashTable*)malloc(sizeof(HashTable));
	if (table == NULL)
	{
		printf("No memory!\n");
		return NULL;
	}
	table->buckets = (HashNode**)calloc(bucket_num, sizeof(HashNode*));
	if (!table->buckets)
	{
		free(table);
		printf("手动分配失败！\n");
		return NULL;
	}
	table->buckets_num = bucket_num;
	return table;
}
int hash_insert(HashTable* table, int key, int data)
{
	if (table == NULL) return -1;
	int idx = hash_func(key, table->buckets_num);
	HashNode* cur = table->buckets[idx];
	while (cur != NULL) {
		if (cur->key == key)
		{
			cur->data = data;
			printf("表内已有键，进行值覆盖:%d", data);
			return 1;
		}
		cur = cur->next;
	}
	printf("表内未有键，创建新节点\n");
	HashNode* newNode = (HashNode*)malloc(sizeof(HashNode));
	HashNode* head = table->buckets[idx];
	if (newNode == NULL) return -1;
	newNode->key = key;
	newNode->data = data;
	newNode->next = head;
	table->buckets[idx] = newNode;
	return 1;
}
int hash_find(HashTable* table, int key)
{
	if (table == NULL) return -1;
	int idx = hash_func(key, table->buckets_num);
	HashNode* cur = table->buckets[idx];
	while (cur != NULL)
	{
		if (cur->key == key)
		{
			printf("已找到键，值为：%d", cur->data);
			return 1;
		}
		cur = cur->next;
	}
	printf("未找到键!\n");
	return 0;
}
int hash_print(HashTable* table)
{
	if (table == NULL) return -1;
	for (int i = 0; i < table->buckets_num; i++)
	{
		HashNode* cur = table->buckets[i];
		while (cur != NULL)
		{
			printf("桶%d查键%d,值为：%d\n", i,cur->key, cur->data);
			cur = cur->next;
		}
	}
	printf("已打印完成\n");
	return 1;
}
int hash_destroy(HashTable** table)
{
	if (table == NULL || *table == NULL) return -1;
	for (int i = 0; i < (*table)->buckets_num; i++)
	{
		HashNode* cur = (*table)->buckets[i];
		while (cur != NULL)
		{
			HashNode* temp = cur;
			cur = cur->next;
			free(temp);
			
		}
		printf("已释放桶%d", i);
	}
	free((*table)->buckets);
	free(*table);
	*table = NULL;	
	return 1;
}