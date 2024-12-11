#include <iostream>
#include"RedBlackTree.h"

int main() {

	RBTree<int> t;
	t.insert(7);
	t.insert(9);
	t.insert(4);
	t.insert(5);
	t.insert(6);
	t.insert(8);
	t.insert(15);
	t.insert(13);
	t.insert(16);
	t.insert(17);
	t.erase(9);
	t.erase(4);
	t.erase(17);

	return 0;
}