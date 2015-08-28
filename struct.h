#pragma once
// the degree of btree
#define M 2   
typedef struct btree_node {
	int k[2*M-1];
	struct btree_node *p[2*M];
	int num;
	bool is_leaf;
	struct btree_node *prev;  // use one struct just for simple,这2个，供B+Tree used
	struct btree_node *next;  // 供B+Tree used
} btree_node;
