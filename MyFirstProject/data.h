#ifndef DATA_H
#define DATA_H
#include <stdlib.h>
#include <string.h>

typedef struct BookNode{
	char *isbn;
	char *title;
	char *author;
	int stock;
	int loanedNum;
	struct BookNode* next;
	struct BookNode* prev;
}BookNode;

typedef struct BookList{
	BookNode* head;
	BookNode* tail;	
	int length;
}BookList;

typedef struct BookHashTable {
    BookList* buckets;  // 哈希桶数组
    int bucket_count;     // 哈希桶数量（建议设为质数，如101、211）
    int total_count;      // 所有书籍的总数量
} BookHashTable;

#endif
	
