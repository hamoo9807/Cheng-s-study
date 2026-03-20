#ifndef USER_H
#define USER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define USER 2
#define ADMIN 1

#define USERNAME_MAX_LEN 12
#define PASSWORD_MAX_LEN 20
#define INIT_USER_CAPACITY 10 // 账号列表初始容量（可动态扩容）

typedef struct UserNode{
      char username[USERNAME_MAX_LEN];
        char password[PASSWORD_MAX_LEN];
        int role; // 1 for admin, 2 for user
        int dailyBorrowed; // 每日已借阅图书数量
	struct UserNode* next;
	struct UserNode* prev;
}UserNode;

typedef struct{
    UserNode* head;
     UserNode* tail;
    int size; // 当前用户数量
    int capacity; // 用户数组容量
}UserList;

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
UserList* initUserList();

int registerUser(UserList* userList, const char *username, 
                 const char *password, int role);


int login(UserList* userList, const char *username, 
          const char *password, UserNode *loginUser);

int isAdmin(UserNode user);

int resetDailyBorrowed(UserList* userList);

int destroyUserList(UserList* userList);

int checkAndResetDailyBorrowed(UserList* userList);



#endif