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
	int shortest = 1 + min<ValType>(get_shortest_path_length(tree->left), get_shortest_path_length(tree->right));
	return shortest;
}


template <typename ValType>
int get_longest_path_length(RedBlackTree<ValType> *tree) {
	
	if (!tree) return 0;
	int longest = 1 + max<ValType>(get_longest_path_length<ValType>(tree->left), get_longest_path_length<ValType>(tree->right));
	
	return longest;	
}

template <typename ValType>
int black_height(RedBlackTree<ValType> *tree) {
	int result = 1; //TODO: what's the black height of an empty tree? 1, if it's an empty leaf; 0, if it's an empty root?
	if (!tree) return result;
	result = tree->is_red ? 0 : 1;
	result += max(black_height(tree->left), black_height(tree->right));
		
	return result;
}

template <typename ValType>
bool is_black(RedBlackTree<ValType> *tree) {
	bool result = true;
	if (!tree) return result;
	result = !tree->is_red;
	return result;
}
	
template <typename ValType>
int count_children(RedBlackTree<ValType> *tree) {
	int result = 0;
	
	if (!tree) return result;
	if (tree->left) ++result;
	if (tree->right) ++result;
	
	return result;
}	

template <typename ValType>
bool is_red_black_tree(RedBlackTree<ValType> *tree) {
	
	bool result = true;
	
	if (tree->is_red) {
		result &= tree->left  ? !tree->left->is_red  : true;
		result &= tree->right ? !tree->right->is_red : true;
	}

	result &= black_height(tree->left) == black_height(tree->right);
	
	//NOTE: redundant, theoretically
	if (is_black(tree) && count_children(tree) == 1) {
		result &= is_black(tree->left);
		result &= is_black(tree->right);
	}
	
	return result;
}

