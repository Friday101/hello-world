/*
program:    Trie 树 / 字典树 / 前缀树
author：    xiao han
time:       2017-10-21 10:57
use:        词频统计，前缀匹配
领域：      字符串匹配
领域相关：  AC自动机 后缀树
*/
#include <iostream>
#include <queue>
#include <string.h>
#include <stdlib.h>
using namespace std;
#define MaxStringLength 20

char pattern[4][MaxStringLength] = {"he","she","her","him"};
char *str = "he does like her and not like him,but she does not like him";


typedef struct TrieNode
{
    char key;
    struct TrieNode *next[26];
    bool IsWord;
}*TrieTree;

TrieNode* createNode()
{
    TrieNode *newNode = (TrieNode *)malloc(sizeof(TrieNode));
    memset(newNode->next,0,sizeof(newNode->next));
    newNode->IsWord = false;
    return newNode;
}

void insert(TrieNode *root,char *key)
{
    TrieNode *node = root;
    int i = 0;
    while(key[i]!='\0')
    {
        if(node->next[key[i] - 'a'] == 0)
        {
            TrieNode *newNode = createNode();
            newNode->key = key[i];
            node->next[key[i] - 'a'] = newNode;
        }
        node = node->next[key[i] - 'a'];
        i++;
    }
    node->IsWord = true;
}

bool search(TrieTree T,char *str)
{
    TrieNode *node = T;
    int len = strlen(str);//sizeof(str)/sizeof(char);
    int i = 0;
    while(i < len)
    {
        if(node->next[str[i] - 'a' ] != 0 )
        {
            node = node->next[str[i] - 'a'];
        }
        else{
            return false;
        }
        i++;
    }
    if(node->IsWord == true)
        return true;
    else return false;
}

int main()
{
    TrieTree root = createNode();

    for(int i = 0; i < 4; i++ )
    {
        cout<<pattern[i]<<endl;
        insert(root,pattern[i]);
    }

    if(search(root,"his"))
    {
        cout<<"yes"<<endl;
    }
    else
    {
        cout<<"no"<<endl;
    }
    return 0;
}
/*
教训：
1.  sizeof与strlen区别：前者是分配给它的内存空间大小，后者是函数，求取字符串长度，以’\0‘结尾
2.  结构体声明实例以后要分配空间，使用malloc，不然造成内存错误
3.  字符数组传参三种形式：char t[],char t[10],char *t,但他们传递的都是第一个字符的地址
*/
