class Solution {
    public void setZeroes(int[][] matrix) {
        int m=matrix.length;
        int n=matrix[0].length;
        boolean fstRowZero = false;
        boolean fstColZero = false;
        for(int j = 0 ; j < n ; j++)
        {
            if(matrix[0][j]==0)
            {
                fstRowZero = true;
                break;
            }
        }
        for (int i = 0; i < m; i++) {
            if (matrix[i][0] == 0) {
                 fstColZero = true;
                break;
            }
        }
        for (int i = 1; i < m; i++) {
            for (int j = 1; j < n; j++) {
                if (matrix[i][j] == 0) {
                    matrix[i][0] = 0; // 标记第i行需要置零
                    matrix[0][j] = 0; // 标记第j列需要置零
                }
            }
        }
        for (int i = 1; i < m; i++) {
            if (matrix[i][0] == 0) {
                for (int j = 1; j < n; j++) {
                    matrix[i][j] = 0;
                }
            }
        }

        // 步骤3：根据第一行的标记，置零对应列（从第二列开始）
        for (int j = 1; j < n; j++) {
            if (matrix[0][j] == 0) {
                for (int i = 1; i < m; i++) {
                    matrix[i][j] = 0;
                }
            }
        }

        // 步骤4：处理第一行（如果原本有0）
        if (fstRowZero) {
            for (int j = 0; j < n; j++) {
                matrix[0][j] = 0;
            }
        }

        // 步骤4：处理第一列（如果原本有0）
        if (fstColZero) {
            for (int i = 0; i < m; i++) {
                matrix[i][0] = 0;
            }

    }
}}
/*不是什么难题，容易理解*/