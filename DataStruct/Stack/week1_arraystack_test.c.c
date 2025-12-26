#include <stdio.h>
#include <stdlib.h>
#define MAX 6
#define empty -1
typedef struct stack {
	int data[MAX];
	int top;
}stack;
stack* initstack()
{
	stack* newS = (stack*)malloc(sizeof(stack));
	if (newS == NULL)
	{
		return NULL;
	}
	newS->top = empty;
	return newS;
}
int isEmpty(stack* s)
{
	return s->top == empty;
}
int isFull(stack* s)
{
	return s->top == MAX - 1;
}
int push(stack* s,int data)
{
	if (s == NULL)
	{
		return -1;
	}
	if (isFull(s))
	{
		return -1;
	}
	s->top++;
	s->data[s->top] = data;
	return 1;
}
int pop(stack* s)
{
	if (s == NULL)
	{
		return -1;
	}
	if (isEmpty(s))
		return -1;
	printf("%d", s->data[s->top]);
	s->top--;
	return 1;
}
int freeS(stack** s)
{
	if (s == NULL)
	{
		return -1;
	}
	if (*s == NULL)
	{
		return -1;
	}
	free(*s);
	*s = NULL;
	return 1;
}