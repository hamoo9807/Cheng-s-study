#include <stdio.h>
#include <stdlib.h>

typedef struct DoubleNode
{
    int data;
    struct DoubleNode* prev;
    struct DoubleNode* next;
} DoubleNode;

typedef struct DoubleList
{
    DoubleNode* head;
    DoubleNode* tail;
    int size;
} DoubleList;

// ========== 原正确函数（复用） ==========
DoubleNode* createNode(int data)
{
    DoubleNode* newNode = (DoubleNode*)malloc(sizeof(DoubleNode));
    if (newNode == NULL)
    {
        printf("error!failed to create node\n");
        return NULL;
    }
    newNode->data = data;
    newNode->next = NULL;
    newNode->prev = NULL;
    return newNode;
}

DoubleList* initList()
{
    DoubleList* newList = (DoubleList*)malloc(sizeof(DoubleList));
    if (newList == NULL)
    {
        printf("error!failed to create newList\n");
        return NULL;
    }
    newList->head = NULL;
    newList->size = 0;
    newList->tail = NULL;
    return newList;
}

int insertNode(DoubleList* list, int data)
{
    if (list == NULL)
    {
        printf("list is NULL!\n");
        return -1;
    }
    DoubleNode* newNode = createNode(data);
    if (newNode == NULL)
    {
        return -1;
    }
    if (list->head == NULL)
    {
        list->head = newNode;
        list->tail = newNode;
    }
    else
    {
        newNode->prev = list->tail;
        list->tail->next = newNode;
        list->tail = newNode;
    }
    list->size++;
    return 1;
}

DoubleNode* findNode(DoubleList* list, int data)
{
    if (list == NULL)
    {
        printf("error!list is NULL!\n");
        return NULL;
    }
    DoubleNode* cur = list->head;
    while (cur != NULL)
    {
        if (cur->data == data)
        {
            return cur;
        }
        cur = cur->next;
    }
    printf("have not found the data!\n");
    return cur;
}

int deleteNode(DoubleList* list, int data)
{
    if (list == NULL)
    {
        printf("error!list is NULL!\n");
        return -1;
    }
    if (list->head == NULL)
    {
        printf("error!isNoNode!\n");
        return -1;
    }
    DoubleNode* target = findNode(list, data);
    if (target == NULL)
    {
        return 0;
    }
    if (target == list->head)
    {
        list->head = target->next;
        if (list->head != NULL)
        {
            list->head->prev = NULL;
        }
        if (list->tail == target)
        {
            list->tail = NULL;
        }
    }
    else if (target == list->tail)
    {
        list->tail = list->tail->prev;
        list->tail->next = NULL;
    }
    else
    {
        target->next->prev = target->prev;
        target->prev->next = target->next;
    }
    free(target);
    list->size--;
    printf("have deleted the Node!\n");
    return 1;
}

int destroyList(DoubleList* list)
{
    if (list == NULL)
    {
        printf("list is NULL!\n");
        return -1;
    }

    if (list->head == NULL)
    {
        printf("list is empty!\n");
        free(list);
        return 0;
    }
    DoubleNode* cur = list->head;
    while (cur != NULL)
    {
        DoubleNode* temp = cur->next;
        free(cur);
        cur = temp;
    }
    free(list);
    printf("have destroyed the list!\n");
    return 1;
}

// ========== 错误函数（复用） ==========
DoubleNode* badCreateNode(int data)
{
    DoubleNode* newNode = (DoubleNode*)malloc(sizeof(DoubleNode));
    // 缺少空指针检查
    newNode->data = data;
    newNode->next = NULL;
    newNode->prev = NULL;
    return newNode;
}

DoubleList* badInitList()
{
    DoubleList* newList = (DoubleList*)malloc(sizeof(DoubleList));
    if (newList == NULL)
    {
        printf("error!failed to create newList\n");
        return NULL;
    }
    newList->head = NULL;
    newList->size = 0;
    // 漏了初始化tail指针
    return newList;
}

int badInsertNode(DoubleList* list, int data)
{
    // 缺少list空指针检查
    DoubleNode* newNode = createNode(data);
    if (newNode == NULL)
    {
        return -1;
    }
    // 直接假设链表非空，处理错误
    newNode->prev = list->tail;
    list->tail->next = newNode;
    list->tail = newNode;
    return 1;
}

int badDeleteNode(DoubleList* list, int data)
{
    if (list == NULL)
    {
        printf("error!list is NULL!\n");
        return -1;
    }
    DoubleNode* target = findNode(list, data);
    if (target == NULL)
    {
        return 0;
    }
    if (target == list->head)
    {
        list->head = target->next;
        if (list->head != NULL)
        {
            list->head->prev = NULL;
        }
    }
    else if (target == list->tail)
    {
        // 错误处理：未正确更新tail指针
        target->prev->next = NULL;
    }
    else
    {
        target->next->prev = target->prev;
        target->prev->next = target->next;
    }
    free(target);
    return 1;
}

// ========== 辅助打印函数（增强对比） ==========
void printList(DoubleList* list, const char* name)
{
    if (list == NULL)
    {
        printf("%s: [NULL]\n", name);
        return;
    }
    printf("%s: 节点数据=[", name);
    DoubleNode* cur = list->head;
    while (cur != NULL)
    {
        printf("%d", cur->data);
        if (cur->next != NULL) printf(", ");
        cur = cur->next;
    }
    printf("] | size=%d | head=%p | tail=%p\n", 
           list->size, (void*)list->head, (void*)list->tail);
}

// ========== 修改后的main函数（核心） ==========
int main()
{
    printf("=========================================\n");
    printf("双向链表 正确函数 VS 错误函数 对比测试\n");
    printf("=========================================\n\n");

    // --------------------------
    // 测试1：初始化功能测试（验证指针初始化）
    // --------------------------
    printf("【测试1：初始化功能测试】\n");
    printf("测试目标：验证链表初始化时head/tail/size的初始值是否正确\n");
    
    // 正确函数调用
    DoubleList* goodList = initList();
    printf("✅ 正确函数(initList)结果：");
    printList(goodList, "goodList");
    printf("   分析：head/tail均为NULL，size=0，初始化符合预期\n");

    // 错误函数调用
    DoubleList* badList = badInitList();
    printf("❌ 错误函数(badInitList)结果：");
    printList(badList, "badList");
    printf("   错误原因：仅初始化head/size，未赋值tail=NULL，tail指向随机野指针\n\n");

    // --------------------------
    // 测试2：空链表插入边界测试（验证空链表插入逻辑）
    // --------------------------
    printf("【测试2：空链表插入边界测试】\n");
    printf("测试目标：验证空链表场景下插入节点的逻辑完整性\n");
    
    // 正确函数调用（空链表插入）
    int goodInsertRet = insertNode(goodList, 10);
    printf("✅ 正确函数(insertNode)插入10：");
    printList(goodList, "goodList");
    printf("   分析：空链表插入后，head/tail指向新节点，size=1，逻辑正确（返回值=%d）\n", goodInsertRet);

    // 错误函数调用（空链表插入，先规避崩溃：手动初始化badList的tail）
    // 注：若不手动初始化，会直接段错误，此处为展示错误逻辑，临时修复tail
    badList->tail = NULL; 
    printf("❌ 错误函数(badInsertNode)插入10（空链表）：");
    // 捕获段错误风险，先提示再执行
    printf("执行前警告：badInsertNode未处理空链表，直接访问list->tail->next会触发段错误！\n");
    // 注释掉崩溃代码，改为输出错误分析
    // int badInsertRet = badInsertNode(badList, 10); 
    printf("   错误原因：1. 未检查list是否为NULL；2. 未判断空链表，默认tail非空，访问NULL->next触发段错误；3. 插入后未更新size\n\n");

    // --------------------------
    // 测试3：尾节点删除边界测试（验证尾节点删除逻辑）
    // --------------------------
    printf("【测试3：尾节点删除边界测试】\n");
    printf("测试目标：验证删除尾节点时指针和元数据的一致性\n");
    
    // 先给正确链表插入多个节点
    insertNode(goodList, 20);
    insertNode(goodList, 30);
    printf("✅ 正确链表插入20/30后：");
    printList(goodList, "goodList");
    
    // 正确函数删除尾节点30
    int goodDeleteRet = deleteNode(goodList, 30);
    printf("✅ 正确函数(deleteNode)删除30：");
    printList(goodList, "goodList");
    printf("   分析：删除尾节点后，tail指向20，size=2，逻辑正确（返回值=%d）\n", goodDeleteRet);

    // 给错误链表插入节点（手动修复后）
    badList->head = createNode(10);
    badList->tail = badList->head;
    badList->size = 1;
    insertNode(badList, 20);
    insertNode(badList, 30); // 用正确函数插入，保证节点结构正常
    printf("❌ 错误链表插入20/30后：");
    printList(badList, "badList");
    
    // 错误函数删除尾节点30
    int badDeleteRet = badDeleteNode(badList, 30);
    printf("❌ 错误函数(badDeleteNode)删除30：");
    printList(badList, "badList");
    printf("   错误原因：1. 删除尾节点时仅关闭next指针，未更新tail=target->prev，tail仍指向已释放的30节点；2. 未执行size--，size仍为3；3. 野指针风险极高\n\n");

    // --------------------------
    // 资源销毁
    // --------------------------
    printf("【资源销毁】\n");
    destroyList(goodList);
    destroyList(badList);

    printf("=========================================\n");
    printf("测试总结：错误函数因缺失边界判断、元数据同步、空指针检查，导致链表状态混乱/崩溃；正确函数通过完整的逻辑校验保证链表一致性\n");
    printf("=========================================\n");

    return 0;
}