#include <iostream>
#include <vector>
#include <string>
using namespace std;

/**
 * @brief 完全对应你给的Python build_next逻辑，构建0起始版next数组
 * @param patt 模式串
 * @return vector<int> next数组，next[i]表示子串patt[0..i]的最长相等前后缀长度
 */
vector<int> build_next(const string& patt) {
    int n = patt.size();
    vector<int> next(n, 0);  // next[0]固定为0，初始化全0
    int prefix_len = 0;      // prefix 前缀，0初始长度
    int i = 1;                // 从第2个字符（索引1）开始计算

    while (i < n) {
        if (patt[prefix_len] == patt[i]) {
            // 当前前缀字符与当前字符匹配：前缀长度增加，next值更新
            prefix_len++;
            next[i] = prefix_len;//存前缀长度
            i++;
        } else {
            if (prefix_len == 0) {
                // 无可用前后缀：next值为0，i前进
                next[i] = 0;
                i++;
            } else {
                // 回退：如现有ABCD__ABCA，A不等D，直接看ABC中的C的值
                prefix_len = next[prefix_len - 1];//查表，看ABC中C的值
            }
        }
    }
    return next;
}

/**
 * @brief KMP匹配函数：在主串s中查找所有模式串p的匹配位置
 * @param s 主串（待搜索的文本）
 * @param p 模式串（要查找的字符串）
 * @return vector<int> 所有匹配的起始下标（从0开始）
 */
vector<int> kmp_search(const string& s, const string& p) {
    vector<int> next = build_next(p);  // 构建next数组
    vector<int> res;                   // 存储匹配结果
    int i = 0;  // 主串s的指针（永不回退）
    int j = 0;  // 模式串p的指针（按next数组回跳）

    while (i < s.size()) {
        if (s[i] == p[j]) {
            // 字符匹配：双指针同步前进
            i++;
            j++;
            if (j == p.size()) {
                // 找到完整匹配：记录起始下标（i - 模式串长度）
                res.push_back(i - p.size());
                // 回跳模式串指针，继续查找下一个匹配
                j = next[j - 1];
            }
        } else {
            if (j == 0) {
                // 模式串指针已在开头：主串指针前进
                i++;
            } else {
                // 模式串指针按next数组回跳，主串指针不回退（KMP核心优化）
                j = next[j - 1];
            }
        }
    }
    return res;
}

// 测试用例
int main() {
    // 你题目中的模式串
    string patt = "abcaabbabcab";
    // 测试主串（包含2个匹配项）
    string s = "xabcabcaabbabcabyabcaabbabcabz";

    // 1. 打印next数组
    vector<int> next = build_next(patt);
    cout << "模式串: " << patt << endl;
    cout << "0起始next数组: [";
    for (size_t i = 0; i < next.size(); i++) {
        if (i > 0) cout << ", ";
        cout << next[i];
    }
    cout << "]" << endl;

    // 2. 执行KMP匹配
    vector<int> match_pos = kmp_search(s, patt);
    cout << "\n主串: " << s << endl;
    cout << "匹配的起始下标: [";
    for (size_t i = 0; i < match_pos.size(); i++) {
        if (i > 0) cout << ", ";
        cout << match_pos[i];
    }
    cout << "]" << endl;

    return 0;
}
/*
简洁的话指出KMP算法：1.BF基础上加入next，为何要next？目的是找收尾相同数，如ABC____ABC，相同数为3，就意味着前三位是不需要重复判断的，省去重新定位的步骤；
那么知道原理了，next如何编写？核心边界处理：
1.next[0]如何处理？只有一个元素，显然，为0，或者教科书上的-1（错误，或者说独特处理），那么开始记录子串每一个元素的next值，next[1]，对比0和1的值，看是否同前后缀名，简单的就不说，
2.当cur与首子串字符相等，cur++，prefix（前缀）++，
3.不同怎么处理：两种情况：
1.前缀重复数为0，很明显，直接移位即可，因为以及确定没重复，
2.不为0，回退？为何要回退，如现有一个不为零的情况，ABAC__ABA，明显为3，下一位设D，ABAC_ABAD，
这里涉及到了一次回溯，为何要回溯？？？ABA 很明显已有这个结构是吧 ABA____ABA 无论情况多差 都已知前面有个A，AD，那么再换个更直接的说法，
已知head-----head_____head------head， 那么最差也是head开始，所以回溯的意义体现出来了，取前缀的next值。
那么到main函数，调用这个next，饶了一大圈，回到1.BF基础上加入next，为何要next？目的是找收尾相同数，如ABC____ABC，相同数为3，就意味着前三位是不需要重复判断的，省去重新定位的步骤；那么知道原理了，next如何编写？ 
那么需要两个cur，一个cur1，一个cur2，cur1遍历主串，cur2遍历子串，
最笼统两个处理：是不是相同的->
是就直接同时前进，
不是？开始调用next，查cur1-1的值，赋予给cur2，并让cur2重新判断当前值，
这时next会发挥它的作用，前面没有前后缀相同的，变成0,0就说明这一串压根没相同的了，包括当前，那么cur1++


*/


/*
一、为什么要用 next 数组？（核心目的）
BF 算法匹配失败会主串 + 模式串一起回退，重复对比浪费时间；next 数组就是提前算出模式串每一位的「最长相等前后缀长度」例：ABC____ABC 首尾相同长度 = 3 → 匹配失败时，前 3 位不用重新比，直接跳过，主串指针永不回退。
二、next 数组怎么写？（核心规则）
边界：next[0] = 0（单个字符无前后缀，固定为 0）
字符匹配：当前前缀字符 = 当前后缀字符
→ 前后缀长度 + 1，记录 next 值，继续往后
字符不匹配（关键）：
若前后缀长度 = 0 → 无重合，直接往后遍历
若前后缀长度≠0 → 回溯
 回溯原因：前面已经有重合的小结构（如ABA____ABA），不用一步步退，直接查历史记录跳转到最优位置
 回溯方法：前缀长度 = next[前缀长度-1]
三、KMP 匹配流程（双指针 + next）
两个指针：i（主串，只进不退）、j（模式串）
字符相同：i++、j++
字符不同：
j=0 → 无重合，i++
j≠0 → j = next[j-1]（跳 next，复用前后缀，跳过重复对比）
j走完模式串 → 匹配成功，记录位置，继续查找

*/