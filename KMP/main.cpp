/*
program:    KMP算法
author：    xiao han
time:       2017-10-25 11:20
use:        字符串匹配
领域：      字符串匹配
领域相关：  后缀树 前缀树 DFA（有限确定自动机） BM算法
*/
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <cstdlib>
#include <winsock.h>
using namespace std;



void get_next(char pattern[],int next[])
{
    //next[0] = sizeof(next) / sizeof(int);
    next[0] = 0;
    for(int k = 0,p = 1;p <= strlen(pattern);p++)
    {
        while( k > 0 && pattern[p] != pattern[k])
            k = next[k - 1];
        if(pattern[p] == pattern[k])
            k++;
        next[p] = k;
    }
}
void makeNext(const char P[],int next[])
{
    int q,k;//q:模版字符串下标；k:最大前后缀长度
    int m = strlen(P);//模版字符串长度
    next[0] = 0;//模版字符串的第一个字符的最大前后缀长度为0
    for (q = 1,k = 0; q < m; ++q)//for循环，从第二个字符开始，依次计算每一个字符对应的next值
    {
        while(k > 0 && P[q] != P[k])//递归的求出P[0]···P[q]的最大的相同的前后缀长度k
            k = next[k-1];          //不理解没关系看下面的分析，这个while循环是整段代码的精髓所在，确实不好理解
        if (P[q] == P[k])//如果相等，那么最大相同前后缀长度加1
        {
            k++;
        }
        next[q] = k;
    }
}

int kmp(char *str,char *pattern,int *next)
{
    int k = -1;
    for(int i = 0;i < strlen(str);i++)
    {
        while( k > -1 &&str[i] != pattern[k + 1])
            k = next[k];
        if(str[i] == pattern[k])
            k++;
        if(k == strlen(pattern) - 1)
        {
            return i - strlen(pattern) + 1;
        }
    }
    return -1;
}

int main()
{
    char *str="acabaabaabcacaabc";
    char *pattern="abaabcac";
    int *next = (int *)malloc(sizeof(int)*strlen(pattern));
    //next = new int[strlen(pattern)];
    cout<<strlen(pattern)<<endl;
    get_next(pattern,next);
    //makeNext(pattern,next);
    int pos = kmp(str,pattern,next);
    cout<<pos<<endl;
    return 0;
}

/*
1.字符串数组初始化，分配空间在全局数据区或者栈区，可以对其内容进行修改；字符串指针指向常量区，没有开辟字符串数组，所以其指向的内容没有修改的权限。
例子：char charr[] = "_test_strcat_redundance"; char* str="zifuchuanshuzu"；
如果要对字符串操作，用字符串处理函数：strcpy strcat等，不支持 ‘=’ 对字符串直接赋值。
2.compare: next分别从 -1,0 开始
(1)
void cal_next(char *str, int *next, int len)
{
    next[0] = -1;//next[0]初始化为-1，-1表示不存在相同的最大前缀和最大后缀
    int k = -1;//k初始化为-1
    for (int q = 1; q <= len-1; q++)
    {
        while (k > -1 && str[k + 1] != str[q])//如果下一个不同，那么k就变成next[k]，注意next[k]是小于k的，无论k取任何值。
        {
            k = next[k];//往前回溯
        }
        if (str[k + 1] == str[q])//如果相同，k++
        {
            k = k + 1;
        }
        next[q] = k;//这个是把算的k的值（就是相同的最大前缀和最大后缀长）赋给next[q]
    }
}
int KMP(char *str, int slen, char *ptr, int plen)
{
    int *next = new int[plen];
    cal_next(ptr, next, plen);//计算next数组
    int k = -1;
    for (int i = 0; i < slen; i++)
    {
        while (k >-1&& ptr[k + 1] != str[i])//ptr和str不匹配，且k>-1（表示ptr和str有部分匹配）
            k = next[k];//往前回溯
        if (ptr[k + 1] == str[i])
            k = k + 1;
        if (k == plen-1)//说明k移动到ptr的最末端
        {
            //cout << "在位置" << i-plen+1<< endl;
            //k = -1;//重新初始化，寻找下一个
            //i = i - plen + 2;//i定位到找到位置处的下一个位置（这里默认存在两个匹配字符串可以部分重叠）
            return i-plen+1;//返回相应的位置
        }
    }
    return -1;
}
(2)
void makeNext(const char P[],int next[])
{
    int q,k;
    int m = strlen(P);
    next[0] = 0;
    for (q = 1,k = 0; q < m; ++q)
    {
        while(k > 0 && P[q] != P[k])
            k = next[k-1];
        if (P[q] == P[k])
        {
            k++;
        }
        next[q] = k;
    }
}

int kmp(const char T[],const char P[],int next[])
{
    int n,m;
    int i,q;
    n = strlen(T);
    m = strlen(P);
    makeNext(P,next);
    for (i = 0,q = 0; i < n; ++i)
    {
        while(q > 0 && P[q] != T[i])
            q = next[q-1];
        if (P[q] == T[i])
        {
            q++;
        }
        if (q == m)
        {
            printf("Pattern occurs with shift:%d\n",(i-m+1));
        }
    }
}
*/
