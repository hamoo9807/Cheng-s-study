#ifndef STORE_H
#define STORE_H

#include "data.h"       // 引入哈希表版的data.h（包含BookHashTable）
#include "user.h"       // 若user.h也改哈希表，需同步引入；否则保留原UserList
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// ===================== 常量定义（无需修改） =====================
#define BOOK_STORE_PATH "./data/books.json"   // 图书数据存储路径
#define USER_STORE_PATH "./data/users.json"   // 用户数据存储路径
#define LOG_STORE_PATH "./data/oper_log.txt"  // 日志存储路径
#define MAX_LINE_LEN 1024                     // 文件读取单行最大长度

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

// ===================== 图书数据导出/导入（必须改：BookList→BookHashTable） =====================
/**
 * @brief 导出哈希表中的图书数据到JSON文件（覆盖写入）
 * @param table 图书哈希表（替代原BookList）
 * @param filePath 存储路径（默认用BOOK_STORE_PATH）
 * @return int 成功=SUCCESS，失败=FILE_OPEN_ERR/FILE_WRITE_ERR/INVALID_INPUT
 */
int exportBooksToFile(BookHashTable* table, const char *filePath);

/**
 * @brief 从JSON文件导入图书数据到内存哈希表
 * @param table 空的图书哈希表（程序启动时初始化的空哈希表）
 * @param filePath 存储路径（默认用BOOK_STORE_PATH）
 * @return int 成功=SUCCESS，失败=FILE_OPEN_ERR/FILE_READ_ERR/JSON_PARSE_ERR
 */
int importBooksFromFile(BookHashTable* table, const char *filePath);

// ===================== 用户数据导出/导入（可选改：UserList→UserHashTable） =====================
/**
 * @brief 导出用户列表/哈希表数据到JSON文件（覆盖写入）
 * @note 若user.h未改哈希表，保留UserList*；若改了，替换为UserHashTable*
 * @param userList 用户列表/哈希表
 * @param filePath 存储路径（默认用USER_STORE_PATH）
 * @return int 成功=SUCCESS，失败=FILE_OPEN_ERR/FILE_WRITE_ERR/INVALID_INPUT
 */

int exportUsersToFile(UserList* userList, const char *filePath);

/**
 * @brief 从JSON文件导入用户数据到内存列表/哈希表
 * @note 若user.h未改哈希表，保留UserList*；若改了，替换为UserHashTable*
 * @param userList 空的用户列表/哈希表（程序启动时初始化的空结构）
 * @param filePath 存储路径（默认用USER_STORE_PATH）
 * @return int 成功=SUCCESS，失败=FILE_OPEN_ERR/FILE_READ_ERR/JSON_PARSE_ERR
 */
int importUsersFromFile(UserList* userList, const char *filePath);



#endif