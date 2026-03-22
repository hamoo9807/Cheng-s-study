class Solution {
    public int removeElement(int[] nums, int val) {
        if(nums==null||nums.length==0)
        {
            return 0;
        }
        int head=0,tail=nums.length-1;
        while(head<=tail)
        {
            if(nums[head]!=val)
            {
                head++;
            }
            else
            {
                while(head<=tail&&nums[tail]==val)
                {
                    tail--;
                }
                if(head<=tail)
                {
                    nums[head]=nums[tail];
                    head++;
                    tail--;
                }
            }
        }
        return head;
    }
}
/*思路：题目需要移除元素，但只需要物理上置尾元素即可，故可以选择头尾指针，（尾部也需要进行遍历，遇到val值不处理，减少交换次数），头指针遇到val时，头尾进行交换，头指针++，尾指针--，执行条件：head<=tail*/