#include <stdio.h>
#include <stdlib.h>
#include <time.h>  // 性能测试计时
typedef struct Node {
	int data;
	struct Node* next;
}Node;
typedef struct stack {
	Node* top;
}stack;

Node* createNode(int data)
{
	Node* newNode = (Node*)malloc(sizeof(Node));
	if (newNode == NULL)
	{
		printf("error!\n");
		return NULL;
	}
	newNode->data = data;
	newNode->next = NULL;
	return newNode;
}
stack* initStack()
{
	stack* newStack = (stack*)malloc(sizeof(stack));
	if (newStack == NULL)
	{
		printf("errro!\n");
		return NULL;
	}
	newStack->top = NULL;
	return newStack;
}
int isEmpty(stack* s)
{
	return s->top== NULL;
}
int push(stack* s, int data)
{
	if (s == NULL)
	{
		printf("error!\n");
		return -1;
	}
	Node* newNode= createNode(data);
	if (newNode == NULL)
	{
		return -1;
	}
	newNode->next = s->top;
	s->top = newNode;

	return  1;
}
int pop(stack* s)
{
	if (s == NULL)
	{
		printf("error!\n");
		return -1;
	}
	if (isEmpty(s))
	{
		printf("empty!\n");
		return 0;
	}

	Node* temp = s->top;
	s->top = s->top->next;
	free(temp);
	return 1;
}
int freeS(stack** s)
{
	if (s == NULL||*s==NULL)
	{
		return 0;
	}
	while (!isEmpty(*s))
	{
		Node* temp = (*s)->top;
		(*s)->top = (*s)->top->next;
		free(temp);
	}
	free(*s);
	*s = NULL;
	printf("free the stack!\n");
	return 1;
}
int Top(stack* s)
{
	if (s == NULL)
	{
		return -1;
	}
	if (isEmpty(s))
	{
		return -1;
	}
	printf("Top:%d\n",s->top->data);
	return s->top->data;
}
void performanceTest() {
    const int TEST_COUNT = 100000;
    stack* s = initStack();
    if (s == NULL) return;

    // 计时开始
    clock_t start = clock();

    // 1. 10万次入栈
    for (int i = 0; i < TEST_COUNT; i++) {
        push(s, i);
    }
    printf("✅ 完成%d次入栈\n", TEST_COUNT);

    // 2. 验证取栈顶（应为99999）
    int topVal = Top(s);
    if (topVal == TEST_COUNT - 1) {
        printf("✅ 栈顶验证成功：%d\n", topVal);
    } else {
        printf("❌ 栈顶验证失败，预期：%d，实际：%d\n", TEST_COUNT - 1, topVal);
    }

    // 3. 10万次出栈
    int popSuccess = 0;
    for (int i = 0; i < TEST_COUNT; i++) {
        if (pop(s) != -1) popSuccess++;
    }
    printf("✅ 完成%d次出栈\n", popSuccess);

    // 计时结束，输出性能结果
    clock_t end = clock();
    double cost = (double)(end - start) / CLOCKS_PER_SEC;
    printf("\n===== 性能测试结果 =====\n");
    printf("总操作次数：%d次入栈 + %d次出栈 = %d次\n", TEST_COUNT, popSuccess, TEST_COUNT + popSuccess);
    printf("总耗时：%.4f 秒\n", cost);
    printf("单次操作平均耗时：%.6f 秒\n", cost / (TEST_COUNT + popSuccess));

    // 销毁栈（验证无内存泄漏）
    freeS(&s);
}


int main()
{
	performanceTest();
	return 0;
}