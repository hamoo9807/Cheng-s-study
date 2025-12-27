#include <stdio.h>
#include <stdlib.h>
#include <time.h>  // 性能测试计时

typedef struct Node {
    int data;
    struct Node* next;
} Node;

// 队列结构：包含队头和队尾指针
typedef struct queue {
    Node* front;  // 队头（出队操作）
    Node* rear;   // 队尾（入队操作）
} Queue;

// 创建节点
Node* createNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("内存分配失败!\n");
        return NULL;
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

// 初始化队列
Queue* initQueue() {
    Queue* newQueue = (Queue*)malloc(sizeof(Queue));
    if (newQueue == NULL) {
        printf("内存分配失败!\n");
        return NULL;
    }
    newQueue->front = NULL;
    newQueue->rear = NULL;
    return newQueue;
}

// 判断队列是否为空
int isEmpty(Queue* q) {
    return q->front == NULL;  // 队头为空则队列空
}

// 入队操作（从队尾插入）
int enqueue(Queue* q, int data) {
    if (q == NULL) {
        printf("队列未初始化!\n");
        return -1;
    }
    Node* newNode = createNode(data);
    if (newNode == NULL) {
        return -1;
    }
    // 如果队列为空，队头和队尾都指向新节点
    if (isEmpty(q)) {
        q->front = newNode;
        q->rear = newNode;
    } else {
        // 否则在队尾添加新节点
        q->rear->next = newNode;
        q->rear = newNode;
    }
    return 1;
}

// 出队操作（从队头移除）
int dequeue(Queue* q) {
    if (q == NULL) {
        printf("队列未初始化!\n");
        return -1;
    }
    if (isEmpty(q)) {
        printf("队列为空，无法出队!\n");
        return 0;
    }
    // 保存队头节点
    Node* temp = q->front;
    // 更新队头指针
    q->front = q->front->next;
    // 如果队头为空，队尾也置为空（队列已空）
    if (q->front == NULL) {
        q->rear = NULL;
    }
    free(temp);
    return 1;
}

// 获取队头元素
int Front(Queue* q) {
    if (q == NULL) {
        printf("队列未初始化!\n");
        return -1;
    }
    if (isEmpty(q)) {
        printf("队列为空!\n");
        return -1;
    }
    printf("队头元素: %d\n", q->front->data);
    return q->front->data;
}

// 释放队列内存
int freeQueue(Queue** q) {
    if (q == NULL || *q == NULL) {
        return 0;
    }
    while (!isEmpty(*q)) {
        Node* temp = (*q)->front;
        (*q)->front = (*q)->front->next;
        free(temp);
    }
    free(*q);
    *q = NULL;
    printf("队列已释放!\n");
    return 1;
}

// 性能测试
void performanceTest() {
    const int TEST_COUNT = 100000;
    Queue* q = initQueue();
    if (q == NULL) return;

    // 计时开始
    clock_t start = clock();

    // 1. 10万次入队
    for (int i = 0; i < TEST_COUNT; i++) {
        enqueue(q, i);
    }
    printf("✅ 完成%d次入队\n", TEST_COUNT);

    // 2. 验证队头（应为0，因为队列先进先出）
    int frontVal = Front(q);
    if (frontVal == 0) {
        printf("✅ 队头验证成功：%d\n", frontVal);
    } else {
        printf("❌ 队头验证失败，预期：%d，实际：%d\n", 0, frontVal);
    }

    // 3. 10万次出队
    int dequeueSuccess = 0;
    for (int i = 0; i < TEST_COUNT; i++) {
        if (dequeue(q) != -1) dequeueSuccess++;
    }
    printf("✅ 完成%d次出队\n", dequeueSuccess);

    // 计时结束，输出性能结果
    clock_t end = clock();
    double cost = (double)(end - start) / CLOCKS_PER_SEC;
    printf("\n===== 性能测试结果 =====\n");
    printf("总操作次数：%d次入队 + %d次出队 = %d次\n", TEST_COUNT, dequeueSuccess, TEST_COUNT + dequeueSuccess);
    printf("总耗时：%.4f 秒\n", cost);
    printf("单次操作平均耗时：%.6f 秒\n", cost / (TEST_COUNT + dequeueSuccess));

    // 销毁队列
    freeQueue(&q);
}

int main() {
    performanceTest();
    return 0;
}