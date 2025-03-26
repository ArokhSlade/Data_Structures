#include <cassert>
#include <cstdio>

#include<string>
#include<sstream>

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
		Tval key;
		
		Node *parent = nullptr;
		Node *left = nullptr;
		Node *right = nullptr;
		
		bool is_red = false;
		
		Node *find(Tval search_key);		
		int count();		
		Node *get_insertion_parent(Tval new_key);
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
		bool is_2_node();
		Node *get_sibling();
		Node *get_far_sibling();
		Node *fix_up();
		Node *fix_up_until(Node *);
		Node *replace_right(Node *);
		Node *replace_left(Node *);
		Node *replace_child(Node *old_child, Node *new_child);
		Node *replace_with(Node *replacement);
		Node *shift();
		Node *shift_right();
		Node *shift_left();
		Node *far_shift();		
		void squash();
		Node *far_squash();
		Node *nearest_node(Tval target_key);
		Node *walk_down_step_root(Tval target_key);
		Node *walk_down_step(Tval target_key);
		void enforce_degree_greater_2();
		Node *remove_min_under_3_node();
		Node *remove_leaf();
		Node *descend_left();
		
		//store contained keys in order
		Tval * inorder_to_buf(Tval *out);	
		
		Node(Tval key_) :key{key_}
		{}
		
		void debug_add_left(Tval new_val, bool set_red);
		void debug_add_right(Tval new_val, bool set_red);
		
	};
	
	Node *root;
	
	void add(const Tval& new_val);
	Node *remove(const Tval& taret_key);
	Node *walk_down(Tval target_key);
	void update_root();
	bool to_string(char *out, int size);
	
	RedBlackTree(Tval key_)
	:root{new Node{key_}}
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
RedBlackTree<Tval>::Node *RedBlackTree<Tval>::Node::find(Tval search_key) {	
	Node *result = nullptr;
	if (key == search_key) {
		result = this;
	} else if (search_key < key) {
		result = left ? left -> find( search_key ) : nullptr;
	} else {
		result = right ? right -> find( search_key ) : nullptr ;
	}
	
	return result;
}


template<typename Tval>
Tval *RedBlackTree<Tval>::Node::inorder_to_buf(Tval *out) {
	Tval *next_slot = out;
	
	next_slot = left ? left -> inorder_to_buf(next_slot) : next_slot;
	*next_slot++ = key;
	next_slot = right ? right -> inorder_to_buf(next_slot) : next_slot;
	
	return next_slot;
}


/**
 *  @return nullptr if already exists, pointer to parent otherwise
 */
template<typename Tval>
RedBlackTree<Tval>::Node *RedBlackTree<Tval>::Node::get_insertion_parent(Tval new_key) {
	
	RedBlackTree::Node *result = nullptr;
	
	if (key == new_key) {
		result = nullptr;
	} else if (new_key < key) {
		result = left ? left -> get_insertion_parent( new_key ) : this;
	} else {
		result = right ? right -> get_insertion_parent( new_key ) : this;
	}
	
	return result;
}


template<typename Tval>
void RedBlackTree<Tval>::Node::attach_child(RedBlackTree::Node *node) {
	assert(node->key != key);
	if (node->key < key) {
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
	RedBlackTree::Node *insertion_parent = get_insertion_parent( node->key );
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
	bool result = right == nullptr;
	return result;
}

template<class T>
bool RedBlackTree<T>::Node::is_2_node() {
	//in this implementation red nodes always lean left unless during transformations	
	assert(!right || !right->is_red); //don't call this function during transformations.
	assert(!is_red); //ensure this function is only called on black nodes.
	bool result = !left || !left->is_red; 
	return result;
}

template<typename Tval>
bool RedBlackTree<Tval>::Node::is_3_node(){
	//in this implementation red nodes always lean left unless during transformations	
	assert(!right || !right->is_red); //don't call this function during transformations.
	
	bool result = left && left->is_red;
	return result;
}

template<typename Tval>
RedBlackTree<Tval>::Node *RedBlackTree<Tval>::Node::get_sibling(){		
	Node *sibling = nullptr;
	if (parent) {
		if (parent->left == this) {
			sibling = parent->right;
		} else {
			sibling = parent->left;
		}
	}
	assert(sibling); //NOTE(Gerald, 2025 03 25): for now, assume this function is only called when sibling's existed is expected
	return sibling;
}

template<typename Tval>
//sibling in the sense of belonging to the same 3-node.
//assumes this is the right child of a black node whose left child is red
//gets the nearest (left) sibling. "far" in the sense, that it has another parent.
RedBlackTree<Tval>::Node *RedBlackTree<Tval>::Node::get_far_sibling(){		
	assert(parent && parent->right == this);
	assert(parent->is_black());
	assert(parent->left && parent->left->is_red);
	
	Node *sibling = parent->left->right;
	return sibling;
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
//"become the right child of my left child"
void RedBlackTree<Tval>::Node::rotate_right(){
	assert(left);
	
	RedBlackTree<Tval>::Node *old_parent = parent;
	
	Node *new_left = left->replace_right(this);
	replace_left(new_left);	
	
	parent->parent = old_parent;
	if(old_parent) {
		old_parent->replace_child(this, parent);
	}

	return;
}

template<typename Tval>
RedBlackTree<Tval>::Node *RedBlackTree<Tval>::Node::replace_with(Node *replacement) {
	parent->replace_child(this, replacement);	
	replacement->replace_left(left);
	replacement->replace_right(right);
	return this;
}

template<typename Tval>
//"become the left child of my right child"
void RedBlackTree<Tval>::Node::rotate_left(){
	assert(right);
	
	RedBlackTree<Tval>::Node *old_parent = parent;
	
	Node *new_right = right->replace_left(this);
	replace_right(new_right);
		
	parent->parent = old_parent;
	if(old_parent) {
		old_parent->replace_child(this, parent);
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
RedBlackTree<Tval>::Node *RedBlackTree<Tval>::Node::fix_up_until(typename RedBlackTree<Tval>::Node *limit) {	
	assert(is_red);	
	
	if (this == limit) {
		return this;
	}
	//NOTE(Gerald): we track the highest node in the tree that is affected
	//we return it so tree can check if it's the new root and update if needed
	//TODO(Gerald): this shouldn't be necessary because we expect to stop before some limit node, thus never affecting the root
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
				highest_changed = parent->fix_up_until(limit);
			}
		}		
	} else { //parent is red
		if (parent->left == this) {			
			assert(parent->parent);
			parent->parent->rotate_right();
			is_red = false;
			highest_changed = parent->fix_up_until(limit);
		} else { // this is right child
			parent->rotate_left();
			parent->rotate_right();
			left->is_red = false;
			highest_changed = fix_up_until(limit);
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

template<class T>
//TODO: what if replacement has different color than original?
RedBlackTree<T>::Node *RedBlackTree<T>::remove(const T& target_key) {
	Node *target_node = walk_down(target_key); 
	Node *replacement = nullptr;
	if (target_node->is_leaf()) {
		replacement = target_node->remove_leaf();		
	} else {
		target_node->right->enforce_degree_greater_2();
		replacement = target_node->right->remove_min_under_3_node();
		target_node->replace_with(replacement);		
	}
	replacement->fix_up();
	return target_node;
}

template<typename Tval>
void RedBlackTree<Tval>::add(const Tval& new_val) {
	Node *highest_changed = root->add(new_val);
	if (highest_changed->is_root()) {
		root = highest_changed;
	}
	return;
}

template<class T>
//I'm a leaf or only-parent of a red child. 
//after removing myself I return my replacement.
RedBlackTree<T>::Node *RedBlackTree<T>::Node::remove_leaf() {	
	assert(is_leaf());
	assert(!left || left->is_red);	
	
	
	if (left) {		
		left->is_red = false;
	}
	
	Node *replacement = left ? left : parent;
	
	parent->replace_child(this, left);
	parent = nullptr;
	
	return replacement;
}

template<typename Tval>
void RedBlackTree<Tval>::update_root() {
	while (root->parent) {
		root = root->parent;
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
void print(Tval key) {	
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




struct StringBuffer{
	char *base;
	int fill;
	int cap;
	
	StringBuffer(int cap_) 
	: base{new char[cap_]{0}}, fill{0}, cap{cap_}
	{
		for (char *cur = base; cur < base+cap ; ++cur){
			assert(*cur == '\0');
		}
	}
	
	~StringBuffer(){
		delete[] base;
	}
	
	int space(bool with_null = true) {
		int result = cap - fill;
		return result;
	}
	
	char *get_buf() {
		char *result = (space() > 0) ? base + fill : nullptr;
		return result;
	}
	
	void modify_fill(int fill_change){
		fill += fill_change;
		return;
	}
	
	bool full() {		
		return fill >= cap;
	}
	
	template<typename T>
	bool put(T value);
	
	void print() {
		printf(base);
	}
	
};

//@return true if value was printed completely
template<class T> bool StringBuffer::put(T value) {
	assert(false);
	return false;
}

template <class Tval>
bool traverse_and_print_nodes_to(StringBuffer *buf, typename RedBlackTree<Tval>::Node *tree, int indent = 0) {
	if (!tree) {
		return true;
	}
	if (!buf || buf->full()) {
		return false;
	}
	
	bool no_errors = true;
	
	for (int i = 0 ; i < indent-1 ; ++i) {
		no_errors &= buf->put(' ');
	}
	
	if (indent != 0) {
		no_errors &= buf->put('|');
	}
	
	if(tree->is_red) {
		no_errors &= buf->put('*');
	}
	no_errors &= buf->put(tree->key);
	no_errors &= buf->put('\n');
	
	if (!buf->full()) no_errors &= traverse_and_print_nodes_to<Tval>(buf, tree->left, indent+1);
	if (!buf->full()) no_errors &= traverse_and_print_nodes_to<Tval>(buf, tree->right, indent+1);
	
	return no_errors;
}

template <> bool StringBuffer::put(char c) {	
	char *buf_mem = get_buf();
	if (buf_mem) {
		sprintf(buf_mem, "%c", c);
		modify_fill(1);
		return true;
	}		
	return false;
}

template <> bool StringBuffer::put(int i) {
	
	char *buf_mem = get_buf();
	if (buf_mem) {
		/* NOTE(Gerald, 2025 03 26):
		   https://en.cppreference.com/w/cpp/io/c/fprintf
		   on snprintf return value:
		   """ 
		   4) Number of characters that would have been written for a sufficiently large buffer if successful (not including the terminating null character), or a negative value if an error occurred. Thus, the (null-terminated) output has been completely written if and only if the returned value is nonnegative and less than buf_size. """
		 */
		int space_before = space();
		int return_code = snprintf(buf_mem, space_before, "%d", i);
		bool write_successful = return_code >= 0 && return_code < space_before;		
		if (write_successful) {
			int chars_written = return_code;
			modify_fill(chars_written);			
			return true;
		} else {
			if (return_code > 0) {
				//didn't fit
				assert(return_code >= space_before);
				fill = cap;	
			} else {
				// and "error occcured" in snprintf
				assert(false);
			}
		}
	}	
	return false;
}






// template <typename T>
// void RedBlackTree<T>::to_string_step(typename RedBlackTree<T>::Node node, StringBuffer *buf, int indent = 0) {
	
	
	// return
// }

template <typename Tval>
bool RedBlackTree<Tval>::to_string(char *out, int size){
	
	StringBuffer buf{size};
	
	bool fits = traverse_and_print_nodes_to<Tval>(&buf, root);
	
	if (fits) {
		for (char *src = buf.base, *dst = out ; src <= buf.base+buf.fill ; ++src, ++dst) {
			*dst = *src;
		}
	}
	
	return fits;
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
	print(tree->key);
	print('\n');
	
	print_red_black_tree_node<Tval>(tree->left, indent+1);
	print_red_black_tree_node<Tval>(tree->right, indent+1);
	
	return;
}

template <class T>
RedBlackTree<T> rb_from_keys(T *keys, int count) {
	RedBlackTree<T> rb{keys[0]};
	for (int i = 1 ; i < count ; ++i) {
		rb.add(keys[i]);
	}
	
	return rb;
}

template<typename Tval>
RedBlackTree<Tval>::Node *RedBlackTree<Tval>::Node::replace_child(typename RedBlackTree<Tval>::Node *old_child, typename RedBlackTree<Tval>::Node *new_child) {
	RedBlackTree<Tval>::Node *replaced = nullptr;
	
	if (left == old_child) {
		replaced = replace_left(new_child);
	} else if (right == old_child) {
		replaced = replace_right(new_child);
	} else {
		assert(false);
	}
	return replaced;
}

template<class T>
RedBlackTree<T>::Node *RedBlackTree<T>::Node::replace_right(RedBlackTree<T>::Node *new_right) {
	RedBlackTree<T>::Node *old_right = right;	
	right = new_right;
	if (right) {
		right->parent = this;
	}
	return old_right;
}


template<class T>
RedBlackTree<T>::Node *RedBlackTree<T>::Node::replace_left(RedBlackTree<T>::Node *new_left) {
	RedBlackTree<T>::Node *old_left = left;
	
	left = new_left;
	if (left) {
		left->parent = this;
	}
	
	return old_left;
}

template<class T>
RedBlackTree<T>::Node *RedBlackTree<T>::Node::shift_right() {
	assert(left && left->is_3_node() && right && right->is_2_node());	
	
	Node *replacement = left;
	rotate_right();
	
	assert(parent->left->is_red);
	parent->left->is_red = false;
	
	rotate_left();
	
	if (is_black()) {
		is_red = true;
	} else {
		assert(parent->parent->is_black());
		parent->parent->is_red = true;
	}
	
	return replacement;	
}

template<class T>
RedBlackTree<T>::Node *RedBlackTree<T>::Node::shift_left() {
	assert(left && left->is_2_node() && right && right->is_3_node());
	
	
	right->rotate_right();
	rotate_left();
	Node *replacement = parent;
	assert(parent->is_red && left->is_black());
	left->is_red = true;
	
	assert(parent->is_red);
	if (is_red) {
		is_red = false;
	} else {
		parent->is_red = false;
	}
	
	
	return replacement;	
}

template<class T>
RedBlackTree<T>::Node *RedBlackTree<T>::Node::shift() {
	assert(left && right);
	Node *replacement = nullptr;
	
	if (left->is_2_node()) {
		replacement = shift_left();
	} else {
		replacement = shift_right();
	}
	
	return replacement;
}

template<class T>
RedBlackTree<T>::Node *RedBlackTree<T>::Node::far_shift() {
	//supposed to solve a specific case
	assert(left && right && left->right && left->right->left);
	assert(left->is_red && left->right->left->is_red);
	left->rotate_left();
	left->left->right->is_red = false;
	rotate_right(); //become the right child of my left child.
	Node *replacement = parent;
	rotate_left();
	is_red = true;
	
	return replacement;
}


template<class T>
void RedBlackTree<T>::Node::squash() {
	assert(left && left->is_2_node() && right && right->is_2_node());
	left->is_red = right->is_red = true;
	is_red = false;
	return;
}

template<class T>
RedBlackTree<T>::Node *RedBlackTree<T>::Node::far_squash() {
	assert(left && right);
	
	rotate_right();
	squash();	
	
	assert(parent->is_red);
	parent->is_red = false;
	
	Node *replacement = parent;
	
	return replacement;
}

template<class T>
RedBlackTree<T>::Node *RedBlackTree<T>::Node::nearest_node(T target_key) {
	if (target_key == key) {
		return this;
	} else if (target_key < key) {
		return left;
	} else { //target_key > key
		return right;
	}
}


template<class T>
RedBlackTree<T>::Node *RedBlackTree<T>::Node::walk_down_step_root(T target_key) {
	assert(is_root());
	if (!left && !right) {
		assert(target_key == key);
		return this; //special case handled in walk_down_step
	}
	
	if (is_3_node()) {
		return nearest_node(target_key);
	}
	
	if (left && left->is_2_node() && right && right->is_2_node()) {
		squash();
		return nearest_node(target_key);
	}
	
	//one child is 3-node
	if (target_key >= key) {
		if (right->is_2_node()) {
			shift_right();
			return parent; //old right is now our parent, target_key could be greater than theirs
		} else if (target_key > key) {
			return right;
		} else { // target_key == key
			return this; //doubly-special case. returning a 2-node without parent. handled in walk_down_step()
		}
	} else { // target_key < key
		if (left->is_2_node()) {
			shift_left();
			return this;
		} else {
			return left;
		}
	}
	
	assert(false);
	return nullptr;
}

template<class T>
RedBlackTree<T>::Node *RedBlackTree<T>::Node::walk_down_step(T target_key) {
	if (is_root()) { //special case where walk_down_step_root returned itself as 2-node
		assert(target_key == key); 
		return this;
	}
	
	if (is_3_node() || is_red) {
		return nearest_node(target_key);
	}
	
	//i am a black 2-node
	if (parent->is_red) {
		if (get_sibling()->is_2_node()) {
			parent->squash();
			return nearest_node(target_key);
		} else { // sibling is 3-node
			parent->shift();
			return nearest_node(target_key);
		}	
	} else { // parent is black
		// i am black right child of 3-node
		if (get_far_sibling()->is_2_node()) {
			parent->far_squash();
			return nearest_node(target_key);
		} else {
			parent->far_shift();
			return nearest_node(target_key);
		}
	}
	
	assert(false);
	return nullptr;
}

template<class T>
void RedBlackTree<T>::Node::enforce_degree_greater_2() {
	assert(!is_root());
	assert(parent->is_3_node() || parent->is_red);
	
	if (is_3_node() || is_red) {
		return;
	}
	
	//i am a black 2-node
	if (parent->is_red) {
		if (get_sibling()->is_2_node()) {
			parent->squash();
		} else { // sibling is 3-node
			parent->shift();
		}	
	} else { // parent is black
		// i am black right child of 3-node
		if (get_far_sibling()->is_2_node()) {
			parent->far_squash();
		} else {
			parent->far_shift();
		}
	}
	return;
}


template<class T>
RedBlackTree<T>::Node *RedBlackTree<T>::Node::remove_min_under_3_node(){
	assert(parent->is_red || parent->is_3_node());
	Node *current = this;
	while (current->left) {
		current->descend_left();
	}
	assert(current->is_red && current->parent->left == current);
	current->parent->left = nullptr;
	current->parent->fix_up_until(this);
	current->parent = nullptr;
	return current;
}


template<class T>
RedBlackTree<T>::Node *RedBlackTree<T>::Node::descend_left(){
	assert(parent->left == this);
	//nothing to do if is_red
	if (is_black()) {
		if (get_sibling()->is_2_node()) {
			parent->squash();
		} else { //sibling is 3-node
			parent->shift_left();
		}
	}
	return left;
}

template<class T>
RedBlackTree<T>::Node *RedBlackTree<T>::walk_down(T target_key) {
	Node *target_node = root->walk_down_step_root(target_key);
	target_node = root->walk_down_step(target_key);
	
	return target_node;
}

template<class T>
void RedBlackTree<T>::Node::debug_add_left(T new_val, bool set_red) {
	left = new Node{new_val};
	left->parent = this;
	left->is_red = set_red;
	return;
}

template<class T>
void RedBlackTree<T>::Node::debug_add_right(T new_val, bool set_red) {
	right = new Node{new_val};
	right->parent = this;
	right->is_red = set_red;
	return;
}