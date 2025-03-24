#include <cassert>
#include <cstdio>

template<typename T>
T min(const T& a, const T& b) {
	T result = b < a ? b : a;
	return result;
}

template<typename T>
T max(const T& a, const T& b) {
	T result = b > a ? b : a;
	return result;
}

template<typename Tval>
struct RedBlackTree {	
	
	struct Node {
		Tval value;
		
		Node *parent = nullptr;
		Node *left = nullptr;
		Node *right = nullptr;
		
		bool is_red = false;
		
		Node *find(Tval search_value);		
		int count();		
		Node *get_insertion_parent(Tval new_value);
		void attach_child(Node *Node);
		void append_leaf(Node *new_node);
		Node *add(const Tval&);
		void flip_colors_with_children();
		void flip_colors_with_parent();
		void rotate_right();
		void rotate_left();
		bool is_black();
		bool is_root();
		bool is_leaf();
		bool is_3_node();
		Node *fix_up();
		void walk_down();
		
		//store contained values in order
		Tval * inorder_to_buf(Tval *out);	
		
		Node(Tval value_) :value{value_}
		{}
	};
	
	Node *root;
	
	void add(const Tval& new_val);
	
	RedBlackTree(Tval value_)
	:root{new Node{value_}}
	{}	
	
	
	
};

// template<typename Tval>
// using RedBlackTree = RedBlackTree<Tval>;

template <typename Tval>
int RedBlackTree<Tval>::Node::count() {
	int result = 1;
	if (left) result += left->count();
	if (right) result += right->count();
	return result;
}

template<typename Tval>
RedBlackTree<Tval>::Node *RedBlackTree<Tval>::Node::find(Tval search_value) {	
	Node *result = nullptr;
	if (value == search_value) {
		result = this;
	} else if (search_value < value) {
		result = left ? left -> find( search_value ) : nullptr;
	} else {
		result = right ? right -> find( search_value ) : nullptr ;
	}
	
	return result;
}


template<typename Tval>
Tval *RedBlackTree<Tval>::Node::inorder_to_buf(Tval *out) {
	Tval *next_slot = out;
	
	next_slot = left ? left -> inorder_to_buf(next_slot) : next_slot;
	*next_slot++ = value;
	next_slot = right ? right -> inorder_to_buf(next_slot) : next_slot;
	
	return next_slot;
}


/**
 *  @return nullptr if already exists, pointer to parent otherwise
 */
template<typename Tval>
RedBlackTree<Tval>::Node *RedBlackTree<Tval>::Node::get_insertion_parent(Tval new_value) {
	
	RedBlackTree::Node *result = nullptr;
	
	if (value == new_value) {
		result = nullptr;
	} else if (new_value < value) {
		result = left ? left -> get_insertion_parent( new_value ) : this;
	} else {
		result = right ? right -> get_insertion_parent( new_value ) : this;
	}
	
	return result;
}


template<typename Tval>
void RedBlackTree<Tval>::Node::attach_child(RedBlackTree::Node *node) {
	assert(node->value != value);
	if (node->value < value) {
		assert(left == nullptr);
		left = node;
	} else {
		assert(right == nullptr);
		right = node;
	}
	node->parent = this;
	
	return; 
}


template<typename Tval>
void RedBlackTree<Tval>::Node::append_leaf(RedBlackTree::Node *node){	
	//percolate down from root and attach to parent
	
	if (!node) return;
	RedBlackTree::Node *insertion_parent = get_insertion_parent( node->value );
	insertion_parent->attach_child(node);
	
	return;
}

template<typename Tval>
bool RedBlackTree<Tval>::Node::is_root(){
	bool result = parent == nullptr;
	return result;
}

template<typename Tval>
bool RedBlackTree<Tval>::Node::is_leaf(){
	bool result = left == nullptr;
		 result = result && right == nullptr;
	return result;
}

template<typename Tval>
bool RedBlackTree<Tval>::Node::is_3_node(){
	bool result = left && left->is_red;
	return result;
}

template<typename Tval>
void RedBlackTree<Tval>::Node::flip_colors_with_parent(){
	assert(parent);
	assert(is_red != parent->is_red);
	
	parent->is_red = is_red;
	is_red = !is_red;
	
	return;
}

template<typename Tval>
void RedBlackTree<Tval>::Node::flip_colors_with_children(){
	assert(left && right);
	assert(left->is_red == right->is_red && left->is_red != is_red);
	
	left->is_red = right->is_red = is_red;
	is_red = !is_red;	
	
	return;
}

template<typename Tval>
void RedBlackTree<Tval>::Node::rotate_right(){
	RedBlackTree<Tval>::Node *old_parent = parent;
	assert(left);
	parent = left;
	left = parent->right;
	if (left){
		left->parent = this;
	}
	parent->right = this;
	
	parent->parent = old_parent;
	if (old_parent) {
		if (old_parent->left == this) {
			old_parent->left = parent;
		} else {
			assert(old_parent->right == this);
			old_parent->right = parent;
		}
	}	
	
	return;
}

template<typename Tval>
void RedBlackTree<Tval>::Node::rotate_left(){
	RedBlackTree<Tval>::Node *old_parent = parent;
	assert(right);
	parent = right;
	right = parent->left;
	if (right) {
		right->parent = this;
	}
	parent->left = this;
	
	parent->parent = old_parent;
	if(old_parent) {
		if (old_parent->left == this){
			old_parent->left = parent;
		} else {
			assert(old_parent->right == this);
			old_parent->right = parent;
		}
	}
	return;
}

template<typename Tval>
RedBlackTree<Tval>::Node *RedBlackTree<Tval>::Node::fix_up() {	
	assert(is_red);	
	//NOTE(Gerald): we track the highest node in the tree that is affected
	//we return it so tree can check if it's the new root and update if needed
	Node *highest_changed = this; 
	
	if (is_root()) {
		is_red = false;
		return highest_changed;
	}
	
	assert(parent);
	
	if (parent->is_black()) {
		if (parent->left == this) {
			//highest_changed = this
		} else { // this is right child
			if (!parent->left || parent->left->is_black()) {
				parent->rotate_left();
				left->flip_colors_with_parent();
				//highest_changed = this
			} else { //parent->left is red
				parent->flip_colors_with_children();
				highest_changed = parent->fix_up();
			}
		}		
	} else { //parent is red
		if (parent->left == this) {			
			assert(parent->parent);
			parent->parent->rotate_right();
			is_red = false;
			highest_changed = parent->fix_up();
		} else { // this is right child
			parent->rotate_left();
			parent->rotate_right();
			left->is_red = false;
			highest_changed = fix_up();
		}
	}
	return highest_changed;
}


template<typename Tval>
RedBlackTree<Tval>::Node *RedBlackTree<Tval>::Node::add(const Tval& new_val) {
	//NOTE(Gerald): we track the highest node in the tree that is affected
	//we return it so tree can check if it's the new root and update if needed
	Node *highest_changed = find(new_val);
	if (highest_changed) {
		return highest_changed;
	}
	
	RedBlackTree::Node *new_node = new RedBlackTree::Node{new_val};
	new_node->is_red = true;
	append_leaf( new_node );
	
	highest_changed = new_node->fix_up();
	
	return highest_changed;
}

template<typename Tval>
void RedBlackTree<Tval>::add(const Tval& new_val) {
	Node *highest_changed = root->add(new_val);
	if (highest_changed->is_root()) {
		root = highest_changed;
	}
	return;
}


template <typename Tval>
int get_shortest_path_length(typename RedBlackTree<Tval>::Node *tree) {
	if (!tree) return 0;
	int shortest = 1 + min<int>(get_shortest_path_length<Tval>(tree->left), get_shortest_path_length<Tval>(tree->right));
	return shortest;
}


template <typename Tval>
int get_longest_path_length(typename RedBlackTree<Tval>::Node *tree) {
	
	if (!tree) return 0;
	int longest = 1 + max<int>(get_longest_path_length<Tval>(tree->left), get_longest_path_length<Tval>(tree->right));
	
	return longest;	
}

template <typename Tval>
int black_height(typename RedBlackTree<Tval>::Node *tree) {
	//NOTE(Gerald): empty children of a red node count as black. but an empty tree has height 0.
	//thus, this will give the wrong result for empty trees.
	//to fix this we could make this a member function. then we can test for parent != null.
	int result = 1; 
	if (!tree) return result;
	result = tree->is_red ? 0 : 1;
	result += max<int>(black_height(tree->left), black_height(tree->right));
		
	return result;
}

template <typename Tval>
bool RedBlackTree<Tval>::Node::is_black() {	
	bool result = !is_red;
	return result;
}
	
template <typename Tval>
int count_children(typename RedBlackTree<Tval>::Node *tree) {
	//counts only non-empty children
	int result = 0;
	
	if (!tree) return result;
	if (tree->left) ++result;
	if (tree->right) ++result;
	
	return result;
}	

template <typename Tval>
//the name is misleading. the argument implies as much. the point is to check for validity
//TODO(Gerald, 2025 03 19): this function has multiple issues. revise.
bool is_red_black_tree(typename RedBlackTree<Tval>::Node *tree) {
	
	bool result = true;
	
	//NOTE(Gerald, 2025 03 19): why, when we check black_height? why only in the root?
	if (tree->is_red) {
		result &= tree->left  ? !tree->left->is_red  : true;
		result &= tree->right ? !tree->right->is_red : true;
	}

	result &= black_height(tree->left) == black_height(tree->right);
	
	//NOTE(Gerald, 2025 03 19): redundant?
	if (is_black(tree) && count_children(tree) == 1) {
		result &= is_red(tree->left) || is_red(tree->right);
	}
	
	return result;
}


template <typename Tval>
void print(Tval value) {	
	assert(false);
	return;
}


template <typename Tval> 
void print(typename RedBlackTree<Tval>::Node *tree) {
	print_red_black_tree_node<Tval>(tree);
}

template <> void print(char c) {
	printf("%c",c);
}

template <> void print(int i) {
	printf("%d",i);
}

template <typename Tval>
void print(RedBlackTree<Tval> *tree) {
	print_red_black_tree_node<Tval>(tree->root);
}



/*
10
|-10
 |-11
 |2
  |0
  |5
|-5
*/
template <typename Tval>
void print_red_black_tree_node(typename RedBlackTree<Tval>::Node *tree, int indent = 0) {
	if (!tree) {
		return;
	}
	
	for (int i = 0 ; i < indent-1 ; ++i) {
		print(' ');
	}
	
	if (indent != 0) {
		print('|');
	}
	
	if(tree->is_red) {
		print('*');
	}
	print(tree->value);
	print('\n');
	
	print_red_black_tree_node<Tval>(tree->left, indent+1);
	print_red_black_tree_node<Tval>(tree->right, indent+1);
	
	return;
}

template <class T>
RedBlackTree<T> rb_from_values(T *values, int count) {
	RedBlackTree<T> rb{values[0]};
	for (int i = 1 ; i < count ; ++i) {
		rb.add(values[i]);
	}
	
	return rb;
}

template<class T>
void RedBlackTree<T>::Node::walk_down() {
	
	if (is_leaf()) {
		// TODO
	} else if (is_root()) {
		if (is_3_node()) {
			// nothing to be done
		} else if (left->is_3_node()) {
			
		} else if (right->is_3_node()) {
			// TODO			
		} else {
			// merge 3 2-nodes into 1 4-node
			assert(left && right && left->is_black() && right->is_black());
			left->is_red = right->is_red = true;			
		}
	} else {
		//TODO
	}
	
	return;
}