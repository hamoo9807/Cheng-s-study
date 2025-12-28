#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>  // 用于时间统计
#define MAX 10000  // 调整队列大小以避免频繁队满/队空
typedef int QueueData;
typedef struct Queue {
    QueueData data[MAX];
    int front;
    int rear;
    int count;  // 修正：添加分号
}Queue;

// 队列初始化
int initQueue(Queue* queue)
{
    if (queue == NULL) return 0;
    memset(queue->data, 0, sizeof(queue->data));
    queue->front = 0;
    queue->rear = 0;
    queue->count = 0;
    return 1;
}

// 判断队列是否为空
int isEmpty(Queue* queue)
{
    if (queue == NULL) return 0;
    return queue->count == 0;
}

// 判断队列是否为满
int isFull(Queue* queue)
{
    if (queue == NULL) return 0;
    return queue->count == MAX;
}

// 入队操作
int Enqueue(Queue* queue, QueueData data)
{
    if (queue == NULL) return 0;
    if (isFull(queue))
    {
        // 为避免频繁打印影响性能，仅在调试时开启
        // printf("队满！%d\n", data);
        return 0;
    }
    queue->data[queue->rear] = data;
    queue->rear = (queue->rear + 1) % MAX;
    queue->count++;
    return 1;
}

// 出队操作
int Dequeue(Queue* queue, QueueData* data)
{
    if (queue == NULL || data == NULL) return 0;
    if (isEmpty(queue))
    {
        // 为避免频繁打印影响性能，仅在调试时开启
        // printf("队空！出队失败\n");
        return 0;
    }
    *data = queue->data[queue->front];
    queue->front = (queue->front + 1) % MAX;
    queue->count--;
    return 1;
}

int main() {
    Queue queue;
    QueueData data;
    clock_t start, end;  // 时间变量
    double enqueue_time, dequeue_time;
    const int OPERATIONS = 10000;  // 操作次数

    // 初始化队列
    if (!initQueue(&queue)) {
        printf("初始化队列失败！\n");
        return -1;
    }

    // 测试1万次入队性能
    start = clock();  // 记录开始时间
    for (int i = 0; i < OPERATIONS; i++) {
        Enqueue(&queue, i);
    }
    end = clock();  // 记录结束时间
    enqueue_time = (double)(end - start) / CLOCKS_PER_SEC * 1000;  // 转换为毫秒

    // 测试1万次出队性能
    start = clock();  // 记录开始时间
    for (int i = 0; i < OPERATIONS; i++) {
        Dequeue(&queue, &data);
    }
    end = clock();  // 记录结束时间
    dequeue_time = (double)(end - start) / CLOCKS_PER_SEC * 1000;  // 转换为毫秒

    // 输出性能对比结果
    printf("数组队列性能测试（%d次操作）：\n", OPERATIONS);
    printf("入队总耗时：%.6f 毫秒\n", enqueue_time);
    printf("平均每次入队耗时：%.6f 微秒\n", enqueue_time * 1000 / OPERATIONS);
    printf("出队总耗时：%.6f 毫秒\n", dequeue_time);
    printf("平均每次出队耗时：%.6f 微秒\n", dequeue_time * 1000 / OPERATIONS);

    return 0;
}