#pragma once
#include "Tree.h"
/*************************************调度类****************************************/
//调度类,根据传进来的参数,选择具体某个策略
class Context 
{
private:
	Tree *strategyChild;
public:
	Context(Tree *child)
 	{
		strategyChild=child;
	}

	void Insert(int target)
 	{
		strategyChild->insert(target);
	}
	void LevelDisplay()
	{
		strategyChild->level_display();
	};
	
	void Del(int target)
	{
		strategyChild->del(target);
	};
	
	void Inorder_Print()
	{
		strategyChild->inorder_print();
	};
	
	// tree node num
	void NodeNum_Print()
	{
		strategyChild->NodeNum_print();
	};
};
