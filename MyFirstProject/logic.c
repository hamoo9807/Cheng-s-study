#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ops.h"       // 引入哈希表版的ops.h（包含BookHashTable）
#include "config.h"
#include "user.h"
#include "data.h"
#include "store.h"

// 修复1：移除重复的宏定义
#define MAX_DAILY_BORROW_NUM 5   

// ===================== 错误码（复用+语义微调） =====================
#define SUCCESS          0   // 操作成功
#define ISBN_EXIST       -1  // ISBN已存在（添加书籍失败）
#define ISBN_NOT_FOUND   -2  // ISBN不存在（借阅/还书/删除失败）
#define MEM_ERR          -3  // 内存分配失败
#define EMPTY_LIST       -4  // 哈希表无任何书籍
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
// 新增：补充语义匹配的错误码（区分库存不足和每日上限）
#define BOOK_STOCK_INSUFFICIENT -15 // 图书库存不足

// 借阅图书（修复：权限校验+入参校验+错误码语义）
int isbnBorrowBook(BookHashTable* table, const char *isbn, int borrowNum, UserNode* loginUser)
{
    // 修复2：先校验核心入参是否为NULL（避免段错误）
    if (!table) {
        fprintf(stderr, "错误：图书哈希表未初始化，无法借阅\n");
        return INVALID_INPUT;
    }
    if (!loginUser) {
        fprintf(stderr, "错误：未登录用户，无法借阅\n");
        return USER_NOT_FOUND;
    }
    if (isbn == NULL || *isbn == '\0') {
        fprintf(stderr, "错误：ISBN不能为空，无法借阅\n");
        return INVALID_INPUT;
    }

    // 修复3：权限判断（普通用户role=2才能借阅，补充日志）
    if (loginUser->role != 2) {
        fprintf(stderr, "错误：用户「%s」无借阅权限（仅普通用户可借阅）\n", loginUser->username);
        return NO_PERMISSION;
    }

    // 校验借阅数量
    if (borrowNum <= 0) {
        fprintf(stderr, "错误：借阅数量必须为正数\n");
        return INVALID_INPUT;
    }

    // 查询图书
    BookNode* book = isbnFindBook(table, isbn);
    if (book == NULL) {
        fprintf(stderr, "错误：ISBN「%s」不存在，无法借阅\n", isbn);
        return ISBN_NOT_FOUND;
    }

    // 修复4：区分“库存不足”和“每日上限”错误码
    if (borrowNum > book->stock) {
        fprintf(stderr, "错误：ISBN「%s」可借数量不足（可借%d本，尝试借%d本）\n", isbn, book->stock, borrowNum);
        return BOOK_STOCK_INSUFFICIENT; // 替换BORROW_LIMIT_EXCEED
    }

    // 检查每日借阅上限
    if (loginUser->dailyBorrowed + borrowNum > MAX_DAILY_BORROW_NUM) {
        fprintf(stderr, "错误：用户「%s」借阅失败，超过每日借阅上限（已借%d本，尝试借%d本，上限%d本）\n", 
                loginUser->username, loginUser->dailyBorrowed, borrowNum, MAX_DAILY_BORROW_NUM);
        return BORROW_LIMIT_EXCEED;
    }

    // 更新图书和用户数据
    book->stock -= borrowNum;
    book->loanedNum += borrowNum;
    loginUser->dailyBorrowed += borrowNum;

    fprintf(stdout, "成功借阅ISBN「%s」的图书%d本（剩余可借%d本），用户「%s」今日已借%d本\n", 
            isbn, borrowNum, book->stock, loginUser->username, loginUser->dailyBorrowed);
    return SUCCESS;
}

// 归还图书（修复：入参校验+更新用户dailyBorrowed）
int returnBook(BookHashTable* table, const char *isbn, int returnNum, UserNode* loginUser)
{
    // 修复5：补充核心入参校验
    if (!table) {
        fprintf(stderr, "错误：图书哈希表未初始化，无法归还\n");
        return INVALID_INPUT;
    }
    if (!loginUser) {
        fprintf(stderr, "错误：未登录用户，无法归还\n");
        return USER_NOT_FOUND;
    }
    if (isbn == NULL || *isbn == '\0') {
        fprintf(stderr, "错误：ISBN不能为空，无法归还\n");
        return INVALID_INPUT;
    }

    // 校验归还数量
    if (returnNum <= 0) {
        fprintf(stderr, "错误：归还数量必须为正数\n");
        return INVALID_INPUT;
    }

    // 查询图书
    BookNode* book = isbnFindBook(table, isbn);
    if (book == NULL) {
        fprintf(stderr, "错误：ISBN「%s」不存在，无法归还\n", isbn);
        return ISBN_NOT_FOUND;
    }

    // 校验归还数量不超过已借数量
    if (returnNum > book->loanedNum) {
        fprintf(stderr, "错误：ISBN「%s」归还数量超过已借数量（已借%d本，尝试归还%d本）\n", isbn, book->loanedNum, returnNum);
        return INVALID_INPUT;
    }

    // 修复6：更新用户每日借阅数（核心逻辑缺失）
    if (loginUser->dailyBorrowed >= returnNum) {
        loginUser->dailyBorrowed -= returnNum;
    } else {
        // 容错：避免每日借阅数为负数（比如异常场景）
        loginUser->dailyBorrowed = 0;
    }

    // 更新图书数据
    book->stock += returnNum;
    book->loanedNum -= returnNum;

    fprintf(stdout, "成功归还ISBN「%s」的图书%d本（剩余已借%d本），用户「%s」今日剩余可借%d本\n", 
            isbn, returnNum, book->loanedNum, loginUser->username, MAX_DAILY_BORROW_NUM - loginUser->dailyBorrowed);
    return SUCCESS;
}

// 查询已借出数量（补充入参校验）
int getBorrowedNum(BookHashTable* table, const char *isbn)
{
    if (!table) {
        fprintf(stderr, "错误：图书哈希表未初始化，无法查询已借出数量\n");
        return INVALID_INPUT;
    }
    if (isbn == NULL || *isbn == '\0') {
        fprintf(stderr, "错误：ISBN不能为空，无法查询已借出数量\n");
        return INVALID_INPUT;
    }

    BookNode* book = isbnFindBook(table, isbn);
    if (book == NULL) {
        fprintf(stderr, "错误：ISBN「%s」不存在，无法查询已借出数量\n", isbn);
        return ISBN_NOT_FOUND; // 修复：返回ISBN_NOT_FOUND而非INVALID_INPUT，语义更准
    }
    return book->loanedNum;
}

// 查询总数量（补充入参校验）
int getTotalNum(BookHashTable* table, const char *isbn)
{
    if (!table) {
        fprintf(stderr, "错误：图书哈希表未初始化，无法查询总数量\n");
        return INVALID_INPUT;
    }
    if (isbn == NULL || *isbn == '\0') {
        fprintf(stderr, "错误：ISBN不能为空，无法查询总数量\n");
        return INVALID_INPUT;
    }

    BookNode* book = isbnFindBook(table, isbn);
    if (book == NULL) {
        fprintf(stderr, "错误：ISBN「%s」不存在，无法查询总数量\n", isbn);
        return ISBN_NOT_FOUND;
    }
    return book->stock + book->loanedNum;
}

// 查询可借数量（补充入参校验）
int getAvailableNum(BookHashTable* table, const char *isbn)
{
    if (!table) {
        fprintf(stderr, "错误：图书哈希表未初始化，无法查询可借数量\n");
        return INVALID_INPUT;
    }
    if (isbn == NULL || *isbn == '\0') {
        fprintf(stderr, "错误：ISBN不能为空，无法查询可借数量\n");
        return INVALID_INPUT;
    }

    BookNode* book = isbnFindBook(table, isbn);
    if (book == NULL) {
        fprintf(stderr, "错误：ISBN「%s」不存在，无法查询可借数量\n", isbn);
        return ISBN_NOT_FOUND;
    }
    return book->stock;
}

// 管理员添加图书（修复：入参校验+日志）
int adminAddBook(BookHashTable* table, const char *isbn, const char *title, const char *author, int totalNum, UserNode* loginUser)
{
    // 补充入参校验
    if (!table) {
        fprintf(stderr, "错误：图书哈希表未初始化，无法添加图书\n");
        return INVALID_INPUT;
    }
    if (!loginUser) {
        fprintf(stderr, "错误：未登录用户，无法添加图书\n");
        return USER_NOT_FOUND;
    }
    if (isbn == NULL || *isbn == '\0' || title == NULL || *title == '\0' || author == NULL || *author == '\0') {
        fprintf(stderr, "错误：ISBN/标题/作者不能为空，无法添加图书\n");
        return INVALID_INPUT;
    }
    if (totalNum <= 0) {
        fprintf(stderr, "错误：添加图书数量必须为正数\n");
        return INVALID_INPUT;
    }

    // 权限校验
    if (loginUser->role != 1) {
        fprintf(stderr, "错误：用户「%s」无添加图书权限（仅管理员可操作）\n", loginUser->username);
        return NO_PERMISSION;
    }

    // 调用addBook并补充日志
    int ret = addBook(table, isbn, title, author, totalNum, 0);
    if (ret == SUCCESS) {
        fprintf(stdout, "管理员「%s」成功添加图书：ISBN「%s」，标题「%s」，数量%d本\n", 
                loginUser->username, isbn, title, totalNum);
    } else if (ret == ISBN_EXIST) {
        fprintf(stderr, "错误：管理员「%s」添加图书失败，ISBN「%s」已存在\n", loginUser->username, isbn);
    }
    return ret;
}

// 管理员删除图书（修复：入参校验+日志）
int adminDeleteBook(BookHashTable* table, UserNode* loginUser, const char *isbn)
{
    // 补充入参校验
    if (!table) {
        fprintf(stderr, "错误：图书哈希表未初始化，无法删除图书\n");
        return INVALID_INPUT;
    }
    if (!loginUser) {
        fprintf(stderr, "错误：未登录用户，无法删除图书\n");
        return USER_NOT_FOUND;
    }
    if (isbn == NULL || *isbn == '\0') {
        fprintf(stderr, "错误：ISBN不能为空，无法删除图书\n");
        return INVALID_INPUT;
    }

    // 权限校验
    if (loginUser->role != 1) {
        fprintf(stderr, "错误：用户「%s」无删除图书权限（仅管理员可操作）\n", loginUser->username);
        return NO_PERMISSION;
    }

    // 调用deleteBook并补充日志
    int ret = deleteBook(table, isbn);
    if (ret == SUCCESS) {
        fprintf(stdout, "管理员「%s」成功删除图书：ISBN「%s」\n", loginUser->username, isbn);
    } else if (ret == ISBN_NOT_FOUND) {
        fprintf(stderr, "错误：管理员「%s」删除图书失败，ISBN「%s」不存在\n", loginUser->username, isbn);
    } else if (ret == BOOK_HAS_BORROWED) {
        fprintf(stderr, "错误：管理员「%s」删除图书失败，ISBN「%s」有未归还借阅\n", loginUser->username, isbn);
    }
    return ret;
}