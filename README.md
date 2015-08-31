文件说明：
1.Tree.h：B树和B+树的通用接口，虚基类。BTree和BPlusTree都继承它，只有BPlusTree才有linear函数
2.BTree.h,BTree.cpp：B树的声明、实现代码
3.BPlusTree.h,BPlusTree.cpp：B+树的声明、实现代码，注：大多数的函数，B和B+都是一样的，但是我还是分开写了，比如输出函数
4.mian.cpp：使用用例,因为我测试的时候windows和linux都有，为使中文不乱码，我在main写的简单的英语说明


时间测试说明：
100W的随机数据存入硬盘，包括插入数据的时间：
B存数据，1.70s左右；
B+存数据，inorder函数存，2.2s左右；linear函数存，2s左右；
如果不算插入的时间，只是存储的时间，0.2s-0.4s左右。
注：inorder函数：从根结点开始遍历，linear函数：从叶子结点链表开始遍历。写入文件用的fwrite来写的整体结构体来存储测试的。另外的fprintf存文件，100W数据，起码要30秒。