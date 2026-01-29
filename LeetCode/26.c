#include <stdio.h>
#include <stdlib.h>
int removeDuplicates(int* nums,int numsSize)
{
	if (numsSize == 0)
	{
		printf("empty nums!please reinput the correct nums\n");
		return numsSize;
	}
	int tail = 1, cur = 1;
	//tail is the tail of the logical array
	//cur is the current index of the physical array
	while (cur < numsSize)
	{
		if (nums[cur] != nums[cur- 1])
		{
			nums[tail] = nums[cur];
			tail++;
		}
		cur++;
	}
	return tail;
}
int* createNums(int num)
{
	if (num <= 0)
	{
		printf("error num!please reinput the correct num\n");
		return NULL;
	}
	int* newNums = (int*)malloc(num * sizeof(int));
	if (newNums == NULL)
	{
		printf("no memory to malloc nums!\n");
		return NULL;
	}
	printf("now input the num in nums[]\n");
	for (int i = 0; i < num; i++)
	{
		scanf("%d", &newNums[i]);
	}
	return newNums;
}
void freeNums(int* nums)
{
	if (nums == NULL)
		return;
	free(nums);
	nums = NULL;
}
int main()
{
	int* nums = createNums(5);
	if (nums == NULL)
	{
		printf("create nums failed!\n");
		return -1;
	}
	
	int newSize = removeDuplicates(nums, 5);
	
	printf("after remove duplicates, nums is: ");
	for (int i = 0; i < newSize; i++)
	{
		printf("%d ", nums[i]);
	}
	printf("\n");

	freeNums(nums);
	return 0;
}