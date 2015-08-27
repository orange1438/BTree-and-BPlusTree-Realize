#include <stdio.h>
#include <stdlib.h>

/**
 * @brief the degree of btree
 * key per node: [M-1, 2M-1]
 * child per node: [M, 2M]
 */
 // M为树的深度 
#define M 2

typedef struct btree_node {
    int k[2*M-1];
    struct btree_node *p[2*M];
    int num;
    bool is_leaf;
} btree_node;

/**
 * @brief allocate a new btree node
 * default: is_leaf == true
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
    node->is_leaf = true;     //默认为叶子 
    
	return node;
}

btree_node *btree_create()
{
    btree_node *node = btree_node_new();
    if(NULL == node) {
        return NULL;
    }

    return node;
}

int btree_split_child(btree_node *parent, int pos, btree_node *child)
{
    btree_node *new_child = btree_node_new();
    if(NULL == new_child) {
        return -1;
    }
	// 新节点的is_leaf与child相同，key的个数为M-1
    new_child->is_leaf = child->is_leaf;
    new_child->num = M - 1;
    
    // 将child后半部分的key拷贝给新节点
    for(int i = 0; i < M - 1; i++) {
        new_child->k[i] = child->k[i+M];
    }

	// 如果child不是叶子，还需要把指针拷过去，指针比节点多1
    if(false == new_child->is_leaf) {
        for(int i = 0; i < M; i++) {
            new_child->p[i] = child->p[i+M];
        }
    }

    child->num = M - 1;

	// child的中间节点需要插入parent的pos处，更新parent的key和pointer
    for(int i = parent->num; i > pos; i--) {
        parent->p[i+1] = parent->p[i];
    }
    parent->p[pos+1] = new_child;

    for(int i = parent->num - 1; i >= pos; i--) {
        parent->k[i+1] = parent->k[i];
    }
    parent->k[pos] = child->k[M-1];
    
    parent->num += 1;
}

 // 执行该操作时，node->num < 2M-1 
void btree_insert_nonfull(btree_node *node, int target)
{
    if(1 == node->is_leaf) {
    	// 如果在叶子中找到，直接删除
        int pos = node->num;
        while(pos >= 1 && target < node->k[pos-1]) {
            node->k[pos] = node->k[pos-1];
            pos--;
        }

        node->k[pos] = target;
        node->num += 1;

    } else {
    	 // 沿着查找路径下降
        int pos = node->num;
        while(pos > 0 && target < node->k[pos-1]) {
            pos--;
        }

        if(2 * M -1 == node->p[pos]->num) {
        	// 如果路径上有满节点则分裂
            btree_split_child(node, pos, node->p[pos]);
            if(target > node->k[pos]) {
                pos++;
            }
        }
        
        btree_insert_nonfull(node->p[pos], target);
    }
}

//插入入口
btree_node* btree_insert(btree_node *root, int target)
{
    if(NULL == root) {
        return NULL;
    }
	
	// 对根节点的特殊处理，如果根是满的，唯一使得树增高的情形
    // 先申请一个新的
    if(2 * M - 1 == root->num) {
        btree_node *node = btree_node_new();
        if(NULL == node) {
            return root;
        }
        
        node->is_leaf = 0;
        node->p[0] = root;
        btree_split_child(node, 0, root);
        btree_insert_nonfull(node, target);
        return node;
    } else {
        btree_insert_nonfull(root, target);    
        return root;
    }
}

// 将y，root->k[pos], z合并到y节点，并释放z节点，y,z各有M-1个节点
void btree_merge_child(btree_node *root, int pos, btree_node *y, btree_node *z)
{
	// 将z中节点拷贝到y的后半部分
	y->num = 2 * M - 1;
	for(int i = M; i < 2 * M - 1; i++) {
		y->k[i] = z->k[i-M];
	}
	y->k[M-1] = root->k[pos];// k[pos]下降为y的中间节点
	
	// 如果z非叶子，需要拷贝pointer
	if(false == z->is_leaf) {
		for(int i = M; i < 2 * M; i++) {
			y->p[i] = z->p[i-M];
		}
	}

	// k[pos]下降到y中，更新key和pointer
	for(int j = pos + 1; j < root->num; j++) {
		root->k[j-1] = root->k[j];
		root->p[j] = root->p[j+1];
	}

	root->num -= 1;
	free(z);
}

/************  删除分析   **************
*
在删除B树节点时，为了避免回溯，当遇到需要合并的节点时就立即执行合并，B树的删除算法如下：从root向叶子节点按照search规律遍历：
（1）  如果target在叶节点x中，则直接从x中删除target，情况（2）和（3）会保证当再叶子节点找到target时，肯定能借节点或合并成功而不会引起父节点的关键字个数少于t-1。
（2）  如果target在分支节点x中：
（a）  如果x的左分支节点y至少包含t个关键字，则找出y的最右的关键字prev，并替换target，并在y中递归删除prev。
（b）  如果x的右分支节点z至少包含t个关键字，则找出z的最左的关键字next，并替换target，并在z中递归删除next。
（c）  否则，如果y和z都只有t-1个关键字，则将targe与z合并到y中，使得y有2t-1个关键字，再从y中递归删除target。
（3）  如果关键字不在分支节点x中，则必然在x的某个分支节点p[i]中，如果p[i]节点只有t-1个关键字。
（a）  如果p[i-1]拥有至少t个关键字，则将x的某个关键字降至p[i]中，将p[i-1]的最大节点上升至x中。
（b）  如果p[i+1]拥有至少t个关键字，则将x个某个关键字降至p[i]中，将p[i+1]的最小关键字上升至x个。
（c）  如果p[i-1]与p[i+1]都拥有t-1个关键字，则将p[i]与其中一个兄弟合并，将x的一个关键字降至合并的节点中，成为中间关键字。
* 
*/

 // 删除入口
btree_node *btree_delete(btree_node *root, int target)
{
	// 特殊处理，当根只有两个子女，切两个子女的关键字个数都为M-1时，合并根与两个子女
    // 这是唯一能降低树高的情形
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

 // root至少有个t个关键字，保证不会回溯
void btree_delete_nonone(btree_node *root, int target)
{
	if(true == root->is_leaf) {
		// 如果在叶子节点，直接删除
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
		if(i < root->num && target == root->k[i]) {
			 // 如果在分支节点找到target
			y = root->p[i];
			z = root->p[i+1];
			if(y->num > M - 1) {
				// 如果左分支关键字多于M-1，则找到左分支的最右节点prev，替换target
                // 并在左分支中递归删除prev,情况2（a)
				int pre = btree_search_predecessor(y);
				root->k[i] = pre;
				btree_delete_nonone(y, pre);
			} else if(z->num > M - 1) {
				// 如果右分支关键字多于M-1，则找到右分支的最左节点next，替换target
                // 并在右分支中递归删除next,情况2(b)
				int next = btree_search_successor(z);
				root->k[i] = next;
				btree_delete_nonone(z, next);
			} else {
				 // 两个分支节点数都为M-1，则合并至y，并在y中递归删除target,情况2(c)
				btree_merge_child(root, i, y, z);
				btree_delete(y, target);
			}
		} else {
			 // 在分支没有找到，肯定在分支的子节点中
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
					// 左邻接节点关键字个数大于M-1
					 //情况3(a)
					btree_shift_to_right_child(root, i-1, p, y);
				} else if(i < root->num && z->num > M - 1) {
					// 右邻接节点关键字个数大于M-1
					// 情况3(b)
					btree_shift_to_left_child(root, i, y, z);
				} else if(i > 0) {
					// 情况3（c)
					btree_merge_child(root, i-1, p, y); // note
					y = p;
				} else {
					// 情况3(c)
					btree_merge_child(root, i, y, z);
				}
				btree_delete_nonone(y, target);
			} else {
				btree_delete_nonone(y, target);
			}
		}

	}
}

//寻找rightmost，以root为根的最大关键字
int btree_search_predecessor(btree_node *root)
{
	btree_node *y = root;
	while(false == y->is_leaf) {
		y = y->p[y->num];
	}
	return y->k[y->num-1];
}

 // 寻找leftmost，以root为根的最小关键字
int btree_search_successor(btree_node *root) 
{
	btree_node *z = root;
	while(false == z->is_leaf) {
		z = z->p[0];
	}
	return z->k[0];
}

 // z向y借节点，将root->k[pos]下降至z，将y的最大关键字上升至root的pos处
void btree_shift_to_right_child(btree_node *root, int pos, 
		btree_node *y, btree_node *z)
{
	z->num += 1;
	for(int i = z->num -1; i > 0; i--) {
		z->k[i] = z->k[i-1];
	}
	z->k[0]= root->k[pos];
	root->k[pos] = y->k[y->num-1];

	if(false == z->is_leaf) {
		for(int i = z->num; i > 0; i--) {
			z->p[i] = z->p[i-1];
		}
		z->p[0] = y->p[y->num];
	}

	y->num -= 1;
}

 // y向借节点，将root->k[pos]下降至y，将z的最小关键字上升至root的pos处
void btree_shift_to_left_child(btree_node *root, int pos,
		btree_node *y, btree_node *z)
{
	y->num += 1;
	y->k[y->num-1] = root->k[pos];
	root->k[pos] = z->k[0];

	for(int j = 1; j < z->num; j++) {
		z->k[j-1] = z->k[j];
	}

	if(false == z->is_leaf) {
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

    btree_node *root = btree_create();

    for(int i = 0; i < sizeof(arr) / sizeof(int); i++) {
        root = btree_insert(root, arr[i]);
        btree_level_display(root);
    }

	//int todel[] = {15, 18, 23, 30, 31, 52, 50};
	int todel[] = {52};
	for(int i = 0; i < sizeof(todel) / sizeof(int); i++) {
		printf("after delete %d\n", todel[i]);
		root = btree_delete(root, todel[i]);
		btree_level_display(root);
	} 

    return 0;
}
