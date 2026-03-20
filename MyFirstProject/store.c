#include "data.h"       
#include "user.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ops.h"

#define MAX_LINE_LEN 512

// 导出图书到文件
int exportBooksToFile(BookHashTable* table, const char *filePath)
{
    if (table == NULL) {
        fprintf(stderr, "错误：哈希表未初始化，无法导出图书\n");
        return INVALID_INPUT;
    }
    if (filePath == NULL || *filePath == '\0') {
        fprintf(stderr, "错误：文件路径不能为空，无法导出图书\n");
        return INVALID_INPUT;
    }
    if (isHashTableEmpty(table) == 1) {
        fprintf(stderr, "错误：哈希表中没有任何图书可导出\n");
        return EMPTY_LIST;
    }

    FILE *file = fopen(filePath, "w");
    if (!file) {
        fprintf(stderr, "错误：无法打开文件「%s」进行写入（权限/路径错误）\n", filePath);
        return UNKNOWN_ERROR;
    }

    int writeFailedCount = 0;
    for (int i = 0; i < table->bucket_count; i++) {
        BookNode* current = table->buckets[i].head;
        while (current) {
            int ret = fprintf(file, "%s,%s,%s,%d,%d\n", 
                    current->isbn, current->title, current->author, 
                    current->stock, current->loanedNum);
            if (ret < 0) {
                fprintf(stderr, "警告：写入ISBN「%s」数据失败，已跳过\n", current->isbn);
                writeFailedCount++;
            }
            current = current->next;
        }
    }

    fclose(file);
    
    if (writeFailedCount > 0) {
        fprintf(stdout, "提示：成功导出图书到「%s」，但有%d条数据写入失败\n", filePath, writeFailedCount);
    } else {
        fprintf(stdout, "成功将所有图书数据导出到「%s」\n", filePath);
    }
    return SUCCESS;
}

// 从文件导入图书
int importBooksFromFile(BookHashTable* table, const char *filePath)
{
    if (table == NULL) {
        fprintf(stderr, "错误：哈希表未初始化，无法导入图书\n");
        return INVALID_INPUT;
    }
    if (filePath == NULL || *filePath == '\0') {
        fprintf(stderr, "错误：文件路径不能为空，无法导入图书\n");
        return INVALID_INPUT;
    }

    FILE *file = fopen(filePath, "r");
    if (!file) {
        fprintf(stderr, "提示：无法打开文件「%s」进行读取（首次运行正常）\n", filePath);
        return SUCCESS; // 首次运行无文件，不算错误
    }

    char line[MAX_LINE_LEN];
    int importedCount = 0;
    int lineNum = 0;

    while (fgets(line, sizeof(line), file)) {
        lineNum++;
        size_t len = strlen(line);
        if (len > 0 && line[len-1] == '\n') {
            line[len-1] = '\0';
        }

        char *isbn = strtok(line, ",");
        char *title = strtok(NULL, ",");
        char *author = strtok(NULL, ",");
        char *stockStr = strtok(NULL, ",");
        char *loanedNumStr = strtok(NULL, ",");

        if (!isbn || !title || !author || !stockStr || !loanedNumStr) {
            fprintf(stderr, "警告：第%d行格式错误，已跳过\n", lineNum);
            continue;
        }

        int stock = atoi(stockStr);
        int loanedNum = atoi(loanedNumStr);
        if (stock < 0 || loanedNum < 0 || loanedNum > stock) {
            fprintf(stderr, "警告：第%d行ISBN「%s」数据非法，已跳过\n", lineNum, isbn);
            continue;
        }

        int result = addBook(table, isbn, title, author, stock, loanedNum);
        if (result == SUCCESS) {
            importedCount++;
        }
    }

    fclose(file);
    if (importedCount > 0) {
        fprintf(stdout, "成功从「%s」导入%d本图书\n", filePath, importedCount);
    }
    return SUCCESS;
}

// 导出用户到文件
int exportUsersToFile(UserList* userList, const char *filePath)
{
    if (userList == NULL) {
        fprintf(stderr, "错误：用户列表未初始化，无法导出用户\n");
        return INVALID_INPUT;
    }
    if (filePath == NULL || *filePath == '\0') {
        fprintf(stderr, "错误：文件路径不能为空，无法导出用户\n");
        return INVALID_INPUT;
    }

    FILE *file = fopen(filePath, "w");
    if (!file) {
        fprintf(stderr, "错误：无法打开文件「%s」进行写入（权限/路径错误）\n", filePath);
        return UNKNOWN_ERROR;
    }

    int writeFailedCount = 0;
    UserNode* current = userList->head;
    while (current) {
        int ret = fprintf(file, "%s,%s,%d,%d\n", 
                current->username, current->password, current->role, current->dailyBorrowed);
        if (ret < 0) {
            fprintf(stderr, "警告：写入用户「%s」数据失败，已跳过\n", current->username);
            writeFailedCount++;
        }
        current = current->next;
    }

    fclose(file);
    
    if (writeFailedCount > 0) {
        fprintf(stdout, "提示：成功导出用户到「%s」，但有%d条数据写入失败\n", filePath, writeFailedCount);
    } else if (userList->head != NULL) {
        fprintf(stdout, "成功将所有用户数据导出到「%s」\n", filePath);
    }
    return SUCCESS;
}

// ========== 修复后的 importUsersFromFile（唯一版本，复用你已有的函数） ==========
int importUsersFromFile(UserList* userList, const char *filePath)
{
    if (userList == NULL) {
        fprintf(stderr, "错误：用户列表未初始化，无法导入用户\n");
        return INVALID_INPUT;
    }
    if (filePath == NULL || *filePath == '\0') {
        fprintf(stderr, "错误：文件路径不能为空，无法导入用户\n");
        return INVALID_INPUT;
    }

    FILE *file = fopen(filePath, "r");
    if (!file) {
        fprintf(stderr, "提示：无法打开文件「%s」进行读取（首次运行正常）\n", filePath);
        return SUCCESS; // 首次运行无文件，不算错误
    }

    char line[MAX_LINE_LEN];
    int importedCount = 0;
    int lineNum = 0;

    while (fgets(line, sizeof(line), file)) {
        lineNum++;
        size_t len = strlen(line);
        if (len > 0 && line[len-1] == '\n') {
            line[len-1] = '\0';
        }

        char *username = strtok(line, ",");
        char *password = strtok(NULL, ",");
        char *roleStr = strtok(NULL, ",");
        char *dailyBorrowedStr = strtok(NULL, ",");

        if (!username || !password || !roleStr || !dailyBorrowedStr) {
            fprintf(stderr, "警告：第%d行格式错误，已跳过\n", lineNum);
            continue;
        }

        int role = atoi(roleStr);
        int dailyBorrowed = atoi(dailyBorrowedStr);
        if (role < 1 || role > 2 || dailyBorrowed < 0) {
            fprintf(stderr, "警告：第%d行用户「%s」数据非法，已跳过\n", lineNum, username);
            continue;
        }

        // 复用你已有的 registerUser 函数（自动查重+创建节点+添加列表）
        int ret = registerUser(userList, username, password, role);
        if (ret == SUCCESS) {
            // 注册成功后，找到该用户并修改 dailyBorrowed（复用 login 的查找逻辑）
            UserNode tempUser;
            login(userList, username, password, &tempUser);
            // 遍历列表找到真实节点
            UserNode* cur = userList->head;
            while (cur) {
                if (strcmp(cur->username, username) == 0) {
                    cur->dailyBorrowed = dailyBorrowed;
                    break;
                }
                cur = cur->next;
            }
            importedCount++;
        }
    }

    fclose(file);
    if (importedCount > 0) {
        fprintf(stdout, "成功从「%s」导入%d个用户\n", filePath, importedCount);
    }
    return SUCCESS;
}