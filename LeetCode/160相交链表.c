struct ListNode* getIntersectionNode(struct ListNode* headA, struct ListNode* headB) {
    if (headA == NULL || headB == NULL)
    {
        return NULL;
    }
    struct* p1 = headA;
    struct* p2 = headB;
    while (p1 != p2)
    {
        p1 = (p1 == NULL) ? headA : p1->next;
        p2 = (p2 == NULL) ? headB : p2->next;
    }
    return p1;
}
/*个人笔记理解：该题用了双指针法，类比成现实例子就是：两个人分别p1、p2，走不同两条路headA、headB
确保在同级函数体内，能做到速度一致，最终两条路如果有交点，将会相遇，防护：p1、p2分别只在headA、headB走到尽头时走另一条路，以及防传空指针*/
/*
时间复杂度：O(n+m):总共执行n+m次，即p1走完headA路去走headB路
空间复杂度:O(1):新引入p1、p2两个指针变量，内存开销为常量
*/
/*【LeetCode 160 相交链表 - 双指针】
1. 核心算法：双指针法（平行双指针）
2. 现实类比：
   - 两个指针pA/pB = 两个人p1/p2；
   - 两条链表headA/headB = 两条不同长度的路；
   - 核心规则：两人速度一致（每次仅走1步），走完自身路径后立刻走对方路径，总路程相等；
   - 结果：有交点则必然相遇，无交点则同时走到终点（NULL）。
3. 关键防护：
   - 空指针防护：先判断headA/headB是否为NULL，避免空指针访问；
   - 路径补全：pA仅在走完headA（指向NULL）时跳转到headB，pB同理，保证总路程一致。
4. 复杂度分析：
   - 时间复杂度：O(n+m) —— 两个指针最多各走「链表A长度n + 链表B长度m」步，总执行次数为n+m；
   - 空间复杂度：O(1) —— 仅新增pA、pB两个指针变量，内存开销为常数级（不随输入规模变化）。*/