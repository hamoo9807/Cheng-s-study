class Solution {
    public void moveZeroes(int[] nums) {
        int np=0;
        for(int i=0;i<nums.length;i++)
        {
           if(nums[i]!=0)
           {
            nums[np]=nums[i];
            np++;
           }
        }
        for(;np<nums.length;np++)
        {
            nums[np]=0;
        }
    }
    }        
/*时间复杂度一致的解法很多，最省时间的做法就是非零前移，后面的全部赋值0*/