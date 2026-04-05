class Solution {
    public List<Integer> inorderTraversal(TreeNode root) {
        List<Interger> res = new ArrayList<>();/*由题中传入的是数组，故用物理上是数组的顺序表存储*/
        Stack<TreeNode> stack = new Stack<>();/*为模拟系统迭代，申请一个存储树节点的栈，用于遍历*/
        TreeNode cur = root;
        while(cur!=null||!stack.isEmpty())/*防空数组处理，当传入是空树时，此时cur==null且stack.isEmpty*/
        {
            while(cur!=null)/*对应到递归写法就是递归的返回条件*/
            {
                stack.push(cur);
                cur = cur.left;
            }
            cur = stack.pop();/*出栈返回地址赋值给指针，实现返回根节点*/
            res.add(cur.val);
            cur = cur.right;
        }
        return res;
    }
}