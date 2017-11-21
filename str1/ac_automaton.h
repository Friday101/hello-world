#ifndef _AC_AUTOMATON_H_
#define _AC_AUTOMATON_H_

#define PATTERN_PATH "pattern.txt"
#define STRING_PATH "string.txt"
#define OUTPUT_PATH "output.txt"


typedef unsigned char byte;
typedef struct trie_node{
	byte val;
	byte* s;
	short flag, terminal, s_size;
	struct trie_node** children;
	struct trie_node *fail, *parent;
	
} AC_NODE, *AC_TREE;

AC_TREE tree_init(const char*);
AC_TREE build_fail(AC_TREE);
void ac_search(AC_TREE, const char*, const char*);

#endif
