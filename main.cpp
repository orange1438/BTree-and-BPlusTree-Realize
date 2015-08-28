#include <stdio.h>
#include <stdlib.h>
#include "BTree.h"
#include "BPlusTree.h"

int main()
{
	BTree bt;
	BPlusTree bpt;
	
    int arr[] = {18, 31, 12, 10, 15, 48, 45, 47, 50, 52, 23, 30, 20};
    for(int i = 0; i < sizeof(arr) / sizeof(int); i++) {
        bt.insert(arr[i]);
		bpt.insert(arr[i]);
    }
    printf("no delete data:\n");
    printf("display about B-Tree:\n");
	bt.level_display();
	bt.inorder_print();
	printf("\n\n");
	
 	printf("display about B+ Tree:\n");
	bt.level_display();
	bt.inorder_print();
	printf("\n");
	bpt.linear_print();
	printf("\n");
	
	printf("delete data...\n");
	int todel[] = {15, 18, 23, 30, 31, 52, 50};
//	int todel[] = {52};
	for(int i = 0; i < sizeof(todel) / sizeof(int); i++) {
		printf("after delete %d\n", todel[i]);
		bt.del(todel[i]);
		bpt.del(todel[i]);
	} 
	
 	printf("\n\ndelete after data:\n");
    printf("display about B-Tree:\n");
	bt.level_display();
	bt.inorder_print();
	printf("\n\n");
	
 	printf("display about B+ Tree:\n");
	bt.level_display();
	bt.inorder_print();
	printf("\n");
	bpt.linear_print();
	printf("\n");
	
    return 0;
}

