#include "ops.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "data.h"

// 统一错误码
#define SUCCESS          0   // 操作成功
#define ISBN_EXIST       -1  // ISBN已存在（添加书籍失败）
#define ISBN_NOT_FOUND   -2  // ISBN不存在（借阅/还书/删除失败）
#define MEM_ERR          -3  // 内存分配失败
#define EMPTY_LIST       -4  // 哈希表无任何书籍
#define INVALID_INPUT    -5  // 输入参数非法
#define UNKNOWN_ERROR    -6  // 未知错误
#define BORROW_LIMIT_EXCEED -7  // 超每日借阅上限（5本）
#define NO_MATCHED_BOOK  -8  // 关键词无匹配图书
#define INVALID_CHOICE   -9  // 关键词匹配多本时选择序号非法
#define BOOK_HAS_BORROWED -10 // 删除书籍时：有未归还借阅记录
#define USER_EXIST       -11 // 账号已存在（注册失败）
#define USER_NOT_FOUND   -12 // 账号不存在（登录失败）
#define PWD_ERROR        -13 // 密码错误（登录失败）
#define NO_PERMISSION    -14 // 无操作权限（普通用户执行管理员操作）
#define BOOK_STOCK_INSUFFICIENT -15 // 图书库存不足
#define HASH_TABLE_SIZE 101

// 初始化哈希表（无问题）
BookHashTable* initHashTable()
{
    BookHashTable* table = malloc(sizeof(BookHashTable)); // 移除不必要的强制转换
    if (!table) {
        fprintf(stderr, "内存分配失败：哈希表结构体\n");
        return NULL;
    }
    table->bucket_count = HASH_TABLE_SIZE;
    table->total_count = 0;
    table->buckets = malloc(HASH_TABLE_SIZE * sizeof(BookList));
    if (!table->buckets) {
        fprintf(stderr, "内存分配失败：桶数组\n");
        free(table);
        return NULL;
    }
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        table->buckets[i].head = NULL;
        table->buckets[i].tail = NULL;
        table->buckets[i].length = 0;
    }
    return table;
}

// 判断哈希表是否为空（无问题）
int isHashTableEmpty(BookHashTable* table)
{
    if (table == NULL) return INVALID_INPUT;
    return table->total_count == 0 ? 1 : 0;
}

// 创建图书节点（修复：初始化prev指针）
BookNode* createNode(const char *isbn, const char *title, 
const char *author, int stock, int loanedNum)
{
    // 调整校验顺序（先数值后字符串，逻辑更清晰）
    if (stock < 0 || loanedNum < 0) {
        fprintf(stderr, "错误：库存/已借出数量不能为负数\n");
        return NULL;
    }
    if(stock < loanedNum){
        fprintf(stderr, "错误：库存(%d)不能小于已借出数量(%d)\n", stock, loanedNum);
        return NULL;
    }
    if (isbn == NULL || title == NULL || author == NULL) {
        fprintf(stderr, "错误：ISBN/标题/作者不能为空\n");
        return NULL;
    }

    BookNode* newNode = malloc(sizeof(BookNode));
    if (!newNode) {
        fprintf(stderr, "内存分配失败：图书节点\n");
        return NULL;
    }

    // 分配字符串内存
    newNode->isbn = malloc(strlen(isbn) + 1);
    if (!newNode->isbn) goto err_free;

    newNode->title = malloc(strlen(title) + 1);
    if (!newNode->title) goto err_free_isbn;

    newNode->author = malloc(strlen(author) + 1);
    if (!newNode->author) goto err_free_title;

    // 拷贝字符串
    strcpy(newNode->isbn, isbn);
    strcpy(newNode->title, title);
    strcpy(newNode->author, author);

    // 修复：初始化双向链表prev指针
    newNode->stock = stock;
    newNode->loanedNum = loanedNum;
    newNode->prev = NULL; // 新增：初始化前驱指针
    newNode->next = NULL;

    return newNode;

    // 错误处理
err_free_title:
    free(newNode->title);
err_free_isbn:
    free(newNode->isbn);
err_free:
    free(newNode);
    fprintf(stderr, "内存分配失败：图书节点字符串\n");
    return NULL;
}

// 按ISBN查询图书（无问题）
BookNode* isbnFindBook(BookHashTable* table, const char *isbn){
    if (table == NULL) {
        fprintf(stderr, "错误：哈希表未初始化\n");
        return NULL;
    }
    if (isbn == NULL || *isbn == '\0') {
        fprintf(stderr, "错误：ISBN不能为空（空指针/空字符串）\n");
        return NULL;
    }

    int index = getHashSocketIndex(isbn, table->bucket_count); // 优化：复用索引函数
    if (index < 0) return NULL;
    
    BookList* bucket = &table->buckets[index];
    BookNode* current = bucket->head;

    while (current != NULL) {
        if (strcmp(current->isbn, isbn) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// 添加图书（修复：错误码+索引类型）
int addBook(BookHashTable* table, const char *isbn, const char *title, 
 const char *author, int stock, int loanedNum)
 {
    // 修复：统一错误码
    if (table == NULL) {
        fprintf(stderr, "错误：哈希表未初始化\n");
        return INVALID_INPUT; // 替换原-1
    }
    if (isbn == NULL || *isbn == '\0' || title == NULL || author == NULL) {
        fprintf(stderr, "错误：ISBN/标题/作者不能为空（空指针/空字符串）\n");
        return INVALID_INPUT;
    }
    if (stock < 0 || loanedNum < 0 || loanedNum > stock) {
        fprintf(stderr, "错误：库存(%d)或已借出数量(%d)不合法\n", stock, loanedNum);
        return INVALID_INPUT;
    }

    // 检查重复ISBN
    BookNode* existBook = isbnFindBook(table, isbn);
    if (existBook != NULL) {
        fprintf(stderr, "错误：ISBN「%s」已存在，无法重复添加\n", isbn);
        return ISBN_EXIST; 
    }

    // 创建新节点（修复：错误码）
    BookNode* newNode = createNode(isbn, title, author, stock, loanedNum);
    if (newNode == NULL) {
        fprintf(stderr, "错误：创建图书节点失败\n");
        return MEM_ERR; // 替换原-1
    }

    // 修复：索引类型+校验逻辑
    int index = getHashSocketIndex(isbn, table->bucket_count);
    if (index < 0 || index >= table->bucket_count) {
        fprintf(stderr, "错误：ISBN「%s」索引(%d)非法，添加失败\n", isbn, index);
        free(newNode->isbn);
        free(newNode->title);
        free(newNode->author);
        free(newNode);
        return UNKNOWN_ERROR;
    }
    BookList* bucket = &table->buckets[index];

    // 尾插法（补充prev指针赋值）
    if (bucket->head == NULL) {
        bucket->head = newNode;
        bucket->tail = newNode;
    } else {
        newNode->prev = bucket->tail; // 新增：双向链表前驱赋值
        bucket->tail->next = newNode;
        bucket->tail = newNode;
    }
    bucket->length++;
    table->total_count++;
    fprintf(stdout, "成功添加图书：ISBN「%s」，标题「%s」\n", isbn, title);
    return SUCCESS;
 }

// 哈希函数（修复：返回值混淆）
unsigned long hashFunction(const char *isbn)
{
    if (isbn == NULL || *isbn == '\0') {
        fprintf(stderr, "错误：ISBN不能为空\n");
        return 0; // 修复：返回0而非错误码（unsigned long不能存负数）
    }

    unsigned long hash = 5381;
    int c;
    while ((c = *isbn++)) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return hash;
}

// 获取哈希索引（无问题）
int getHashSocketIndex(const char *isbn, int bucket_count)
{
    if (isbn == NULL || *isbn == '\0') {
        fprintf(stderr, "错误：ISBN不能为空\n");
        return INVALID_INPUT; // 替换原ISBN_NOT_FOUND，语义更准确
    }
    if (bucket_count <= 0) {
        fprintf(stderr, "错误：哈希桶数量必须为正整数\n");
        return UNKNOWN_ERROR;
    }
    unsigned long hashValue = hashFunction(isbn);
    return hashValue % bucket_count;
}

// 获取总图书数（无问题）
int getTotalBookCount(BookHashTable* table)
{
    if (table == NULL) {
        fprintf(stderr, "错误：哈希表未初始化\n");
        return INVALID_INPUT;
    }
    return table->total_count;
}

// 删除图书（无问题）
int deleteBook(BookHashTable* table, const char *isbn){
    if (table == NULL) {
        fprintf(stderr, "错误：哈希表未初始化\n");
        return INVALID_INPUT;
    }
    if (isbn == NULL || *isbn == '\0') {
        fprintf(stderr, "错误：ISBN不能为空（空指针/空字符串）\n");
        return INVALID_INPUT;
    }

    BookNode* target = isbnFindBook(table, isbn);
    if (target == NULL) {
        fprintf(stderr, "错误：ISBN「%s」不存在，无法删除\n", isbn);
        return ISBN_NOT_FOUND;
    }

    if (target->loanedNum > 0) {
        fprintf(stderr, "错误：无法删除ISBN「%s」，有%d本未归还\n", isbn, target->loanedNum);
        return BOOK_HAS_BORROWED;
    }

    int index = getHashSocketIndex(isbn, table->bucket_count);
    if (index < 0 || index >= table->bucket_count) {
        fprintf(stderr, "错误：ISBN「%s」的索引(%d)超出范围，删除失败\n", isbn, index);
        return UNKNOWN_ERROR;
    }
    BookList* bucket = &table->buckets[index];

    // 双向链表指针调整
    if (target == bucket->head) {
        bucket->head = target->next;
        if (bucket->head != NULL) {
            bucket->head->prev = NULL;
        } else {
            bucket->tail = NULL;
        }
    } else if (target == bucket->tail) {
        bucket->tail = target->prev;
        if (bucket->tail != NULL) {
            bucket->tail->next = NULL;
        }
    } else {
        target->prev->next = target->next;
        target->next->prev = target->prev;
    }

    // 释放内存
    free(target->isbn);
    free(target->title);
    free(target->author);
    free(target);

    // 更新计数
    bucket->length--;
    table->total_count--;

    fprintf(stdout, "成功删除ISBN「%s」的图书\n", isbn);
    return SUCCESS;
}

// 更新库存（无问题）
int updateStock(BookHashTable* table, const char *isbn, int stock)
{
    if (table == NULL) {
        fprintf(stderr, "错误：哈希表未初始化\n");
        return INVALID_INPUT;
    }
    if (isbn == NULL || *isbn == '\0') {
        fprintf(stderr, "错误：ISBN不能为空（空指针/空字符串）\n");
        return INVALID_INPUT;
    }
    if (stock < 0) {
        fprintf(stderr, "错误：库存不能为负数\n");
        return INVALID_INPUT;
    }

    BookNode* target = isbnFindBook(table, isbn);
    if (target == NULL) {
        fprintf(stderr, "错误：ISBN「%s」不存在，无法更新库存\n", isbn);
        return ISBN_NOT_FOUND;
    }
    if (stock < target->loanedNum) {
        fprintf(stderr, "错误：新库存(%d)不能小于已借出数量(%d)\n", stock, target->loanedNum);
        return INVALID_INPUT;
    }

    target->stock = stock;
    fprintf(stdout, "成功更新ISBN「%s」的库存为%d\n", isbn, stock);
    return SUCCESS;
}

// 更新标题（修复：重复校验）
int updateTitle(BookHashTable* table, const char *isbn, const char *title){
    // 修复：合并重复校验，调整顺序
    if (table == NULL) {
        fprintf(stderr, "错误：哈希表未初始化\n");
        return INVALID_INPUT;
    }
    if (isbn == NULL || *isbn == '\0') {
        fprintf(stderr, "错误：ISBN不能为空（空指针/空字符串）\n");
        return INVALID_INPUT;
    }
    if (title == NULL || *title == '\0') {
        fprintf(stderr, "错误：新标题不能为空\n");
        return INVALID_INPUT;
    }

    BookNode* target = isbnFindBook(table, isbn);
    if (target == NULL) {
        fprintf(stderr, "错误：ISBN「%s」不存在，无法更新标题\n", isbn);
        return ISBN_NOT_FOUND;
    }

    // 优化：新旧标题相同则直接返回
    if (strcmp(target->title, title) == 0) {
        fprintf(stdout, "提示：ISBN「%s」的标题已是「%s」，无需更新\n", isbn, title);
        return SUCCESS;
    }

    char* newTitle = malloc(strlen(title) + 1);
    if (!newTitle) {
        fprintf(stderr, "内存分配失败：新标题\n");
        return MEM_ERR;
    }
    strcpy(newTitle, title);

    free(target->title);
    target->title = newTitle;

    fprintf(stdout, "成功更新ISBN「%s」的标题为「%s」\n", isbn, title);
    return SUCCESS;
}

// 更新作者（修复：重复校验）
int updateAuthor(BookHashTable* table, const char *isbn, const char *author)
{
    // 修复：合并重复校验，调整顺序
    if (table == NULL) {
        fprintf(stderr, "错误：哈希表未初始化\n");
        return INVALID_INPUT;
    }
    if (isbn == NULL || *isbn == '\0') {
        fprintf(stderr, "错误：ISBN不能为空（空指针/空字符串）\n");
        return INVALID_INPUT;
    }
    if (author == NULL || *author == '\0') {
        fprintf(stderr, "错误：新作者不能为空\n");
        return INVALID_INPUT;
    }

    BookNode* target = isbnFindBook(table, isbn);
    if (target == NULL) {
        fprintf(stderr, "错误：ISBN「%s」不存在，无法更新作者\n", isbn);
        return ISBN_NOT_FOUND;
    }

    // 优化：新旧作者相同则直接返回
    if (strcmp(target->author, author) == 0) {
        fprintf(stdout, "提示：ISBN「%s」的作者已是「%s」，无需更新\n", isbn, author);
        return SUCCESS;
    }

    char* newAuthor = malloc(strlen(author) + 1);
    if (!newAuthor) {
        fprintf(stderr, "内存分配失败：新作者\n");
        return MEM_ERR;
    }
    strcpy(newAuthor, author);

    free(target->author);
    target->author = newAuthor;

    fprintf(stdout, "成功更新ISBN「%s」的作者为「%s」\n", isbn, author);
    return SUCCESS;
}

// 更新已借出数量（无问题）
int updateLoanedNum(BookHashTable* table, const char *isbn, int loanedNum)
{
    if (table == NULL) {
        fprintf(stderr, "错误：哈希表未初始化\n");
        return INVALID_INPUT;
    }
    if (isbn == NULL || *isbn == '\0') {
        fprintf(stderr, "错误：ISBN不能为空（空指针/空字符串）\n");
        return INVALID_INPUT;
    }
    if (loanedNum < 0) {
        fprintf(stderr, "错误：已借出数量不能为负数\n");
        return INVALID_INPUT;
    }

    BookNode* target = isbnFindBook(table, isbn);
    if (target == NULL) {
        fprintf(stderr, "错误：ISBN「%s」不存在，无法更新已借出数量\n", isbn);
        return ISBN_NOT_FOUND;
    }
    if (loanedNum > target->stock) {
        fprintf(stderr, "错误：已借出数量(%d)不能大于库存(%d)\n", loanedNum, target->stock);
        return INVALID_INPUT;
    }

    target->loanedNum = loanedNum;
    fprintf(stdout, "成功更新ISBN「%s」的已借出数量为%d\n", isbn, loanedNum);
    return SUCCESS;
}

// 遍历所有图书（无问题）
void traverseAllBooks(BookHashTable* table, void (*callback)(BookNode*))
{
    if (table == NULL || callback == NULL) {
        fprintf(stderr, "错误：哈希表未初始化或回调函数不能为空\n");
        return;
    }
    for (int i = 0; i < table->bucket_count; i++) {
        BookNode* current = table->buckets[i].head;
        while (current != NULL) {
            callback(current);
            current = current->next;
        }
    }
}

// 判断是否可借阅（无问题）
int isBorrowable(BookHashTable* table, const char *isbn)
{
    if (table == NULL) {
        fprintf(stderr, "错误：哈希表未初始化\n");
        return INVALID_INPUT;
    }
    if (isbn == NULL || *isbn == '\0') {
        fprintf(stderr, "错误：ISBN不能为空（空指针/空字符串）\n");
        return INVALID_INPUT;
    }

    BookNode* target = isbnFindBook(table, isbn);
    if (target == NULL) {
        fprintf(stderr, "错误：ISBN「%s」不存在，无法判断是否可借\n", isbn);
        return ISBN_NOT_FOUND;
    }

    return target->loanedNum < target->stock ? 1 : 0;
}

// ops.c 末尾补充
int destroyHashTable(BookHashTable* table)
{
    if (table == NULL) {
        fprintf(stderr, "错误：哈希表未初始化，无法销毁\n");
        return INVALID_INPUT;
    }

    // 遍历所有桶，释放每个桶的链表节点
    for (int i = 0; i < table->bucket_count; i++) {
        BookNode* cur = table->buckets[i].head;
        while (cur != NULL) {
            BookNode* temp = cur;
            cur = cur->next;
            // 释放字符串内存
            free(temp->isbn);
            free(temp->title);
            free(temp->author);
            // 释放节点本身
            free(temp);
        }
        // 清空桶的指针
        table->buckets[i].head = NULL;
        table->buckets[i].tail = NULL;
        table->buckets[i].length = 0;
    }

    // 释放桶数组
    free(table->buckets);
    // 释放哈希表结构体本身
    free(table);
    
    fprintf(stdout, "成功销毁哈希表\n");
    return SUCCESS;
}