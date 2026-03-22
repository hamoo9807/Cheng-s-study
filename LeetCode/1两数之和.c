/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
#define HASH_SIZE 20011 //根据取值范围定义哈希容量，且遵循质数原则、略大原则、非二次幂原则（避免高位信息丢失）
#define EMPTY -1
int getHashKey(int num)/*取键值函数：哈希表的数组下标即键值，存入哈希表的值即键值*/
{
    int result = (num + 10000) % HASH_SIZE;/*题中取值范围为-1e04~1e04，将数值映射为合法非负下标，负数取模可能为负，需补偿”*/
    return result < 0 ? result + HASH_SIZE : result;
}

int* twoSum(int* nums, int numsSize, int target, int* returnSize) {
    *returnSize = 2;
    int* result = (int*)malloc(2 * sizeof(int));
    int hash[HASH_SIZE];
    for (int i = 0; i < HASH_SIZE; i++)
    {
        hash[i] = EMPTY;
    }
    for (int i = 0; i < numsSize; i++)
    {
        int compelement = target - nums[i];//取补数
        int key = getHashKey(compelement);//取补数的键值
        
        if (hash[key] != EMPTY)
        {
            result[0] = hash[key];
            result[1] = i;
            return result; 
        }

       int curKey = getHashKey(nums[i]);//补数未在哈希表上查找到，计算 curKey
        hash[curKey] = i;//存入哈希表，i为哈希索引
    }
    return NULL;
}