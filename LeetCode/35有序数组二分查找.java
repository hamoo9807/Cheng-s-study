class Solution {
    public int searchInsert(int[] nums, int target) {
        // 左指针：初始指向数组第一个元素
        int left = 0;
        // 右指针：初始指向数组最后一个元素
        int right = nums.length - 1;
        
        // 二分查找核心循环：缩小区间，直到左指针超过右指针
        while (left <= right) {
            // 计算中间索引（避免 left+right 溢出，等价于 (left+right)/2）
            int mid = left + (right - left) / 2;
            
            if (nums[mid] == target) {
                // 找到目标值，直接返回索引
                return mid;
            } else if (nums[mid] < target) {
                // 目标值在右侧，左指针右移
                left = mid + 1;
            } else {
                // 目标值在左侧，右指针左移
                right = mid - 1;
            }
        }
        
        // 循环结束时，left 就是插入位置（核心结论）
        return left;
    }
}