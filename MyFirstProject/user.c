#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>  // 新增：引入时间头文件
#include "ops.h"
#include "config.h"
#include "user.h"

#define USER 2
#define ADMIN 1

#define USERNAME_MAX_LEN 12
#define PASSWORD_MAX_LEN 20
#define INIT_USER_CAPACITY 10

// ===================== 错误码 =====================
#define SUCCESS          0
#define ISBN_EXIST       -1
#define ISBN_NOT_FOUND   -2
#define MEM_ERR          -3
#define EMPTY_LIST       -4
#define INVALID_INPUT    -5
#define UNKNOWN_ERROR    -6
#define BORROW_LIMIT_EXCEED -7
#define NO_MATCHED_BOOK  -8
#define INVALID_CHOICE   -9
#define BOOK_HAS_BORROWED -10
#define USER_EXIST       -11
#define USER_NOT_FOUND   -12
#define PWD_ERROR        -13
#define NO_PERMISSION    -14
#define BOOK_STOCK_INSUFFICIENT -15 // 图书库存不足
// 新增：全局变量 - 记录上次重置日期（格式：YYYYMMDD，比如20260320）
static int lastResetDate = 0;

// 工具函数：获取当前日期（返回YYYYMMDD格式的整数）
static int getCurrentDate() {
    time_t now = time(NULL);
    struct tm *tm_now = localtime(&now);
    // 拼接为YYYYMMDD（比如2026年3月20日 → 20260320）
    return (tm_now->tm_year + 1900) * 10000 + 
           (tm_now->tm_mon + 1) * 100 + 
           tm_now->tm_mday;
}

// 初始化用户列表（新增：初始化lastResetDate）
UserList* initUserList()
{
    UserList* list = malloc(sizeof(UserList));
    if (!list) {
        fprintf(stderr, "内存分配失败：用户列表结构体\n");
        return NULL;
    }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    list->capacity = INIT_USER_CAPACITY;
    
    // 新增：程序启动时，初始化上次重置日期为当天
    lastResetDate = getCurrentDate();
    return list;
}

// 注册用户（无修改）
int registerUser(UserList* userList, const char *username, 
 const char *password, int role)
 {
    if (userList == NULL) {
        fprintf(stderr, "错误：用户列表未初始化，无法注册用户\n");
        return INVALID_INPUT;
    }
    if (username == NULL || *username == '\0') {
        fprintf(stderr, "错误：用户名不能为空，无法注册用户\n");
        return INVALID_INPUT;
    }
    if (password == NULL || *password == '\0') {
        fprintf(stderr, "错误：密码不能为空，无法注册用户\n");
        return INVALID_INPUT;
    }
    if (role != USER && role != ADMIN) {
        fprintf(stderr, "错误：角色必须为%d（管理员）或%d（普通用户），无法注册用户\n", ADMIN, USER);
        return INVALID_INPUT;
    }

    UserNode* current = userList->head;
    while (current) {
        if (strcmp(current->username, username) == 0) {
            fprintf(stderr, "错误：用户名「%s」已存在，无法重复注册\n", username);
            return USER_EXIST;
        }
        current = current->next;
    }

    UserNode* newNode = malloc(sizeof(UserNode));
    if (!newNode) {
        fprintf(stderr, "内存分配失败：用户节点\n");
        return MEM_ERR;
    }
    strncpy(newNode->username, username, USERNAME_MAX_LEN - 1);
    newNode->username[USERNAME_MAX_LEN - 1] = '\0';
    strncpy(newNode->password, password, PASSWORD_MAX_LEN - 1);
    newNode->password[PASSWORD_MAX_LEN - 1] = '\0';
    newNode->role = role;
    newNode->dailyBorrowed = 0;
    newNode->next = NULL;
    newNode->prev = NULL;

    if (userList->head == NULL) {
        userList->head = newNode;
        userList->tail = newNode;
    } else {
        newNode->prev = userList->tail;
        userList->tail->next = newNode;
        userList->tail = newNode;
    }

    userList->size++;
    fprintf(stdout, "成功注册用户「%s」（角色：%s）\n", username, role == ADMIN ? "管理员" : "普通用户");
    return SUCCESS;
}

// 登录函数（无修改）
int login(UserList* userList, const char *username, 
 const char *password, UserNode *loginUser)
 {
    if (userList == NULL) {
        fprintf(stderr, "错误：用户列表未初始化，无法登录\n");
        return INVALID_INPUT;
    }
    if (username == NULL || *username == '\0') {
        fprintf(stderr, "错误：用户名不能为空，无法登录\n");
        return INVALID_INPUT;
    }
    if (password == NULL || *password == '\0') {
        fprintf(stderr, "错误：密码不能为空，无法登录\n");
        return INVALID_INPUT;
    }

    UserNode* current = userList->head;
    while (current) {
        if (strcmp(current->username, username) == 0) {
            if (strcmp(current->password, password) == 0) {
                if (loginUser) {
                    strncpy(loginUser->username, current->username, USERNAME_MAX_LEN - 1);
                    loginUser->username[USERNAME_MAX_LEN - 1] = '\0';
                    strncpy(loginUser->password, current->password, PASSWORD_MAX_LEN - 1);
                    loginUser->password[PASSWORD_MAX_LEN - 1] = '\0';
                    loginUser->role = current->role;
                    loginUser->dailyBorrowed = current->dailyBorrowed;
                    loginUser->prev = NULL;
                    loginUser->next = NULL;
                }
                fprintf(stdout, "用户「%s」登录成功（角色：%s）\n", username, current->role == ADMIN ? "管理员" : "普通用户");
                return SUCCESS;
            } 
            else {
                fprintf(stderr, "错误：用户「%s」密码错误，登录失败\n", username);
                return PWD_ERROR;
            }
        }
        current = current->next;
    }
    fprintf(stderr, "错误：用户名「%s」不存在，登录失败\n", username);
    return USER_NOT_FOUND;
}

// 判断是否为管理员（无修改）
int isAdmin(UserNode user)
{
    return user.role == ADMIN ? 1 : 0;
}

// 销毁用户列表（无修改）
int destroyUserList(UserList* userList)
{
    if(userList == NULL) {
        fprintf(stderr, "错误：用户列表未初始化，无法销毁\n");
        return INVALID_INPUT;
    }
    UserNode* cur=userList->head;
    while(cur){
        UserNode* temp=cur;
        cur=cur->next;
        free(temp);
    }
    // 新增：释放用户列表结构体本身
    free(userList);
    fprintf(stdout, "成功销毁用户列表\n");
    return SUCCESS;
}

// 重置所有用户每日借阅数量（原函数保留，作为底层实现）
int resetDailyBorrowed(UserList* userList)
{
    if(userList == NULL) {
        fprintf(stderr, "错误：用户列表未初始化，无法重置每日借阅数量\n");
        return INVALID_INPUT;
    }
    UserNode* cur=userList->head;
    while(cur){
        cur->dailyBorrowed=0;
        cur=cur->next;
    }
    // 新增：更新上次重置日期为当天
    lastResetDate = getCurrentDate();
    fprintf(stdout, "成功重置所有用户的每日借阅数量（重置日期：%d）\n", lastResetDate);
    return SUCCESS;
}

// 新增：核心函数 - 检查是否跨天，自动重置每日借阅数量
int checkAndResetDailyBorrowed(UserList* userList) {
    if (userList == NULL) {
        fprintf(stderr, "错误：用户列表未初始化，无法检查重置状态\n");
        return INVALID_INPUT;
    }

    int currentDate = getCurrentDate();
    // 如果当前日期 ≠ 上次重置日期 → 跨天了，需要重置
    if (currentDate != lastResetDate) {
        fprintf(stdout, "检测到跨天（上次重置：%d → 当前：%d），自动重置每日借阅数量\n", lastResetDate, currentDate);
        return resetDailyBorrowed(userList);
    }
    // 未跨天，无需操作
    return SUCCESS;
}

// 示例：借阅图书函数（调用checkAndResetDailyBorrowed，你可整合到自己的借阅逻辑中）
int borrowBook(BookHashTable* bookTable, UserList* userList, UserNode* loginUser, const char* isbn) {
    // 第一步：先检查是否跨天，自动重置每日借阅数量
    int ret = checkAndResetDailyBorrowed(userList);
    if (ret != SUCCESS) {
        return ret;
    }

    // 第二步：检查用户当日借阅是否超限（比如最多5本）
    if (loginUser->dailyBorrowed >= 5) {
        fprintf(stderr, "错误：用户「%s」今日借阅已达上限（5本），无法借阅\n", loginUser->username);
        return BORROW_LIMIT_EXCEED;
    }

    // 第三步：检查图书是否可借（你的原有逻辑）
    int borrowable = isBorrowable(bookTable, isbn);
    if (borrowable == 0) {
        fprintf(stderr, "错误：ISBN「%s」无库存，无法借阅\n", isbn);
        return NO_MATCHED_BOOK;
    }

    // 第四步：更新图书已借出数量 + 用户每日借阅数
    BookNode* book = isbnFindBook(bookTable, isbn);
    book->loanedNum++;
    loginUser->dailyBorrowed++;

    fprintf(stdout, "用户「%s」成功借阅ISBN「%s」，今日已借阅%d本\n", loginUser->username, isbn, loginUser->dailyBorrowed);
    return SUCCESS;
}