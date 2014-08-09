#include <iostream>
#include <limits>

using namespace std;

template<typename Key>
class SplayTree;

template<typename Key>
class Node {
	friend class SplayTree<Key>;
private:
	Key key;
	SplayTree<Key> left;
	SplayTree<Key> right;
public:
	Node(const Key& key) : key(key) { }
};

template<typename Key>
class SplayTree {
private:
	Node<Key>* root;
public:
	SplayTree() : root(0) { }
	
	~SplayTree() {
		delete root;
	}
	
	void add(const Key& key);
	
	bool contains(const Key& key);
	
	bool remove(const Key& key);
	
	void write() const;
	
private:
	// precondition: middle.root != 0
	void splay(const Key& key);
	
	// precondition: middle.root != 0
	void splay(const Key& key, SplayTree<Key>& left, SplayTree<Key>& right, SplayTree<Key>& maxLeft, SplayTree<Key>& minRight);
	
	void rotate(bool left);
};



template<typename Key>
void SplayTree<Key>::add(const Key& key) {
	if(root != 0) {
		// We splay the key to the top.
		splay(key);
		
		// If the key didn't exist in the tree, we add it.
		if(root->key != key) {
			Node<Key>* left;
			Node<Key>* right;
			
			if(key < root->key) {
				left = root->left.root;
				right = root;
				right->left.root = 0;
			} else {
				left = root;
				right = root->right.root;
				left->right.root = 0;
			}
			
			root = new Node<Key>(key);
			root->left.root = left;
			root->right.root = right;
		}
	} else {
		root = new Node<Key>(key);
	}
}

template<typename Key>
bool SplayTree<Key>::contains(const Key& key) {
	if(root != 0) {
		splay(key);
		return root->key == key;
	} else {
		return false;
	}
}

template<typename Key>
bool SplayTree<Key>::remove(const Key& key) {
	if(root != 0) {
		splay(key);
		
		if(root->key == key) {
			Node<Key>* newRoot;
			
			if(left != 0) {
				root->left.splay(numeric_limits<Key>::max());
				root->left.root->right.root = root->right.root;
				newRoot = root->left.root;
			} else {
				root->right.splay(numeric_limits<Key>::min());
				root->right.root->left.root = root->left.root;
				newRoot = root->right.root;
			}
			
			root->left.root = 0;
			root->right.root = 0;
			//delete root;
			
			root = newRoot;
			
			return true;
		} else {
			return false;
		}
	}
}

template<typename Key>
void SplayTree<Key>::write() const {
	if(root != 0) {
		root->left.write();
		cout << root->key << " ";
		root->right.write();
	}
}

template<typename Key>
void SplayTree<Key>::splay(const Key& key) {
	SplayTree<Key> left;
	SplayTree<Key> right;
	SplayTree<Key> maxLeft;
	SplayTree<Key> minRight;
	
	splay(key, left, right, maxLeft, minRight);
	
	if(left.root != 0) {
		maxLeft.root = left.root;
		root->left.root = left.root;
	}
	
	if(right.root != 0) {
		minRight.root = right.root;
		root->right.root = right.root;
	}
	
	// root 0 to prevent destructors
	left.root = 0;
	right.root = 0;
	maxLeft.root = 0;
	minRight.root = 0;
	
}

template<typename Key>
void SplayTree<Key>::splay(const Key& key, SplayTree<Key>& left, SplayTree<Key>& right, SplayTree<Key>& maxLeft, SplayTree<Key>& minRight) {
	if(key < root->key && root->left.root != 0) {
		if(key < root->left.root->key && root->left.root->left.root != 0) {
			// zig-zig
			rotate(false);
			right.root = root;
			root = root->left.root;
			right.root->left.root = 0;
			splay(key, left, right.root->left, maxLeft, minRight);
		} else if(key > root->left.root->key && root->left.root->right.root != 0) {
			// zig-zag
			right.root = root;
			left.root = root->left.root;
			root = root->left.root->right.root;
			
			left.root->right.root = 0;
			right.root->left.root = 0;
			splay(key, left.root->right, right.root->left, maxLeft, minRight);
		} else {
			// zig
			right.root = root;
			root = root->left.root;
			right.root->left.root = 0;
			splay(key, left, right.root->left, maxLeft, minRight);
		}
	} else if(key > root->key && root->right.root != 0) {
		if(key < root->right.root->key && root->right.root->right.root != 0) {
			// zig-zig
			rotate(true);
			left.root = root;
			root = root->right.root;
			left.root->right.root = 0;
			splay(key, left.root->right, right, maxLeft, minRight);
		} else if(key > root->right.root->key && root->right.root->left.root != 0) {
			// zig-zag
			left.root = root;
			right.root = root->right.root;
			root = root->right.root->left.root;
			
			left.root->right.root = 0;
			right.root->left.root = 0;
			splay(key, left.root->right, right.root->left, maxLeft, minRight);
		} else {
			// zig
			left.root = root;
			root = root->right.root;
			left.root->right.root = 0;
			splay(key, left.root->right, right, maxLeft, minRight);
		}
	} else {
		maxLeft = left;
		minRight = right;
	}
}

template<typename Key>
void SplayTree<Key>::rotate(bool left) {
	Node<Key>* up;
	Node<Key>* down = root;
	
	if(left) {
		up = down->right.root;
		down->right.root = up->left.root;
		up->left.root = down;
	} else {
		up = down->left.root;
		down->left.root = up->right.root;
		up->right.root = down;
	}
	
	root = up;
}

int main() {
	SplayTree<int> tree;
	
	for(int i = 0; i < 50; i++) {
		tree.add(i);
	}
	
	for(int i = 99; i >= 50; i--) {
		tree.add(i);
	}
	
	tree.remove(27);
	
	tree.write();
	
	return 0;
}