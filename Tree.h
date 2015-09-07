#pragma once
#include "struct.h"
#include <stdio.h>
/**
 * @brief the degree of btree
 * key per node: [M-1, 2M-1]
 * child per node: [M, 2M]
 */

class Tree
{
protected:	
	btree_node *roots;
	
	FILE *pfile;
	
	/**
	 * @brief create a btree root
	 *
	 * @return pointer of btree root
	 */
	virtual btree_node *btree_create()=0;

	/**
	 * @brief allocate a new btree node
	 * default: is_leaf == true
	 *
	 * @return pointer of new node
	 */
	virtual btree_node *btree_node_new()=0;

	/**
	 * @brief split child if num of key in child exceed 2M-1
	 *
	 * @param parent: parent of child
	 * @param pos: p[pos] points to child
	 * @param child: the node to be splited
	 *
	 * @return 
	 */
	virtual int btree_split_child(btree_node *parent, int pos, btree_node *child)=0;

	/**
	 * @brief insert a value into btree
	 * the num of key in node less than 2M-1
	 *
	 * @param node: tree root
	 * @param target: target to insert
	 */
	virtual void btree_insert_nonfull(btree_node *node, int target)=0;

	/**
	 * @brief merge y, z and root->k[pos] to left
	 * this appens while y and z both have M-1 keys
	 *
	 * @param root: parent node
	 * @param pos: postion of y 
	 * @param y: left node to merge
	 * @param z: right node to merge
	 */
	virtual void btree_merge_child(btree_node *root, int pos, btree_node *y, btree_node *z)=0;

	/**
	 * @brief delete a vlue from btree
	 * root has at least M keys
	 *
	 * @param root: btree root
	 * @param target: target to delete
	 *
	 * @return 
	 */
	virtual void btree_delete_nonone(btree_node *root, int target)=0;

	/**
	 * @brief find the leftmost value
	 *
	 * @param root: root of tree
	 *
	 * @return: the leftmost value
	 */
	virtual int btree_search_successor(btree_node *root)=0;

	/**
	 * @brief find the rightmost value
	 *
	 * @param root: root of tree
	 *
	 * @return: the rightmost value
	 */
	virtual int btree_search_predecessor(btree_node *root)=0;

	/**
	 * @brief shift a value from z to y
	 *
	 * @param root: btree root
	 * @param pos: position of y
	 * @param y: left node
	 * @param z: right node
	 */
	virtual void btree_shift_to_left_child(btree_node *root, int pos, btree_node *y, btree_node *z)=0;

	/**
	 * @brief shift a value from z to y
	 *
	 * @param root: btree root
	 * @param pos: position of y
	 * @param y: left node
	 * @param z: right node
	 */
	virtual void btree_shift_to_right_child(btree_node *root, int pos, btree_node *y, btree_node *z)=0;

	/**
	 * @brief insert a value into btree

	 *
	 * @param root: tree root
	 * @param target: target to insert
	 *
	 * @return: new root of tree
	 */
	virtual btree_node* btree_insert(btree_node *root, int target)=0;

	/**
	 * @brief delete a vlue from btree
	 *
	 * @param root: btree root
	 * @param target: target to delete
	 *
	 * @return: new root of tree
	 */
	virtual btree_node *btree_delete(btree_node *root, int target)=0;

	/**
	 * @brief inorder traverse the btree
	 *
	 * @param root: root of treee
	 */
	virtual void btree_inorder_print(btree_node *root)=0;


	/**
	 * @brief level print the btree
	 *
	 * @param root: root of tree
	 */
	virtual void btree_level_display(btree_node *root)=0;

	/**
 	* @Save the btree
 	*
 	* @param root: root of tree
 	*/
  	virtual void Save(btree_node *root)=0;
  	
  	int btree_node_num;  //记录多少个树结点： how many  btree_node
public:

	Tree(void)
	{
		btree_node_num=0;
	};
	virtual ~Tree(void)
	{
		btree_node_num=0;
		delete roots;
	};

	/*
	* @param target: target to insert
	*/
	void insert(int target)
	{
		roots = btree_insert(roots, target);
		Save(roots);  // 即时保存 
	};

	/**
	 * @brief level print the btree
	 */
	void level_display()
	{
		btree_level_display(roots);
	};
	
	/**
	 * @brief level print the btree
	 */
	void del(int target)
	{
		roots = btree_delete(roots, target);

		Save(roots);  // 即时保存 
	};
	
	void inorder_print()
	{
		btree_inorder_print(roots);
	};
	
	// tree node num
	void NodeNum_print()
	{
		printf("%d\n", btree_node_num);
	};
};
