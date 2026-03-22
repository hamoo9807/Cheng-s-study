class Solution {
    public int climbStairs(int n) {
        if(n<=2)
        {
            return n; //n=1，f(n)=1，n=2，f(n)=2；f(n)=n；
        }
        int[] method = {1,2};//存储走一步、走两步的方法
        for(int i = 3 ; i <= n ; i++ )
        {
            method[1]=method[0]+method[1];
            method[0]=method[1]-method[0];//不用cur，只进行累加动态更新，最新method[1]就是指向cur 
        }
        return method[1];//返回cur的方法数
    }
}