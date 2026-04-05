class Solution{
    public List<List<Interger>> threeSum(int[] num){
        List<List<Interger>> res = new ArrayList<>();
        if(num==null||num.length<=3)
        {
            return res;
        }
        Arrays.soft(nums);

        int n = nums.length;
     for(int i=0;i<n;i++){
         if(nums[i]>0) break;
         if(i>0&&nums[i]==nums[i-1])
         {
             continue;
         }
         int left=i+1;
         int right=n-1;
         while(left<right)
         {
             if(nums[left]+nums[right]+nums[i]==0)
             {
                 res.add(Arrays.asList(nums[i],nums[left],nums[right]));
                 while(left<right&&nums[left]==nums[left+1]) left++;
                 while(left<right&&nums[right]==nums[right-1]) right--;
                 left++;
                 right--;

             }
             else if(nums[left]+nums[right]+nums[i]<0)
             {
                 left++;
             }
             else if (nums[left]+nums[right]+nums[i]>0)
             {
                 right--;
             }

         }
     }
     return res;
    }
}
