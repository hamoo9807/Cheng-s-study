void merge(int* nums1, int nums1Size, int m, int* nums2, int nums2Size, int n) {
    int p1 = m - 1;
    int p2 = n - 1;
    int p = m + n - 1;
    while (p1 >= 0 && p2 >= 0)
    {
        if (nums1[p1] > nums2[p2])
        {
            nums1[p] = nums1[p1];
            p1--;
        }
        else
        {
            nums1[p] = nums2[p2];
            p2--;
        }
        p--;
    }
    while (p2 >= 0)
    {
        nums1[p] = nums2[p2];
        p2--;
        p--;
    }
}

/*
个人笔记：
尝试正向指针分别对比时，会发现每次覆盖都需要将整体移位，麻烦且低效
逆向指针覆写，可以完美利用题中末尾项为空白项
故用三个指针：p1尾指针、p2尾指针、p尾指针
为何用尾指针？题中是确保非递减，即递增序列，从最大值开始比较覆写可满足需求
最后边界处理：
Q1:p1覆写完了p2没覆写完？ A1:分析p1覆写完了 剩下的p2都是小于p1最小项的，加入边界处理，即直接在p标记指针继续覆写直到p2写完
Q2:p2覆写完了p1没覆写完？ A2:分析p2覆写完，即数组已填满，剩下的p1项都是遵循递增序列，无需额外处理
*/