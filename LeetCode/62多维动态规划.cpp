class Solution {
public:
    int uniquePaths(int m, int n) {
        vector<vector<int>> dp(m,vector(n,0));//m为列，n为行
        //边界初始化
        for(int i=0;i<m;++i) dp[i][0] = 1;
        for(int j=0;j<n;++j) dp[0][j] = 1;
        for(int i=1;i<m;++i){
            for(int j=1;j<n;++j){
                dp[i][j]=dp[i-1][j]+dp[i][j-1];//十分容易理解，当前可以分解为左一个向右前进一步和上一格向下前进一步
            }
        }
        return dp[m-1][n-1];
    }
};