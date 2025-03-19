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

template<typename ValType>
struct RedBlackTree {	
	
	bool is_red = false;
	ValType value;
	
	RedBlackTree *parent = nullptr;
	RedBlackTree *left = nullptr;
	RedBlackTree *right = nullptr;
	
	RedBlackTree(ValType value_)
	:value(value_)
	{}	
	
	RedBlackTree *find(ValType search_value);
	
	int count();
	
	RedBlackTree *get_insertion_parent(ValType new_value);
	void attach_child(RedBlackTree *new_node);
	void append_leaf(RedBlackTree *new_node);
	void add(const ValType&);
	void flip_colors_with_children();
	void flip_colors_with_parent();
	void rotate_right();
	void rotate_left();
	bool is_root();
	void fix_up();
	
	//store contained values in order
	ValType * inorder_to_buf(ValType *out);	
	
};

// template<typename ValType>
// using RedBlackTree = RedBlackTree<ValType>;

template <typename ValType>
int RedBlackTree<ValType>::count() {
	int result = 1;
	if (left) result += left->count();
	if (right) result += right->count();
	return result;
}

template<typename ValType>
RedBlackTree<ValType> *RedBlackTree<ValType>::find(ValType search_value) {	
	RedBlackTree *result = nullptr;
	if (value == search_value) {
		result = this;
	} else if (search_value < value) {
		result = left ? left -> find( search_value ) : nullptr;
	} else {
		result = right ? right -> find( search_value ) : nullptr ;
	}
	
	return result;
}


template<typename ValType>
ValType *RedBlackTree<ValType>::inorder_to_buf(ValType *out) {
	ValType *next_slot = out;
	
	next_slot = left ? left -> inorder_to_buf(next_slot) : next_slot;
	*next_slot++ = value;
	next_slot = right ? right -> inorder_to_buf(next_slot) : next_slot;
	
	return next_slot;
}


/**
 *  @return nullptr if already exists, pointer to parent otherwise
 */
template<typename ValType>
RedBlackTree<ValType> *RedBlackTree<ValType>::get_insertion_parent(ValType new_value) {
	
	RedBlackTree *result = nullptr;
	
	if (value == new_value) {
		result = nullptr;
	} else if (new_value < value) {
		result = left ? left -> get_insertion_parent( new_value ) : this;
	} else {
		result = right ? right -> get_insertion_parent( new_value ) : this;
	}
	
	return result;
}


template<typename ValType>
void RedBlackTree<ValType>::attach_child(RedBlackTree *node) {
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


template<typename ValType>
void RedBlackTree<ValType>::append_leaf(RedBlackTree *node){	
	//percolate down from root and attach to parent
	
	if (!node) return;
	RedBlackTree *insertion_parent = get_insertion_parent( node->value );
	insertion_parent->attach_child(node);
	
	return;
}

template<typename ValType>
bool RedBlackTree<ValType>::is_root(){
	bool result = parent == nullptr;
	return result;
}

template<typename ValType>
void RedBlackTree<ValType>::flip_colors_with_parent(){
	assert(parent);
	assert(is_red != parent->is_red);
	
	parent->is_red = is_red;
	is_red = !is_red;
	
	return;
}

template<typename ValType>
void RedBlackTree<ValType>::flip_colors_with_children(){
	assert(left && right);
	assert(left->is_red == right->is_red && left->is_red != is_red);
	
	left->is_red = right->is_red = is_red;
	is_red = !is_red;	
	
	return;
}

template<typename ValType>
void RedBlackTree<ValType>::rotate_right(){
	RedBlackTree<ValType> *old_parent = parent;
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

template<typename ValType>
void RedBlackTree<ValType>::rotate_left(){
	RedBlackTree<ValType> *old_parent = parent;
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

template<typename ValType>
void RedBlackTree<ValType>::fix_up() {	
	assert(is_red);	
	if (is_root()) {
		is_red = false;
		return;
	}
	assert(parent);
	if (is_black(parent)) {
		if (parent->left == this) {
			return;
		} else { // this is right child
			if (!parent->left) {
				parent->rotate_left();
				left->flip_colors_with_parent();
			} else {
				assert(parent->left->is_red);
				parent->flip_colors_with_children();
				parent->fix_up();
			}
		}		
	} else { //parent is red
		if (parent->left == this) {			
			assert(parent->parent);
			parent->parent->rotate_right();
			is_red = false;
			parent->fix_up();
		} else { // this is right child
			parent->rotate_left();
			parent->rotate_right();
			left->is_red = false;
			fix_up();
		}
	}
	return;
}


template<typename ValType>
void RedBlackTree<ValType>::add(const ValType& new_val) {
	
	if (find(new_val)) return;
	RedBlackTree *new_node = new RedBlackTree{new_val};
	new_node->is_red = true;	
	append_leaf( new_node );
	new_node->fix_up();
	
	return;
}


template <typename ValType>
int get_shortest_path_length(RedBlackTree<ValType> *tree) {
	if (!tree) return 0;
	int shortest = 1 + min<int>(get_shortest_path_length(tree->left), get_shortest_path_length(tree->right));
	return shortest;
}


template <typename ValType>
int get_longest_path_length(RedBlackTree<ValType> *tree) {
	
	if (!tree) return 0;
	int longest = 1 + max<int>(get_longest_path_length<ValType>(tree->left), get_longest_path_length<ValType>(tree->right));
	
	return longest;	
}

template <typename ValType>
int black_height(RedBlackTree<ValType> *tree) {
	//NOTE(Gerald): empty children of a red node count as black. but an empty tree has height 0.
	//thus, this will give the wrong result for empty trees.
	//to fix this we could make this a member function. then we can test for parent != null.
	int result = 1; 
	if (!tree) return result;
	result = tree->is_red ? 0 : 1;
	result += max<int>(black_height(tree->left), black_height(tree->right));
		
	return result;
}

template <typename ValType>
bool is_black(RedBlackTree<ValType> *tree) {
	if (!tree) return true; //empty children count as black
	bool result = !tree->is_red;
	return result;
}
	
template <typename ValType>
int count_children(RedBlackTree<ValType> *tree) {
	//counts only non-empty children
	int result = 0;
	
	if (!tree) return result;
	if (tree->left) ++result;
	if (tree->right) ++result;
	
	return result;
}	

template <typename ValType>
//the name is misleading. the argument implies as much. the point is to check for validity
//TODO(Gerald, 2025 03 19): this function has multiple issues. revise.

bool is_red_black_tree(RedBlackTree<ValType> *tree) {
	
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


template <typename ValType>
void print(ValType value) {	
	assert(false);
	return;
}


template <typename ValType> 
void print(RedBlackTree<ValType> *tree) {
	print_red_black_tree<ValType>(tree);
}

template <> void print(char c) {
	printf("%c",c);
}

template <> void print(int i) {
	printf("%d",i);
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
template <typename ValType>
void print_red_black_tree(RedBlackTree<ValType> *tree, int indent = 0) {
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
	
	print_red_black_tree(tree->left, indent+1);
	print_red_black_tree(tree->right, indent+1);
	
	return;
}