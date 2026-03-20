#ifndef LOGIC_H
#define LOGIC_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ops.h"       // 引入哈希表版的ops.h（包含BookHashTable）
#include "config.h"
#include "user.h"

#define MAX_DAILY_BORROW_NUM 5   
// ===================== 错误码（复用+语义微调） =====================
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

/*ErrorCodeDefine*/

// ===================== 核心业务逻辑（全部替换BookList→BookHashTable） =====================
/**
 * @brief 按ISBN借阅书籍（核心逻辑不变，底层调用哈希表接口）
 * @param table 图书哈希表（替代原BookList）
 * @param isbn 图书ISBN
 * @param borrowNum 借阅数量
 * @param dailyBorrowed 每日已借阅数量（入参+出参，用于判断上限）
 * @return int 错误码（SUCCESS/ISBN_NOT_FOUND/BORROW_LIMIT_EXCEED等）
 */
int isbnBorrowBook(BookHashTable* table, const char *isbn, int borrowNum, UserNode* loginUser);


/**
 * @brief 归还书籍（按ISBN）
 * @param table 图书哈希表（替代原BookList）
 * @param isbn 图书ISBN
 * @param returnNum 归还数量
 * @return int 错误码（SUCCESS/ISBN_NOT_FOUND/INVALID_INPUT等）
 */
int returnBook(BookHashTable* table, const char *isbn, int returnNum, UserNode* loginUser);

/**
 * @brief 获取书籍已借出数量
 * @param table 图书哈希表（替代原BookList）
 * @param isbn 图书ISBN
 * @return int 成功=已借出数量，失败=错误码（ISBN_NOT_FOUND等）
 */
int getBorrowedNum(BookHashTable* table, const char *isbn);

/**
 * @brief 获取书籍总库存（stock字段）
 * @param table 图书哈希表（替代原BookList）
 * @param isbn 图书ISBN
 * @return int 成功=总库存，失败=错误码（ISBN_NOT_FOUND等）
 */
int getTotalNum(BookHashTable* table, const char *isbn);

/**
 * @brief 获取书籍可借阅数量（stock - loanedNum）
 * @param table 图书哈希表（替代原BookList）
 * @param isbn 图书ISBN
 * @return int 成功=可借阅数量，失败=错误码（ISBN_NOT_FOUND等）
 */
int getAvailableNum(BookHashTable* table, const char *isbn);

/**
 * @brief 管理员添加书籍（核心逻辑不变，底层调用哈希表addBook接口）
 * @param table 图书哈希表（替代原BookList）
 * @param isbn 图书ISBN
 * @param title 图书标题
 * @param author 图书作者
 * @param totalNum 总库存
 * @return int 错误码（SUCCESS/ISBN_EXIST/MEM_ERR等）
 */
int adminAddBook(BookHashTable* table, const char *isbn, const char *title, const char *author, int totalNum , UserNode* loginUser);

/**
 * @brief 管理员删除书籍（校验权限+借阅记录，底层调用哈希表deleteBook接口）
 * @param table 图书哈希表（替代原BookList）
 * @param loginUser 登录用户（用于校验管理员权限）
 * @param isbn 图书ISBN
 * @return int 错误码（SUCCESS/NO_PERMISSION/BOOK_HAS_BORROWED等）
 */
int adminDeleteBook(BookHashTable* table, UserNode* loginUser, const char *isbn);



#endif