#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>  // Windows高精度计时头文件

// ===================== 数组队列（循环队列）实现 =====================
#define ARRAY_QUEUE_MAX 10000  // 适配1万次入队
typedef int QueueData;
typedef struct ArrayQueue {
    QueueData data[ARRAY_QUEUE_MAX];
    int front;
    int rear;
    int count;
} ArrayQueue;

// 数组队列初始化
int initArrayQueue(ArrayQueue* queue) {
    if (queue == NULL) return 0;
    memset(queue->data, 0, sizeof(queue->data));
    queue->front = 0;
    queue->rear = 0;
    queue->count = 0;
    return 1;
}

// 数组队列判空
int isArrayQueueEmpty(ArrayQueue* queue) {
    if (queue == NULL) return 0;
    return queue->count == 0;
}

// 数组队列判满
int isArrayQueueFull(ArrayQueue* queue) {
    if (queue == NULL) return 0;
    return queue->count == ARRAY_QUEUE_MAX;
}

// 数组队列入队
int arrayEnqueue(ArrayQueue* queue, QueueData data) {
    if (queue == NULL || isArrayQueueFull(queue)) return 0;
    queue->data[queue->rear] = data;
    queue->rear = (queue->rear + 1) % ARRAY_QUEUE_MAX;
    queue->count++;
    return 1;
}

// 数组队列出队
int arrayDequeue(ArrayQueue* queue, QueueData* data) {
    if (queue == NULL || data == NULL || isArrayQueueEmpty(queue)) return 0;
    *data = queue->data[queue->front];
    queue->front = (queue->front + 1) % ARRAY_QUEUE_MAX;
    queue->count--;
    return 1;
}

// ===================== 链式队列实现 =====================
#define LINK_QUEUE_MAX 10000
typedef struct LinkNode {
    int data;
    struct LinkNode* next;
} LinkNode;

typedef struct LinkQueue {
    LinkNode* front;
    LinkNode* rear;
    int size;
} LinkQueue;

// 创建链表节点
LinkNode* createLinkNode(int data) {
    LinkNode* newNode = (LinkNode*)malloc(sizeof(LinkNode));
    if (newNode == NULL) return NULL;
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

// 初始化链式队列
LinkQueue* initLinkQueue() {
    LinkQueue* q = (LinkQueue*)malloc(sizeof(LinkQueue));
    if (q == NULL) return NULL;
    q->front = NULL;
    q->rear = NULL;
    q->size = 0;
    return q;
}

// 链式队列入队
int linkEnqueue(LinkQueue* q, int data) {
    if (q == NULL || q->size >= LINK_QUEUE_MAX) return 0;
    LinkNode* newNode = createLinkNode(data);
    if (newNode == NULL) return 0;
    if (q->rear == NULL) {
        q->front = newNode;
        q->rear = newNode;
    }
    else {
        q->rear->next = newNode;
        q->rear = newNode;
    }
    q->size++;
    return 1;
}

// 链式队列出队
int linkDequeue(LinkQueue* q, int* data) {
    if (q == NULL || data == NULL || q->size == 0) return 0;
    LinkNode* temp = q->front;
    *data = temp->data;
    q->front = q->front->next;
    if (q->front == NULL) q->rear = NULL;
    free(temp);
    q->size--;
    return 1;
}

// 销毁链式队列
void destroyLinkQueue(LinkQueue* q) {
    if (q == NULL) return;
    LinkNode* cur = q->front;
    while (cur != NULL) {
        LinkNode* temp = cur->next;
        free(cur);
        cur = temp;
    }
    free(q);
}

// ===================== 高精度计时函数（Windows） =====================
// 获取当前高精度时间（微秒）
double getCurrentTimeUs() {
    LARGE_INTEGER freq, count;
    QueryPerformanceFrequency(&freq);  // 获取计数器频率
    QueryPerformanceCounter(&count);    // 获取当前计数值
    return (double)count.QuadPart * 1000000 / freq.QuadPart;
}

// ===================== 主函数：性能对比测试 =====================
int main() {
    const int TOTAL_OP = 10000;  // 1万次操作
    double start, end;
    double array_enq_time, array_deq_time;
    double link_enq_time, link_deq_time;
    QueueData data;

    // -------------------- 1. 测试数组队列 --------------------
    ArrayQueue array_q;
    initArrayQueue(&array_q);

    // 数组队列入队计时
    start = getCurrentTimeUs();
    for (int i = 0; i < TOTAL_OP; i++) {
        arrayEnqueue(&array_q, i);
    }
    end = getCurrentTimeUs();
    array_enq_time = end - start;

    // 数组队列出队计时
    start = getCurrentTimeUs();
    for (int i = 0; i < TOTAL_OP; i++) {
        arrayDequeue(&array_q, &data);
    }
    end = getCurrentTimeUs();
    array_deq_time = end - start;

    // -------------------- 2. 测试链式队列 --------------------
    LinkQueue* link_q = initLinkQueue();
    if (link_q == NULL) {
        printf("链式队列初始化失败！\n");
        return -1;
    }

    // 链式队列入队计时
    start = getCurrentTimeUs();
    for (int i = 0; i < TOTAL_OP; i++) {
        linkEnqueue(link_q, i);
    }
    end = getCurrentTimeUs();
    link_enq_time = end - start;

    // 链式队列出队计时
    start = getCurrentTimeUs();
    for (int i = 0; i < TOTAL_OP; i++) {
        linkDequeue(link_q, &data);
    }
    end = getCurrentTimeUs();
    link_deq_time = end - start;

    // -------------------- 3. 输出性能对比结果 --------------------
    printf("===================== 队列性能对比（%d次操作） =====================\n", TOTAL_OP);
    printf("| 队列类型   | 入队总耗时(微秒) | 单次入队耗时(微秒) | 出队总耗时(微秒) | 单次出队耗时(微秒) |\n");
    printf("|------------|------------------|--------------------|------------------|--------------------|\n");
    printf("| 数组队列   | %.2f             | %.6f               | %.2f             | %.6f               |\n",
        array_enq_time, array_enq_time / TOTAL_OP,
        array_deq_time, array_deq_time / TOTAL_OP);
    printf("| 链式队列   | %.2f             | %.6f               | %.2f             | %.6f               |\n",
        link_enq_time, link_enq_time / TOTAL_OP,
        link_deq_time, link_deq_time / TOTAL_OP);

    // 销毁链式队列
    destroyLinkQueue(link_q);
    return 0;
}