/**
 * Definition for singly-linked list.
 * public class ListNode {
 *     int val;
 *     ListNode next;
 *     ListNode() {}
 *     ListNode(int val) { this.val = val; }
 *     ListNode(int val, ListNode next) { this.val = val; this.next = next; }
 * }
 */
class Solution {
    public ListNode mergeTwoLists(ListNode list1, ListNode list2) {
        if(list1 == null && list2 == null){
            return null;
        }
        if(list1 == null){
            return list2;
        }
        if(list2 == null){
            return list1;
        }
        ListNode dummy = new ListNode();
        ListNode cur=dummy;
        while(list1 !=null && list2 !=null)
        {
            if(list1.val>list2.val)
            {
                cur.next=list2;
                list2=list2.next;

            }
            else
            {
                cur.next=list1;
                list1=list1.next;

            }
            cur=cur.next;
        }
        if(list1 == null){
            cur.next = list2;
        }
        if(list2 == null){
            cur.next = list1;
        }
        return dummy.next;
    }
}
/*笔记：
*定义一个虚拟的哨兵节点，后面就是直接两个指针遍历list1和list2，再接上去的问题了
* 边界处理：
* 1.list1和list2都为空，返回空，list1为空list2非空，返回list2，相反就返回list1
* 2.list1赋值完list2没赋值完，直接把list2剩下的部分连到dummy上，相反同理
* */