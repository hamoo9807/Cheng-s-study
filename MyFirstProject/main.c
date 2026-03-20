#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

// 引入你的所有头文件
#include "user.h"
#include "ops.h"
#include "logic.h"
#include "store.h"
#include "data.h"

// ===================== 控制台彩色输出宏（精美UI） =====================
#define COLOR_RESET   "\033[0m"
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_BLUE    "\033[34m"
#define COLOR_PURPLE  "\033[35m"
#define COLOR_CYAN    "\033[36m"
#define COLOR_GRAY    "\033[90m"

// ===================== 全局变量（你的数据结构） =====================
BookHashTable* g_bookTable = NULL;
UserList* g_userList = NULL;

// ===================== 强制UTF-8编码（解决中文乱码） =====================
void setConsoleUtf8() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
}

// ===================== 清屏（Windows兼容） =====================
void clearScreen() {
    system("cls");
}

// ===================== 暂停等待（精美UI） =====================
void pauseWait() {
    printf(COLOR_GRAY "\n按任意键继续..." COLOR_RESET);
    getchar();
    getchar(); // 吸收换行符
}

// ===================== 打印分隔线（精美UI） =====================
void printSeparator() {
    printf(COLOR_CYAN "========================================\n" COLOR_RESET);
}

// ===================== 打印标题（精美UI） =====================
void printTitle(const char* title) {
    clearScreen();
    printSeparator();
    printf(COLOR_YELLOW "          %s\n" COLOR_RESET, title);
    printSeparator();
}

// ===================== 确保data文件夹存在 =====================
void ensureDataDir() {
    system("md data 2>nul");
}

// ===================== 打印书表的特殊实现（导出+Live Server+HTML） =====================
void printBookListWithLiveServer() {
    printTitle("📚 图书列表（Live Server预览）");
    
    // 1. 导出书籍到CSV
    ensureDataDir();
    int ret = exportBooksToFile(g_bookTable, "data/books.csv");
    if (ret != SUCCESS) {
        printf(COLOR_RED "❌ 导出书籍数据失败！\n" COLOR_RESET);
        pauseWait();
        return;
    }
    
    // 2. 打开Live Server（如果已安装）
    printf(COLOR_GREEN "✅ 书籍数据已导出到 data/books.csv\n" COLOR_RESET);
    printf(COLOR_CYAN "💡 正在打开Live Server和HTML文件...\n" COLOR_RESET);
    
    // 3. 打开index.html（系统默认浏览器）
    system("start index.html");
    
    printf(COLOR_GREEN "✅ 已打开HTML文件！\n" COLOR_RESET);
    printf(COLOR_GRAY "提示：如果Live Server未启动，请手动在VS Code中打开index.html并启动Live Server\n" COLOR_RESET);
    pauseWait();
}

// ===================== 管理员菜单 =====================
void adminMenu(UserNode* loginUser) {
    while (1) {
        printTitle("👨‍💼 管理员菜单");
        printf(COLOR_CYAN "【1】添加图书\n");
        printf("【2】删除图书\n");
        printf("【3】修改图书库存\n");
        printf("【4】查看图书列表（Live Server预览）\n");
        printf("【5】导出所有数据\n");
        printf("【6】重置所有用户每日借阅数\n");
        printf("【0】退出登录\n" COLOR_RESET);
        printSeparator();
        printf(COLOR_YELLOW "请选择操作：" COLOR_RESET);
        
        int choice;
        scanf("%d", &choice);
        getchar(); // 吸收换行
        
        if (choice == 0) break;
        
        char isbn[32], title[128], author[128];
        int num, ret;
        
        switch (choice) {
            case 1:
                // 添加图书
                printTitle("📝 添加图书");
                printf("请输入ISBN：");
                scanf("%s", isbn);
                getchar();
               // 修复后的安全代码
                printf("请输入书名：");
                fgets(title, sizeof(title), stdin);
                // 去掉 fgets 读入的换行符
                title[strcspn(title, "\n")] = '\0';

                 printf("请输入作者：");
                 fgets(author, sizeof(author), stdin);
                author[strcspn(author, "\n")] = '\0';
                printf("请输入库存数量：");
                scanf("%d", &num);
                
                ret = adminAddBook(g_bookTable, isbn, title, author, num, loginUser);
                if (ret == SUCCESS) printf(COLOR_GREEN "\n✅ 添加成功！\n" COLOR_RESET);
                else if (ret == ISBN_EXIST) printf(COLOR_RED "\n❌ ISBN已存在！\n" COLOR_RESET);
                else printf(COLOR_RED "\n❌ 添加失败（错误码：%d）\n" COLOR_RESET, ret);
                pauseWait();
                break;
                
            case 2:
                // 删除图书
                printTitle("🗑️ 删除图书");
                printf("请输入要删除的ISBN：");
                scanf("%s", isbn);
                
                ret = adminDeleteBook(g_bookTable, loginUser, isbn);
                if (ret == SUCCESS) printf(COLOR_GREEN "\n✅ 删除成功！\n" COLOR_RESET);
                else if (ret == ISBN_NOT_FOUND) printf(COLOR_RED "\n❌ ISBN不存在！\n" COLOR_RESET);
                else if (ret == BOOK_HAS_BORROWED) printf(COLOR_RED "\n❌ 该图书有未归还借阅，无法删除！\n" COLOR_RESET);
                else printf(COLOR_RED "\n❌ 删除失败（错误码：%d）\n" COLOR_RESET, ret);
                pauseWait();
                break;
                
            case 3:
                // 修改库存
                printTitle("📊 修改库存");
                printf("请输入ISBN：");
                scanf("%s", isbn);
                printf("请输入新库存：");
                scanf("%d", &num);
                
                ret = updateStock(g_bookTable, isbn, num);
                if (ret == SUCCESS) printf(COLOR_GREEN "\n✅ 库存修改成功！\n" COLOR_RESET);
                else if (ret == ISBN_NOT_FOUND) printf(COLOR_RED "\n❌ ISBN不存在！\n" COLOR_RESET);
                else printf(COLOR_RED "\n❌ 修改失败（错误码：%d）\n" COLOR_RESET, ret);
                pauseWait();
                break;
                
            case 4:
                // 查看图书列表（Live Server预览）
                printBookListWithLiveServer();
                break;
                
            case 5:
                // 导出所有数据
                printTitle("💾 导出数据");
                ensureDataDir();
                exportBooksToFile(g_bookTable, "data/books.csv");
                exportUsersToFile(g_userList, "data/users.csv");
                printf(COLOR_GREEN "\n✅ 所有数据已导出到 data/ 文件夹！\n" COLOR_RESET);
                pauseWait();
                break;
                
            case 6:
                // 重置所有用户每日借阅数
                printTitle("🔄 重置借阅数");
                ret = resetDailyBorrowed(g_userList);
                if (ret == SUCCESS) printf(COLOR_GREEN "\n✅ 已重置所有用户每日借阅数！\n" COLOR_RESET);
                pauseWait();
                break;
                
            default:
                printf(COLOR_RED "\n❌ 无效选择！\n" COLOR_RESET);
                pauseWait();
                break;
        }
    }
}

// ===================== 普通用户菜单 =====================
void userMenu(UserNode* loginUser) {
    while (1) {
        printTitle("👤 普通用户菜单");
        printf(COLOR_CYAN "【1】借阅图书\n");
        printf("【2】归还图书\n");
        printf("【3】查看图书列表（Live Server预览）\n");
        printf("【4】查看今日已借数量\n");
        printf("【0】退出登录\n" COLOR_RESET);
        printSeparator();
        printf(COLOR_YELLOW "请选择操作：" COLOR_RESET);
        
        int choice;
        scanf("%d", &choice);
        getchar();
        
        if (choice == 0) break;
        
        char isbn[32];
        int num, ret;
        
        switch (choice) {
            case 1:
                // 借阅图书
                printTitle("📖 借阅图书");
                // 先检查跨天重置
                checkAndResetDailyBorrowed(g_userList);
                
                printf("请输入ISBN：");
                scanf("%s", isbn);
                printf("请输入借阅数量：");
                scanf("%d", &num);
                
                ret = isbnBorrowBook(g_bookTable, isbn, num, loginUser);
                if (ret == SUCCESS) printf(COLOR_GREEN "\n✅ 借阅成功！\n" COLOR_RESET);
                else if (ret == ISBN_NOT_FOUND) printf(COLOR_RED "\n❌ ISBN不存在！\n" COLOR_RESET);
                else if (ret == BORROW_LIMIT_EXCEED) printf(COLOR_RED "\n❌ 超过每日借阅上限！\n" COLOR_RESET);
                else if (ret == BOOK_STOCK_INSUFFICIENT) printf(COLOR_RED "\n❌ 库存不足！\n" COLOR_RESET);
                else printf(COLOR_RED "\n❌ 借阅失败（错误码：%d）\n" COLOR_RESET, ret);
                pauseWait();
                break;
                
            case 2:
                // 归还图书
                printTitle("📚 归还图书");
                printf("请输入ISBN：");
                scanf("%s", isbn);
                printf("请输入归还数量：");
                scanf("%d", &num);
                
                ret = returnBook(g_bookTable, isbn, num, loginUser);
                if (ret == SUCCESS) printf(COLOR_GREEN "\n✅ 归还成功！\n" COLOR_RESET);
                else if (ret == ISBN_NOT_FOUND) printf(COLOR_RED "\n❌ ISBN不存在！\n" COLOR_RESET);
                else printf(COLOR_RED "\n❌ 归还失败（错误码：%d）\n" COLOR_RESET, ret);
                pauseWait();
                break;
                
            case 3:
                // 查看图书列表（Live Server预览）
                printBookListWithLiveServer();
                break;
                
            case 4:
                // 查看今日已借
                printTitle("📊 今日借阅情况");
                printf(COLOR_CYAN "你今日已借阅：%d 本（上限5本）\n" COLOR_RESET, loginUser->dailyBorrowed);
                printf(COLOR_CYAN "今日剩余可借：%d 本\n" COLOR_RESET, 5 - loginUser->dailyBorrowed);
                pauseWait();
                break;
                
            default:
                printf(COLOR_RED "\n❌ 无效选择！\n" COLOR_RESET);
                pauseWait();
                break;
        }
    }
}

// ===================== 登录功能 =====================
int doLogin(UserNode* loginUser) {
    printTitle("🔐 用户登录");
    char username[USERNAME_MAX_LEN], password[PASSWORD_MAX_LEN];
    printf("请输入用户名：");
    scanf("%s", username);
    printf("请输入密码：");
    scanf("%s", password);
    
    int ret = login(g_userList, username, password, loginUser);
    if (ret == SUCCESS) {
        printf(COLOR_GREEN "\n✅ 登录成功！\n" COLOR_RESET);
        Sleep(800);
        return 1;
    } else if (ret == USER_NOT_FOUND) {
        printf(COLOR_RED "\n❌ 用户名不存在！\n" COLOR_RESET);
    } else if (ret == PWD_ERROR) {
        printf(COLOR_RED "\n❌ 密码错误！\n" COLOR_RESET);
    } else {
        printf(COLOR_RED "\n❌ 登录失败（错误码：%d）\n" COLOR_RESET, ret);
    }
    pauseWait();
    return 0;
}

// ===================== 注册功能 =====================
void doRegister() {
    printTitle("📝 用户注册");
    char username[USERNAME_MAX_LEN], password[PASSWORD_MAX_LEN];
    int role;
    printf("请输入用户名：");
    scanf("%s", username);
    printf("请输入密码：");
    scanf("%s", password);
    printf("请选择角色（1=管理员/2=普通用户）：");
    scanf("%d", &role);
    
    int ret = registerUser(g_userList, username, password, role);
    if (ret == SUCCESS) {
        printf(COLOR_GREEN "\n✅ 注册成功！\n" COLOR_RESET);
    } else if (ret == USER_EXIST) {
        printf(COLOR_RED "\n❌ 用户名已存在！\n" COLOR_RESET);
    } else {
        printf(COLOR_RED "\n❌ 注册失败（错误码：%d）\n" COLOR_RESET, ret);
    }
    pauseWait();
}

// ===================== 主函数（程序入口） =====================
int main() {
    // 初始化
    setConsoleUtf8();
    ensureDataDir();
    clearScreen();
    
    // 初始化你的数据结构
    g_bookTable = initHashTable();
    g_userList = initUserList();
    
    // 导入初始数据
    importBooksFromFile(g_bookTable, "data/books.csv");
    importUsersFromFile(g_userList, "data/users.csv");
    
    // 主菜单
    UserNode loginUser;
    memset(&loginUser, 0, sizeof(UserNode));
    
    while (1) {
        printTitle("📚 图书管理系统 v1.0");
        printf(COLOR_CYAN "【1】登录\n");
        printf("【2】注册\n");
        printf("【0】退出程序\n" COLOR_RESET);
        printSeparator();
        printf(COLOR_YELLOW "请选择操作：" COLOR_RESET);
        
        int choice;
        scanf("%d", &choice);
        getchar();
        
        switch (choice) {
            case 1:
                // 登录
                if (doLogin(&loginUser)) {
                    // 根据角色进入不同菜单
                    if (loginUser.role == 1) {
                        adminMenu(&loginUser);
                    } else {
                        userMenu(&loginUser);
                    }
                    // 退出登录后清空
                    memset(&loginUser, 0, sizeof(UserNode));
                }
                break;
                
            case 2:
                // 注册
                doRegister();
                break;
                
            case 0:
                // 退出程序，保存数据
                printTitle("👋 退出程序");
                ensureDataDir();
                exportBooksToFile(g_bookTable, "data/books.csv");
                exportUsersToFile(g_userList, "data/users.csv");
                destroyHashTable(g_bookTable);
                destroyUserList(g_userList);
                printf(COLOR_GREEN "✅ 数据已保存，程序已退出！\n" COLOR_RESET);
                return 0;
                
            default:
                printf(COLOR_RED "\n❌ 无效选择！\n" COLOR_RESET);
                pauseWait();
                break;
        }
    }
    
    return 0;
}