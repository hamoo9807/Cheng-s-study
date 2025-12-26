#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#define empty -1
#define full 100000
typedef struct stack{
	int data[full];
int top;
}stack;
stack* initStack()
{
	stack* newStack = (stack*)malloc(sizeof(stack));
	if (newStack == NULL)
	{
		printf("error!No memory\n");
		return NULL;
	}
	newStack->top = empty;
	return newStack;
}
int isFull(stack* s)
{
	return s->top == full-1;
}
int isEmpty(stack* s)
{
	return s->top == empty;
}
int push(stack* s, int data)
{
	if (s == NULL)
	{
		printf("stack is NULL!\n");
		return -1;
	}
	if (isFull(s))
	{
		printf("stack is full!\n");
		return -1;
	}
	s->top++;
	s ->data[s->top] = data;

	return 1;
}
int pop(stack* s)
{
	if (s == NULL)
	{
		printf("error!stack is NULL\n");
		return -1;
	}
	if (isEmpty(s))
	{
		printf("error!stack is empty!\n");
		return -1;
	}

	s->top--;
	return 1;
}
int destroyStack(stack** s)
{
	if (s == NULL)
	{
		printf("stack is NULL!\n");
		return 0;
	}
	stack* temp = *s;
	*s = NULL;
	free(temp);

	printf("have freed the stack!\n");
	return 1;
}
int main()
{
clock_t start, end;  // 定义计时变量
	double total_time;

	stack* s = initStack();
	
	// 记录压栈开始时间
	start = clock();
	for (int i = 0; i < 100000; i++)  // 十万次压栈
	{
		push(s, i);
	}
	end = clock();  // 记录压栈结束时间
	total_time = (double)(end - start) / CLOCKS_PER_SEC;
	printf("十万次压栈操作耗时: %f 秒\n", total_time);

	// 记录出栈开始时间
	start = clock();
	for (int i = 0; i < 100000; i++)  // 十万次出栈
	{
		pop(s);
	}
	end = clock();  // 记录出栈结束时间
	total_time = (double)(end - start) / CLOCKS_PER_SEC;
	printf("十万次出栈操作耗时: %f 秒\n", total_time);

	destroyStack(&s);
	return 0;
}