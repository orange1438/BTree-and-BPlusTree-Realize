///////////////////////////////////////
//////////  ×¢ÊÍ¼û B-TREE  /////////
/////////                    /////////
///////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief the degree of btree
 * key per node: [M-1, 2M-1]
 * child per node: [M, 2M]
 */
#define M 2  // the degree of btree
#define TRUE 1
#define FALSE 0

typedef struct btree_node {
    int k[2*M-1];
    struct btree_node *p[2*M];
    int num;
    int is_leaf;
	struct btree_node *prev;  // use one struct just for simple
	struct btree_node *next;
} btree_node;


/**
 * @brief allocate a new btree node
 * default: is_leaf == TRUE
 *
 * @return pointer of new node
 */
btree_node *btree_node_new();


/**
 * @brief create a btree root
 *
 * @return pointer of btree root
 */
btree_node *btree_create();


/**
 * @brief split child if num of key in child exceed 2M-1
 *
 * @param parent: parent of child
 * @param pos: p[pos] points to child
 * @param child: the node to be splited
 *
 * @return 
 */
int btree_split_child(btree_node *parent, int pos, btree_node *child);


/**
 * @brief insert a value into btree
 * the num of key in node less than 2M-1
 *
 * @param node: tree root
 * @param target: target to insert
 */
void btree_insert_nonfull(btree_node *node, int target);


/**
 * @brief insert a value into btree

 *
 * @param root: tree root
 * @param target: target to insert
 *
 * @return: new root of tree
 */
btree_node* btree_insert(btree_node *root, int target);


/**
 * @brief merge y, z and root->k[pos] to left
 * this appens while y and z both have M-1 keys
 *
 * @param root: parent node
 * @param pos: postion of y 
 * @param y: left node to merge
 * @param z: right node to merge
 */
void btree_merge_child(btree_node *root, int pos, btree_node *y, btree_node *z);

/**
 * @brief delete a vlue from btree
 *
 * @param root: btree root
 * @param target: target to delete
 *
 * @return: new root of tree
 */
btree_node *btree_delete(btree_node *root, int target);

/**
 * @brief delete a vlue from btree
 * root has at least M keys
 *
 * @param root: btree root
 * @param target: target to delete
 *
 * @return 
 */
void btree_delete_nonone(btree_node *root, int target);


/**
 * @brief find the rightmost value
 *
 * @param root: root of tree
 *
 * @return: the rightmost value
 */
int btree_search_predecessor(btree_node *root);


/**
 * @brief find the leftmost value
 *
 * @param root: root of tree
 *
 * @return: the leftmost value
 */
int btree_search_successor(btree_node *root);


/**
 * @brief shift a value from z to y
 *
 * @param root: btree root
 * @param pos: position of y
 * @param y: left node
 * @param z: right node
 */
void btree_shift_to_left_child(btree_node *root, int pos, btree_node *y, btree_node *z);

/**
 * @brief shift a value from z to y
 *
 * @param root: btree root
 * @param pos: position of y
 * @param y: left node
 * @param z: right node
 */
void btree_shift_to_right_child(btree_node *root, int pos, btree_node *y, btree_node *z);


/**
 * @brief inorder traverse the btree
 *
 * @param root: root of treee
 */
void btree_inorder_print(btree_node *root);

/**
 * @brief print tree linearly using prev/next pointer
 *
 * @param root: root of tree
 */
void btree_linear_print(btree_node *root);



/**
 * @brief level print the btree
 *
 * @param root: root of tree
 */
void btree_level_display(btree_node *root);

btree_node *btree_node_new()
{
    btree_node *node = (btree_node *)malloc(sizeof(btree_node));
    if(NULL == node) {
        return NULL;
    }

    for(int i = 0; i < 2 * M -1; i++) {
        node->k[i] = 0;
    }

    for(int i = 0; i < 2 * M; i++) {
        node->p[i] = NULL;
    }

    node->num = 0;
    node->is_leaf = TRUE;
	node->prev = NULL;
	node->next = NULL;
	return node; 
}

btree_node *btree_create()
{
    btree_node *node = btree_node_new();
    if(NULL == node) {
        return NULL;
    }

	node->next = node;
	node->prev = node;

    return node;
}

int btree_split_child(btree_node *parent, int pos, btree_node *child)
{
    btree_node *new_child = btree_node_new();
    if(NULL == new_child) {
        return -1;
    }

    new_child->is_leaf = child->is_leaf;
    new_child->num = M - 1;
    
    for(int i = 0; i < M - 1; i++) {
        new_child->k[i] = child->k[i+M];
    }

    if(FALSE == new_child->is_leaf) {
        for(int i = 0; i < M; i++) {
            new_child->p[i] = child->p[i+M];
        }
    }

    child->num = M - 1;
	if(TRUE == child->is_leaf) {
		child->num++;  // if leaf, keep the middle ele, put it in the left
	}

    for(int i = parent->num; i > pos; i--) {
        parent->p[i+1] = parent->p[i];
    }
    parent->p[pos+1] = new_child;

    for(int i = parent->num - 1; i >= pos; i--) {
        parent->k[i+1] = parent->k[i];
    }
    parent->k[pos] = child->k[M-1];
    
    parent->num += 1;

	// update link
	if(TRUE == child->is_leaf) {
		new_child->next = child->next;
		child->next->prev = new_child;
		new_child->prev = child;
		child->next = new_child;
	}
}

void btree_insert_nonfull(btree_node *node, int target)
{
    if(TRUE == node->is_leaf) {
        int pos = node->num;
        while(pos >= 1 && target < node->k[pos-1]) {
            node->k[pos] = node->k[pos-1];
            pos--;
        }

        node->k[pos] = target;
        node->num += 1;

    } else {
        int pos = node->num;
        while(pos > 0 && target < node->k[pos-1]) {
            pos--;
        }

        if(2 * M -1 == node->p[pos]->num) {
            btree_split_child(node, pos, node->p[pos]);
            if(target > node->k[pos]) {
                pos++;
            }
        }
        
        btree_insert_nonfull(node->p[pos], target);
    }
}

btree_node* btree_insert(btree_node *root, int target)
{
    if(NULL == root) {
        return NULL;
    }

    if(2 * M - 1 == root->num) {
        btree_node *node = btree_node_new();
        if(NULL == node) {
            return root;
        }
        
        node->is_leaf = FALSE;
        node->p[0] = root;
        btree_split_child(node, 0, root);
        btree_insert_nonfull(node, target);
        return node;
    } else {
        btree_insert_nonfull(root, target);    
        return root;
    }
}

void btree_merge_child(btree_node *root, int pos, btree_node *y, btree_node *z)
{
	if(TRUE == y->is_leaf) {
		y->num = 2 * M - 2;
		for(int i = M; i < 2 * M - 1; i++) {
			y->k[i-1] = z->k[i-M];
		}
	} else {
		y->num = 2 * M - 1;
		for(int i = M; i < 2 * M - 1; i++) {
			y->k[i] = z->k[i-M];
		}
		y->k[M-1] = root->k[pos];
		for(int i = M; i < 2 * M; i++) {
			y->p[i] = z->p[i-M];
		}
	}

	for(int j = pos + 1; j < root->num; j++) {
		root->k[j-1] = root->k[j];
		root->p[j] = root->p[j+1];
	}

	root->num -= 1;

	// update link
	if(TRUE == y->is_leaf) {
		y->next = z->next;
		z->next->prev = y;
	}

	free(z);
}

btree_node *btree_delete(btree_node *root, int target)
{
	if(1 == root->num) {
		btree_node *y = root->p[0];
		btree_node *z = root->p[1];
		if(NULL != y && NULL != z &&
				M - 1 == y->num && M - 1 == z->num) {
			btree_merge_child(root, 0, y, z);
			free(root);
			btree_delete_nonone(y, target);
			return y;
		} else {
			btree_delete_nonone(root, target);
			return root;
		}
	} else {
		btree_delete_nonone(root, target);	
		return root;
	}
}

void btree_delete_nonone(btree_node *root, int target)
{
	if(TRUE == root->is_leaf) {
		int i = 0;
		while(i < root->num && target > root->k[i]) i++;
		if(target == root->k[i]) {
			for(int j = i + 1; j < 2 * M - 1; j++) {
				root->k[j-1] = root->k[j];
			}
			root->num -= 1;
		} else {
			printf("target not found\n");
		}
	} else {
		int i = 0;
		btree_node *y = NULL, *z = NULL;
		while(i < root->num && target > root->k[i]) i++;
		
		y = root->p[i];
		if(i < root->num) {
			z = root->p[i+1];
		}
		btree_node *p = NULL;
		if(i > 0) {
			p = root->p[i-1];
		}

		if(y->num == M - 1) {
			if(i > 0 && p->num > M - 1) {
				btree_shift_to_right_child(root, i-1, p, y);
			} else if(i < root->num && z->num > M - 1) {
				btree_shift_to_left_child(root, i, y, z);
			} else if(i > 0) {
				btree_merge_child(root, i-1, p, y);
				y = p;
			} else {
				btree_merge_child(root, i, y, z);
			}
			btree_delete_nonone(y, target);
		} else {
			btree_delete_nonone(y, target);
		}
	}
}

int btree_search_predecessor(btree_node *root)
{
	btree_node *y = root;
	while(FALSE == y->is_leaf) {
		y = y->p[y->num];
	}
	return y->k[y->num-1];
}

int btree_search_successor(btree_node *root) 
{
	btree_node *z = root;
	while(FALSE == z->is_leaf) {
		z = z->p[0];
	}
	return z->k[0];
}


void btree_shift_to_right_child(btree_node *root, int pos, 
		btree_node *y, btree_node *z)
{
	z->num += 1;

	if(FALSE == z->is_leaf) {
		z->k[0] = root->k[pos];
		root->k[pos] = y->k[y->num-1];
	} else {
		z->k[0] = y->k[y->num-1];
		root->k[pos] = y->k[y->num-2];
	}

	for(int i = z->num -1; i > 0; i--) {
		z->k[i] = z->k[i-1];
	}

	if(FALSE == z->is_leaf) {
		for(int i = z->num; i > 0; i--) {
			z->p[i] = z->p[i-1];
		}
		z->p[0] = y->p[y->num];
	} 

	y->num -= 1;
}

void btree_shift_to_left_child(btree_node *root, int pos,
		btree_node *y, btree_node *z)
{
	y->num += 1;

	if(FALSE == z->is_leaf) {
		y->k[y->num-1] = root->k[pos];
		root->k[pos] = z->k[0];
	} else {
		y->k[y->num-1] = z->k[0];
		root->k[pos] = z->k[0];
	}

	for(int j = 1; j < z->num; j++) {
		z->k[j-1] = z->k[j];
	}

	if(FALSE == z->is_leaf) {
		y->p[y->num] = z->p[0];
		for(int j = 1; j <= z->num; j++) {
			z->p[j-1] = z->p[j];
		}
	} 

	z->num -= 1;
}

void btree_inorder_print(btree_node *root) 
{
    if(NULL != root) {
        btree_inorder_print(root->p[0]);
        for(int i = 0; i < root->num; i++) {
            printf("%d ", root->k[i]);
            btree_inorder_print(root->p[i+1]);
        }
    }
}

void btree_linear_print(btree_node *root) 
{
	if(NULL != root) {
		btree_node *leftmost = root;
		while(FALSE == leftmost->is_leaf) {
			leftmost = leftmost->p[0];
		}
		
		btree_node *iter = leftmost;
		do {
			for(int i = 0; i < iter->num; i++) {
            	printf("%d ", iter->k[i]);
			}
			iter = iter->next;
        } while(iter != leftmost);
		printf("\n");
	}
}

void btree_level_display(btree_node *root) 
{
	
	// just for simplicty, can't exceed 200 nodes in the tree
    btree_node *queue[200] = {NULL};
    int front = 0;
    int rear = 0;

    queue[rear++] = root;

    while(front < rear) {
        btree_node *node = queue[front++];

        printf("[");
        for(int i = 0; i < node->num; i++) {
            printf("%d ", node->k[i]);
        }
        printf("]");

        for(int i = 0; i <= node->num; i++) {
            if(NULL != node->p[i]) {
                queue[rear++] = node->p[i];               
            }
        }
    }
	printf("\n");
}

int main()
{
    int arr[] = {18, 31, 12, 10, 15, 48, 45, 47, 50, 52, 23, 30, 20};
//    int arr[] = {18, 31, 12, 10};

    btree_node *root = btree_create();

    for(int i = 0; i < sizeof(arr) / sizeof(int); i++) {
        root = btree_insert(root, arr[i]);
        btree_level_display(root);
		btree_linear_print(root);
    }

	//int todel[] = {15, 18, 23, 30, 31, 52, 50};
	int todel[] = {45, 30, 12, 10};
	for(int i = 0; i < sizeof(todel) / sizeof(int); i++) {
		printf("after delete %d\n", todel[i]);
		root = btree_delete(root, todel[i]);
		btree_level_display(root);
		btree_linear_print(root);
	} 

    return 0;
}
