#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>
#include "ac_automaton.h"

AC_TREE tree_init(const char* pattern_path)
{
	AC_TREE root = (AC_TREE)malloc(sizeof(AC_NODE));
	root->flag = 1;
	root->parent = NULL;
	root->fail = NULL;
	root->s_size = 0;
	root->terminal = 0;
	root->children = (AC_NODE**)malloc(sizeof(AC_NODE*)*256);
	for (int i = 0; i < 256; i++) {
		root->children[i] = NULL;
	}
	int fd = open(pattern_path, O_RDONLY);
	if (fd < 0) {
		printf("pattern file open error\n");
		return NULL;
	}
	byte buf[4096], bsrc[128];
	unsigned long count = 0;
	int pos = 0;
	AC_NODE *temp = root;
	while((count=read(fd, buf, 4096)) > 0)
	{
		for(int i = 0; i < count; i++)
		{
			while(i < count&&buf[i] != 0x0a)//\n
			{
				if (temp->children[buf[i]] == NULL) {
					AC_NODE *new_node = (AC_NODE*)malloc(sizeof(AC_NODE));
					new_node->val = buf[i];
					new_node->parent = temp;
					new_node->terminal = 0;
					new_node->s_size = 0;
					new_node->flag = ((0x80^buf[i])>>7 == 1||temp->flag == 0) ? 1 : 0;//the first bit is 1 or temp->flag = 0
					new_node->children = (AC_NODE**)malloc(sizeof(AC_NODE*)*256);
					for (int i = 0; i < 256; i++) {
						new_node->children[i] = NULL;
					}
					temp->children[buf[i]] = new_node;
				}

				bsrc[pos] = buf[i];
				pos++;
				temp = temp->children[buf[i]];
				i++;
			}
			if (buf[i] == 0x0a)
			{
				temp->terminal = 1;
				//byte des[pos];
				byte *des = (byte*)malloc(sizeof(byte)*pos);
				temp->s_size = pos;
				memcpy(des, bsrc, sizeof(byte)*(pos));
				temp->s = des;
				temp = root;
				pos = 0;
			}
		}
	}
	if(close(fd) != 0) printf("file close error\n");
	return root;
}
AC_TREE build_fail(AC_TREE root)
{
	if(root == NULL)return NULL;
	int size = 1048576;
	AC_NODE ** n_queue = (AC_NODE**)malloc(sizeof(AC_NODE*)*size);
	int head = 0, tail = 0;
	//empty: head == tail ,full: (tail+1) % size == head
	//init
	tail++;
	n_queue[tail] = root;
	while(head != tail)
	{
		AC_NODE *c = n_queue[(head+1)%size];
		head = (head+1) % size;
		for(int i = 0;i < 256; i++)
		{
			AC_NODE *cnode = c->children[i];
			if(cnode != NULL&&cnode->val != 0x00)
			{
				tail = (tail+1) % size;
				n_queue[tail] = cnode;
				if((tail+1) % size == head){printf("queue full\n");return NULL;}
				AC_NODE *temp = cnode->parent->fail;
				while(temp != NULL&&temp->children[cnode->val] == NULL)
					temp = temp->fail;
				cnode->fail = (temp!=NULL) ? temp->children[cnode->val] : root;
				//printf("cnode->val:%c cnode->depth: %d cnode->fail->val:%c cnode->fail->depth:%d\n",cnode->val, cnode->depth, cnode->fail->val, cnode->fail->depth);
			}
		}
	}

	if (n_queue != NULL) {
		free(n_queue);
	}
	return root;
}

void ac_search(AC_TREE root, const char* string_path, const char* result_path)
{
	if (root == NULL) return ;
	int string_fd = open(string_path, O_RDONLY);
	FILE *output_fd = fopen(result_path, "w");
	if (string_fd < 0) {
		printf("string file open error\n");
		return;
	}

	int buf_size = 1048576;
	byte buf[buf_size];
	unsigned long long str_pos = 0;
	unsigned long count = 0;
	int buf_ptr = 0, temp_flag = 1;
	AC_NODE *temp = root;

	while((count=read(string_fd, buf, buf_size))>0)
	{

		while(buf_ptr < count)
		{
			temp_flag = temp->flag;
			while(temp != NULL&&(temp->children[buf[buf_ptr]] == NULL))
				temp = temp->fail;
			if(temp == NULL)//last word is done
			{
				temp = root;
				if(((buf[buf_ptr]^0x80)>>7)!=1 && temp->flag==1)//the first char is not 1
				{
					str_pos += 1;
					buf_ptr += 1;
				}
			}else{
				temp = temp->children[buf[buf_ptr]];
				AC_NODE *write_temp = temp;
				while(write_temp->fail != NULL)
				{
					//write result to file(format: key pos\n)
					if(write_temp->terminal == 1){
						fwrite(write_temp->s, sizeof(byte), write_temp->s_size, output_fd);
						fprintf(output_fd, " %llu\n", ((str_pos+1)-write_temp->s_size));
					}
					write_temp = write_temp->fail;
				}
			}
			str_pos += 1;
			buf_ptr += 1;
		}
		buf_ptr = buf_ptr - (int)count;//every time is from 0
	}

	close(string_fd);
	fclose(output_fd);
}

