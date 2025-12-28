#include <stdio.h>
#include<stdlib.h>
#define MAX 1024
typedef struct Node //基础链表节点定义
{
	int data;
	struct Node* next;
}Node;
typedef struct Queue {//基础链表头、尾、size定义
	Node* front;
	Node* rear;
	int size;
}Queue;
Node* createNode(int data) //基础链表创建
{
	Node* newNode = (Node*)malloc(sizeof(Node));
	if (newNode == NULL)
	{
		printf("No memory!\n");
		return NULL;
	}
	newNode->data = data;
	newNode->next = NULL;
	return newNode;
}
Queue* initQueue()
{
	Queue* newQueue = (Queue*)malloc(sizeof(Queue));
	if (newQueue == NULL)
	{
		printf("No memory!\n");
		return NULL;
	}
	newQueue->front = NULL;
	newQueue->rear = NULL;
	newQueue->size = 0;
	return newQueue;
}
int isFull(Queue* queue)
{
	if (queue == NULL)
	{
		printf("NULL queue!\n");
		return -1;
	}
	return queue->size == MAX;
}
int Enqueue(Queue* queue, int data)
{
	if (queue == NULL)
	{
		printf("queue null!\n");
		return -1;
	}
	if (isFull(queue)==1)
	{
		printf("queue FULL!\n");
		return -1;
	}
	Node* newNode = createNode(data); //以下为链表尾插操作
	if (newNode == NULL)
	{
		return -1;
	}
	if (queue->rear == NULL)
	{
		queue->front = newNode;
		queue->rear = newNode;
	}
	else
	{
		queue->rear->next = newNode;
		queue->rear = newNode;
	}//以上为链表尾插操作
	queue->size++;
	printf("Enqueue:%d\n", data);
	return 1;
}
int isEmpty(Queue* queue)
{
	if (queue == NULL)
	{
		printf("queue NULL!\n");
		return -1;
	}
	return queue->size == 0;
}
int Dequeue(Queue* queue,int* data)
{
	if (queue == NULL || data == NULL)
	{
		printf("NULL!\n");
		return -1;
	}
	if (isEmpty(queue)==1)
	{
		printf("queue Empty!\n");
		return -1;
	}
	printf("Dequeue:%d\n", queue->front->data);
	*data = queue->front->data;
	Node* temp = queue->front;
	if (queue->front->next == NULL)
	{
		queue->front = NULL;
		queue->rear = NULL;
	}
	else
	{
		queue->front = queue->front->next;
	}
	free(temp);
	queue->size--;
	return 1;
}
int DestroyQueue(Queue** queue)
{
	if (queue == NULL||*queue==NULL)
	{
		printf("queue NULL!\n");
		return 0;
	}
		Node* cur = (*queue)->front;
		while (cur != NULL)
		{
			Node* temp = cur->next;
			free(cur);
			cur = temp;
		}
	free(*queue);
	*queue = NULL;
	printf("have destroyed Queue!\n");
	return 1;
}
