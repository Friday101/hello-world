/*
program:    KMP�㷨
author��    xiao han
time:       2017-10-21 10:57
use:        �ַ���ƥ��
����      �ַ���ƥ��
������أ�  ��׺�� ǰ׺�� DFA������ȷ���Զ����� BM�㷨
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
    int q,k;//q:ģ���ַ����±ꣻk:���ǰ��׺����
    int m = strlen(P);//ģ���ַ�������
    next[0] = 0;//ģ���ַ����ĵ�һ���ַ������ǰ��׺����Ϊ0
    for (q = 1,k = 0; q < m; ++q)//forѭ�����ӵڶ����ַ���ʼ�����μ���ÿһ���ַ���Ӧ��nextֵ
    {
        while(k > 0 && P[q] != P[k])//�ݹ�����P[0]������P[q]��������ͬ��ǰ��׺����k
            k = next[k-1];          //�����û��ϵ������ķ��������whileѭ�������δ���ľ������ڣ�ȷʵ�������
        if (P[q] == P[k])//�����ȣ���ô�����ͬǰ��׺���ȼ�1
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
1.�ַ��������ʼ��������ռ���ȫ������������ջ�������Զ������ݽ����޸ģ��ַ���ָ��ָ��������û�п����ַ������飬������ָ�������û���޸ĵ�Ȩ�ޡ�
���ӣ�char charr[] = "_test_strcat_redundance"; char* str="zifuchuanshuzu"��
���Ҫ���ַ������������ַ�����������strcpy strcat�ȣ���֧�� ��=�� ���ַ���ֱ�Ӹ�ֵ��
2.compare: next�ֱ�� -1,0 ��ʼ
(1)
void cal_next(char *str, int *next, int len)
{
    next[0] = -1;//next[0]��ʼ��Ϊ-1��-1��ʾ��������ͬ�����ǰ׺������׺
    int k = -1;//k��ʼ��Ϊ-1
    for (int q = 1; q <= len-1; q++)
    {
        while (k > -1 && str[k + 1] != str[q])//�����һ����ͬ����ôk�ͱ��next[k]��ע��next[k]��С��k�ģ�����kȡ�κ�ֵ��
        {
            k = next[k];//��ǰ����
        }
        if (str[k + 1] == str[q])//�����ͬ��k++
        {
            k = k + 1;
        }
        next[q] = k;//����ǰ����k��ֵ��������ͬ�����ǰ׺������׺��������next[q]
    }
}
int KMP(char *str, int slen, char *ptr, int plen)
{
    int *next = new int[plen];
    cal_next(ptr, next, plen);//����next����
    int k = -1;
    for (int i = 0; i < slen; i++)
    {
        while (k >-1&& ptr[k + 1] != str[i])//ptr��str��ƥ�䣬��k>-1����ʾptr��str�в���ƥ�䣩
            k = next[k];//��ǰ����
        if (ptr[k + 1] == str[i])
            k = k + 1;
        if (k == plen-1)//˵��k�ƶ���ptr����ĩ��
        {
            //cout << "��λ��" << i-plen+1<< endl;
            //k = -1;//���³�ʼ����Ѱ����һ��
            //i = i - plen + 2;//i��λ���ҵ�λ�ô�����һ��λ�ã�����Ĭ�ϴ�������ƥ���ַ������Բ����ص���
            return i-plen+1;//������Ӧ��λ��
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
