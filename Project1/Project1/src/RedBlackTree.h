#pragma once

enum Color {
	RED,
	BLACK
};

template <class T>
class RBTreeNode {

public:

	T key;
	Color color;
	RBTreeNode <T>* left, * right, * parent;

	RBTreeNode() {
		left = right = parent = nullptr;
		isLeft = isRight = false;
		color = RED;
	}

	RBTreeNode(T key) {
		this->key = key;
		left = right = parent = nullptr;
		color = RED;
	}
	bool isLeft() {
		if(this->parent)
			return this->parent->left == this;
		return false;
	}
	bool isRight() {
		if (this->parent)
			return this->parent->right == this;
		return false;
	}
	bool isLeaf() {
		return !left && !right;
	}

	
	bool checkChildrenColors(Color leftColor, Color rightColor) {
		bool a = !this->left || this->left->color == BLACK;
		bool b = !this->right || this->right->color == BLACK;
		if (a && b)
			return (leftColor == BLACK && rightColor == BLACK);
		else if(!a&&b)
			return (leftColor == RED && rightColor == BLACK);
		else if(a&&!b)
			return (leftColor == BLACK && rightColor == RED);
		return (leftColor == RED && rightColor == RED);
	}
};

template <class T>
class RBTree {

public:

	RBTreeNode <T>* root;
	int size;
	RBTree() {
		size = 0;
		root = nullptr;
	}
	void rotateRight(RBTreeNode <T>* n) {
		
		RBTreeNode <T>* temp = n->left;
		n->left = temp->right;
		if (n->left)
			n->left->parent = n;
		temp->right = n;
		temp->parent = n->parent;
		if (!n->parent)
			root = temp;
		else {
			if (n->isLeft())
				n->parent->left = temp;
			else
				n->parent->right = temp;
		}
		n->parent = temp;
	}

	void rotateLeft(RBTreeNode <T>* n) {

		RBTreeNode <T>* temp = n->right;
		n->right = temp->left;
		if (n->right)
			n->right->parent = n;
		temp->left = n;
		temp->parent = n->parent;
		if (!n->parent)
			root = temp;
		else {
			if (n->isLeft())
				n->parent->left = temp;
			else
				n->parent->right = temp;
		}
		n->parent = temp;
	}


	void insert(T key) {
		if (!root) {
			root = new RBTreeNode <T>(key);
			root->color = BLACK;
		}
		else
			insertAux(key, root);
		size++;
	}
	void insertAux(T key, RBTreeNode <T>* n) {

		if (key > n->key) {
			if(n->right)
				insertAux(key, n->right);
			else {
				RBTreeNode <T>* x = new RBTreeNode <T>(key);
				n->right = x;
				x->parent = n;
				insertFix(x);
			}
			return;
		}
		else {
			if (n->left)
				insertAux(key, n->left);
			else {
				RBTreeNode <T>* x = new RBTreeNode <T>(key);
				n->left = x;
				x->parent = n;
				insertFix(x);
			}
		}
	}
	
	void insertFix(RBTreeNode <T>* n) {

		
		while (n != root && n->parent->color == RED) {
			if (n->parent->isLeft()) {

				RBTreeNode <T>* uncle = n->parent->parent->right;
				//if uncle is black or nil node
				if (!uncle || uncle->color == BLACK) {
					
					if (n->isRight()) {
						rotateLeft(n->parent);
						n = n->left;
					}
					n->parent->color = BLACK;
					n->parent->parent->color = RED;
					rotateRight(n->parent->parent);
				}
				else {
					uncle->color = n->parent->color = BLACK;
					n = uncle->parent;
					n->color = RED;
				}
			}
			else {
				RBTreeNode <T>* uncle = n->parent->parent->left;
				//if uncle is black or nil node
				if (!uncle || uncle->color == BLACK) {

					if (n->isLeft()) {
						rotateRight(n->parent);
						n = n->right;
					}
					n->parent->color = BLACK;
					n->parent->parent->color = RED;
					rotateLeft(n->parent->parent);
				}
				else {
					uncle->color = n->parent->color = BLACK;
					n = uncle->parent;
					n->color = RED;
				}
			}
		}
		root->color = BLACK;
	}

	
	int getSize() {
		return size;
	}

	void erase(T key) {
		eraseAux(key, root);
		
	}
	
	RBTreeNode<T>* getLeaf(RBTreeNode <T>* n) {
		if (n->isLeaf())
			return n;
		RBTreeNode <T>* temp = n->left;
		if (temp) {
			while (temp->right)
				temp = temp->right;
			return getLeaf(temp);
		}
		else {
			temp = n->right;
			while (temp->left)
				temp = temp->left;
			return getLeaf(temp);
		}
	}

	void eraseAux(T key, RBTreeNode <T>* n) {
		if (!n)
			return;
		if (n->key == key) {
			// take predessor if it doesn't exist take successor
			
			T key = n->key;
			RBTreeNode <T>* temp = getLeaf(n);
			n->key = temp->key;
			temp->key = key;
			size--;
			// temp is pointing to the node that will be deleted
			deleteFix(temp);
			if (!temp->parent)
				root = nullptr;
			else {
				if (temp->isLeft())
					temp->parent->left = nullptr;
				else
					temp->parent->right = nullptr;
			}
			delete temp;
		}
		else if (key < n->key)
			eraseAux(key, n->left);
		else
			eraseAux(key, n->right);
	}

	void deleteFix(RBTreeNode<T>* n) {

		if (n->color == RED || n == root) {
			n->color = BLACK;
			return;
		}

		RBTreeNode<T>* parent = n->parent;
		if (n->isLeft()) {
			if (!parent->right || parent->right->color == BLACK) {
				if (!parent->right || parent->right->checkChildrenColors(BLACK, BLACK)) {
					parent->right->color = RED;
					deleteFix(parent);
				}
				else {

					if (parent->right->checkChildrenColors(RED, BLACK)) {
						parent->right->left->color = BLACK;
						parent->right->color = RED;
						rotateRight(parent->right);
						deleteFix(n);
					}
					else {
						parent->right->right->color = BLACK;
						parent->right->color = parent->color;
						parent->color = BLACK;
						rotateLeft(parent);
					}
				}
			}
			else {
				parent->right->color = parent->color;
				parent->color = RED;
				rotateLeft(parent);
				deleteFix(n);
			}
		}
		else {
			if (!parent->left || parent->left->color == BLACK) {
				if (!parent->left || parent->left->checkChildrenColors(BLACK, BLACK)) {
					parent->left->color = RED;
					deleteFix(parent);
				}
				else {
					if (parent->left->checkChildrenColors(BLACK, RED)) {
						parent->left->right->color = BLACK;
						parent->left->color = RED;
						rotateLeft(parent->left);
						deleteFix(n);
					}
					else {
						parent->left->left->color = BLACK;
						parent->left->color = parent->color;
						parent->color = BLACK;
						rotateRight(parent);
					}
				}
			}
			else {
				parent->left->color = parent->color;
				parent->color = RED;
				rotateLeft(parent);
				deleteFix(n);
			}
		}

	}

};