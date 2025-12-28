#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX 1024

typedef struct Queue
{
	int data[MAX];
	int front;
	int rear;
	int count;
}Queue;
int initQueue(Queue* queue)
{
	if (queue == NULL)
	{
		printf("queue NULL!\n");
		return -1;
	}
	queue->count = 0;
	memset(queue->data, 0, sizeof(queue->data));
	queue->front = 0;
	queue->rear = 0;
	return 1;
}
int isEmpty(Queue* queue)
{
	if (queue == NULL)
	{
		printf("queue NULL!\n");
		return -1;
	}
	return queue->count == 0;
}
int isFull(Queue* queue)
{
	if (queue == NULL)
	{
		printf("queue NULL!\n");
		return -1;
	}
	return queue->count == MAX;
}
int Enqueue(Queue* queue, int data)
{
	if (queue == NULL)
	{
		printf("NULL queue!\n");
		return -1;
  }
	if (isFull(queue))
	{
		printf("Full Queue!\n");
		return -1;
	}
	queue->data[queue->rear] = data;
	queue->rear = (queue->rear + 1) % MAX;//利用取模，去除MAX周期数
	printf("Enqueue:%d\n", data);
	queue->count++;
	return 1;
}

int Dequeue(Queue* queue,int* data)
{
	if (queue == NULL||data==NULL)
	{
		printf("NULL!\n");
		return -1;
	}
	if (isEmpty(queue))
	{
		printf("Empty queue!\n");
		return -1;
	}
	*data = queue->data[queue->front];
	printf("Dequeue:%d\n", queue->data[queue->front]);
	queue->front = (queue->front + 1) % MAX;
	queue->count--;
	return 1;
}