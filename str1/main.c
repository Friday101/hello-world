//
//  main.c
//  AC
//
//  Created by 任权 on 2017/10/16.
//  Copyright © 2017年 任权. All rights reserved.
//

#include <time.h>
#include "ac_automaton.h"
#include <stdio.h>

int main(int argc, const char * argv[]) {


    //format: ./strsearch string.txt pattern.txt result.txt

    if (argc < 4) {
        printf("Usage: strsearch (string.txt) (pattern.txt) (result.txt)\n");
        return 0;
    }
    const char *string_path = argv[1];
    const char *pattern_path = argv[2];
    const char *result_path = argv[3];
    time_t t1, t2, t3, t4;
    t1 = time(NULL);
    printf("tree init\n");
    AC_TREE r = tree_init(pattern_path);
    t2 = time(NULL);
    printf("bulid fail ptr\n");
    r = build_fail(r);
    t3 = time(NULL);
    printf("search\n");
    ac_search(r, string_path, result_path);
    t4 = time(NULL);
    printf("tree_init():%lds, build_fail():%lds, ac_search():%lds\n", t2 - t1, t3 - t2, t4 - t3);

    return 0;
}
/*
1.read() : ssize_t read (int fd, void *buf, size_t count);
#include <unistd.h>
（1）如果成功，返回读取的字节数；

（2）如果出错，返回-1并设置errno；

（3）如果在调read函数之前已是文件末尾，则返回0

2.1KB = 1024B 1MB = 1048576B

3.fwrite fprintf
fwrite是将数据不经转换直接以二进制的形式写入文件，而fprintf是将数据转换为字符后再写入文件
 当使用fwrite将一个int型数字65写入文本文件时，由于65对应的二进制数是1000001，十六进制数是0x41，存储的是以二进制的形式1000001.
 在notepad++中使用十六进制方式打开显示的是:0x0041，转换为十进制则为65，使用记事本打开这个文本文件后显示的是A，
 因为记事本程序默认为存储在文本文件中的数据都是ASCII码形式存储，它把65当做ASCII码翻译为字符A。

当使用fpintf将一个int型数字65写入文本文件时，将65每一位转换为ASCII码存储，6、5分别对应ASCII码54、53，存储的是ASCII码54、53.
在notepad++中使用十六进制方式打开显示的是:3635，转换为十进制则为54、53，这正是数字6、5的ASCII码。
使用记事本打开这个文本文件时，记事本将存储在其中的54、53当做ASCII码翻译为字符6、5显示，我们看到的是便是字符65。
*/
