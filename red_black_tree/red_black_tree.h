#include <cassert>

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
	
	//store contained values in order
	ValType * inorder_to_buf(ValType *out);	
	
	void fix_upwards_from(RedBlackTree *);
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
	
	return; 
}


template<typename ValType>
void RedBlackTree<ValType>::append_leaf(RedBlackTree *node){	
	//percolate down from root and attach to parent
	
	if (!node) return;
	RedBlackTree *parent = get_insertion_parent( node->value );
	parent->attach_child(node);
	
	return;
}


template<typename ValType>
void RedBlackTree<ValType>::fix_upwards_from(RedBlackTree *) {	
	
	return;
}


template<typename ValType>
void RedBlackTree<ValType>::add(const ValType& new_val) {
	
	if (find(new_val)) return;
	RedBlackTree *new_node = new RedBlackTree{new_val};
	append_leaf( new_node );
	fix_upwards_from(new_node);
	
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

