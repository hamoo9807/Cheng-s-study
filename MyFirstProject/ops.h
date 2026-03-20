#ifndef OPS_H
#define OPS_H

#include "data.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#define SUCCESS          0   // 操作成功
#define ISBN_EXIST       -1  // ISBN已存在（添加书籍失败）
#define ISBN_NOT_FOUND   -2  // ISBN不存在（借阅/还书/删除失败）
#define MEM_ERR          -3  // 内存分配失败（替换原MEMORY_FULL，命名更准确）
#define EMPTY_LIST       -4  // 哈希表无任何书籍（原链表为空，语义微调）
#define INVALID_INPUT    -5  // 输入参数非法（空指针/负数/格式错误）
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
#define HASH_TABLE_SIZE 101  // 哈希表默认桶数量，质数有助于减少冲突
/*ErrorCodeDefine*/

// 初始化哈希表（替代原initList，指定桶数量，默认101）
BookHashTable* initHashTable();

// 销毁哈希表（释放所有桶、节点内存，替代原destroyList）
int destroyHashTable(BookHashTable* table);

// 判断哈希表是否为空（无任何书籍，替代原isEmpty）
int isHashTableEmpty(BookHashTable* table);

// 检查书籍是否可借阅（库存 > 已借出数）
int isBorrowable(BookHashTable* table, const char *isbn);

// 创建书籍节点（逻辑不变，复用）
BookNode* createNode(const char *isbn, const char *title, 
                     const char *author, int stock, int loanedNum);

// 添加书籍到哈希表（替代原addBook，补充loanedNum参数）
int addBook(BookHashTable* table, const char *isbn, const char *title, 
            const char *author, int stock, int loanedNum);

// 按ISBN查找书籍（核心优化：先哈希定位桶，再遍历桶内链表）
BookNode* isbnFindBook(BookHashTable* table, const char *isbn);


// 删除指定ISBN的书籍（替代原deleteBook）
int deleteBook(BookHashTable* table, const char *isbn);

// 更新书籍库存
int updateStock(BookHashTable* table, const char *isbn, int stock);

// 更新书籍标题
int updateTitle(BookHashTable* table, const char *isbn, const char *title);

// 更新书籍作者
int updateAuthor(BookHashTable* table, const char *isbn, const char *author);

// 更新已借出数量
int updateLoanedNum(BookHashTable* table, const char *isbn, int loanedNum);

/* ---------------------- 扩展辅助函数（可选，提升实用性） ---------------------- */
// 获取哈希表中书籍总数
int getTotalBookCount(BookHashTable* table);

// 遍历所有书籍（用于打印/导出，替代链表的遍历）
void traverseAllBooks(BookHashTable* table, void (*callback)(BookNode*));

unsigned long hashFunction(const char *isbn);

int getHashSocketIndex(const char *isbn, int bucket_count);


#endif