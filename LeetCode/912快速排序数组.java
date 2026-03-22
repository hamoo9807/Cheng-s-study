import java.util.Random;

class Solution {
    // 全局随机数对象，避免频繁创建
    private static final Random random = new Random();

    public int[] sortArray(int[] nums) {
        // 边界处理：空数组或单元素数组直接返回
        if (nums == null || nums.length <= 1) {
            return nums;
        }
        // 递归快排，处理整个数组
        quickSort(nums, 0, nums.length - 1);
        return nums;
    }

    // 递归分治核心方法
    private void quickSort(int[] nums, int left, int right) {
        // 终止条件：子数组长度 <= 1
        if (left >= right) {
            return;
        }
        // 分区获取基准索引，拆分左右子数组
        int pivotIdx = partition(nums, left, right);
        quickSort(nums, left, pivotIdx - 1);
        quickSort(nums, pivotIdx + 1, right);
    }

    // 分区核心（随机基准+双向扫描）
    private int partition(int[] nums, int left, int right) {
        // 1. 随机选基准，交换到左边界
        int randIdx = left + random.nextInt(right - left + 1);
        swap(nums, left, randIdx);
        int pivot = nums[left];

        // 2. 双指针扫描
        int i = left + 1, j = right;
        while (true) {
            // 左指针找 >= 基准的数
            while (i <= j && nums[i] < pivot) {
                i++;
            }
            // 右指针找 <= 基准的数
            while (i <= j && nums[j] > pivot) {
                j--;
            }
            // 指针交叉，终止扫描
            if (i > j) {
                break;
            }
            // 交换违规数，指针继续移动
            swap(nums, i++, j--);
        }

        // 3. 基准归位（核心）
        swap(nums, left, j);
        return j;
    }

    // 交换元素
    private void swap(int[] nums, int i, int j) {
        if (i == j) {
            return;
        }
        int temp = nums[i];
        nums[i] = nums[j];
        nums[j] = temp;
    }
}
/*感慨：很难，分区分治学了一个小时才学明白*/