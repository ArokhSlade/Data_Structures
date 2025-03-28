#include "red_black_tree.h"

#include<iostream>
#include<string>
#include<sstream>

#define array_count(array) (sizeof(array)/sizeof(array[0]))

template<typename T>
std::string array_to_string(T *array, int count) {
	std::stringstream stream;
	if (!array) return "";
	
	stream << "{";
	if (count > 0) {
		stream << array[0];
	}
	for (int i = 1 ; i < count ; ++i ) {
		stream << ", " << array[i];
	}
	stream << "}";
	std::string result = stream.str();
	return result;
}

template <typename T>
using RBTree = RedBlackTree<T>;
template <typename T>
using Node = RBTree<T>::Node;

using std::cout;

template<class T>
void print_test(char *name, T *keys, int count, RBTree<T> *rb) {
	cout << "test : " << name << '\n';
	cout << array_to_string(keys, count) << '\n';
	print(rb);
	return;
}

void test_01() {
	int test_key = 1;
	RBTree<int> rb{test_key};
	Node<int> *root = rb.root->find(test_key);
	
	bool OK = true;
	OK &= root != nullptr;
	OK &= root->key == test_key;
	
	cout << "test: find root\n";	
	cout << (OK ? "OK" : "ERROR") << "\n";	
	cout << "\n";
	
	return;
}


void test_02() {
	int test_key = 1;
	RBTree<int> rb{test_key};	
	rb.add(-1);
		
	bool OK = true;
	OK &= rb.root->find(1) != nullptr;	
	OK &= rb.root->find(-1) != nullptr;
	
	cout << "test: add and find 2 elements\n";
	cout << (OK ? "OK" : "ERROR") << "\n";	
	cout << "\n";
	
	return;
}


void test_03() {
	int test_key = 0;
	RBTree<int> rb{test_key};
	rb.add(-1);
	rb.add(1);
		
	bool OK = true;
	OK &= rb.root->find(0) != nullptr;	
	OK &= rb.root->find(-1) != nullptr;
	OK &= rb.root->find(1) != nullptr;	
	
	int exp[] = {-1,0,1};
	int out[ array_count(exp) ];
	
	rb.root->inorder_to_buf(out);
	for (int i = 0 ; i < array_count(out) ; ++i) {
		OK &= out[i] == exp[i];
	}	
	
	cout << "test: add 3 elements and output the keys in-order\n";
	cout << "result  : " << array_to_string<int>(out, array_count(out)) << "\n";
	cout << "expected: " << array_to_string<int>(exp, array_count(exp)) << "\n";
	cout << (OK ? "OK" : "ERROR") << "\n";	
	cout << "\n";
	
	return;
}



void test_04(){
	RBTree<int> rb{0};
	
	rb.add(0);
	rb.add(1);
	rb.add(1);
	rb.add(-1);
	rb.add(-1);
	
	bool OK = true;


	int exp[] = {-1,0,1};
	int out[ 6 ];
	
	rb.root->inorder_to_buf(out);
	
	int rb_count = rb.root->count();
	OK &= rb_count == 3;
	
	for (int i = 0 ; i < rb_count ; ++i) {
		OK &= exp[i] == out[i];
	}	
	
	cout << "test: add 6 elements with duplicates, 3 of which are duplicates\n";
	cout << (OK ? "OK" : "ERROR") << "\n";	
	cout << "result  : " << array_to_string<int>(out, rb_count) << "\n";
	cout << "expected: " << array_to_string<int>(exp, array_count(exp)) << "\n";	
	cout << "\n";
}


void test_05(){
	RBTree<int> rb{0};
	
	rb.add(14);
	rb.add(5);
	rb.add(-1);
	rb.add(-3);	
	rb.add(8);	
	
	bool OK = true;


	int exp[] = {-3,-1,0,5,8,14};
	int out[ array_count(exp) ];
	
	rb.root->inorder_to_buf(out);
	
	cout << "test: add 6 elements unsorted, and output the keys in-order\n";
	cout << "result  : " << array_to_string<int>(out, array_count(out)) << "\n";
	cout << "expected: " << array_to_string<int>(exp, array_count(exp)) << "\n";	
	cout << "\n";
}

void test_06(){
	RBTree<int> rb{0};	
	print(&rb);
	cout << "---\n";
	rb.add(-1);
	print(&rb);
	cout << "---\n";
	rb.add(-2);
	print(&rb);
	cout << "---\n";
	rb.add(-3);
	print(&rb);	
	cout << "---\n";
	rb.add(-4);
	print(&rb);
	cout << "---\n";
	rb.add(-5);
	print(&rb);
	cout << "---\n";
	
	
	bool OK = true;
	
	int out[6] = {};
	rb.root->inorder_to_buf(out);
	
	int shortest_path_length = get_shortest_path_length<int>(rb.root);
	int longest_path_length = get_longest_path_length<int>(rb.root);
	
	cout << "test: shortest and longest path\n";
	cout << array_to_string(out,array_count(out)) << "\n";
	cout << "shortest: " << shortest_path_length << "\n";
	cout << "longest: " << longest_path_length << "\n";
	cout << "\n";
}


void test_07(){
	RBTree<int> rb{0};	
	
	rb.add(-1);
	rb.add(1);
	rb.add(-2);	

	RBTree<int>::Node *temp = rb.root;
	temp->is_red = false;
	temp = rb.root->find(-1);
	
	bool OK = true;
	
	int out[6] = {};
	rb.root->inorder_to_buf(out);
	
	int shortest_path_length = get_shortest_path_length<int>(rb.root);
	int longest_path_length = get_longest_path_length<int>(rb.root);
	
	cout << "test: shortest and longest path\n";
	cout << array_to_string(out,array_count(out)) << "\n";
	cout << "shortest: " << shortest_path_length << "\n";
	cout << "longest: " << longest_path_length << "\n";
	cout << "\n";
}

void test_08(){
	
	int keys[] = {0,-10};
	RBTree<int> rb{keys[0]};
	
	for (int i = 1 ; i < array_count(keys) ; i++) {
		rb.add(keys[i]);
	}	
	
	cout << "test: red node\n";
	cout << array_to_string(keys, array_count(keys)) << '\n';
	print(&rb);
}

void test_09(){
	
	int keys[] = {0,-10,5,-6,3,8,-5};
	RBTree<int> rb{keys[0]};
	
	for (int i = 1 ; i < array_count(keys) ; i++) {
		rb.add(keys[i]);
	}	
	
	cout << "test: print(tree)\n";
	cout << array_to_string(keys, array_count(keys)) << '\n';
	print(&rb);
}

void test_10(){
	
	char keys[] = {'S','E','A','R','C','H','E','X','A','M','P','L','E'};
	RBTree<char> rb{keys[0]};
	
	for (int i = 1 ; i < array_count(keys) ; i++) {
		rb.add(keys[i]);
	}	
	
	cout << "test: \n";
	cout << array_to_string(keys, array_count(keys)) << '\n';
	print(&rb);
}

void test_11(){
	
	char keys[] = {'A','C','E','H','L','M','P','R','S','X'};
	RBTree<char> rb{keys[0]};
	
	for (int i = 1 ; i < array_count(keys) ; i++) {
		rb.add(keys[i]);
	}	
	
	cout << "test: \n";
	cout << array_to_string(keys, array_count(keys)) << '\n';
	print(&rb);
}

void test_12() {
	/*
	walk_down case root #0: root is 3-node -> nothing to be done
	0       -> 0
	> *-1   -> > *-1	
	*/
	char name[] = "walk_down at root, root is 3-node (has red child)";
	int keys[] = {0,-1};
	RBTree<int> rb = rb_from_keys<int>(keys, 2);
	print_test(name, keys, 2, &rb);
	rb.root->walk_down_step_root(-1);
	print(&rb);
	
	return;
}

void test_13() {
	/*
	walk_down case root #1: both children are 2-nodes (no red children)
	0       -> 0
	> -1    -> > *-1
	>  1    -> > * 1
	*/
	char name[] = "walk_down at root, both children 2-nodes";
	int keys[] = {0,-1,1};
	RBTree<int> rb = rb_from_keys<int>(keys, 3);	
	print_test(name, keys, 3, &rb);
	rb.root->walk_down_step_root(1);
	print(&rb);
	
	return;
}


void test_14() {
	/*	
	0    
	|-1  	
	|2   
	 |*1
	*/
	char name[] = "walk_down at root, right child is 3-node, target is greater (2) -> do nothing";
	int keys[] = {0,-1,2,1};
	RBTree<int> rb = rb_from_keys<int>(keys, 4);
	print_test(name, keys, 4, &rb);
	rb.root->walk_down_step_root(2);
	cout << "result: \n";
	print(&rb);
	
	return;
}



void test_15() {	
	char name[] = "replace_right(-2)";
	int keys[] = {0,-1,1,-2};
	RBTree<int> rb = rb_from_keys<int>(keys, 4);
	print_test(name, keys, 4, &rb);
	Node<int> *new_right = rb.root->left->left;
	RedBlackTree<int>::Node *old_right = rb.root->replace_right(new_right);
	cout << "result: \n";
	print(&rb);	
	cout << "old right: \n";
	//print<RedBlackTree<int>::Node *>(old_right); //template specialization not called
	print_red_black_tree_node<int>(old_right);
	
	
	return;
}

void test_16() {	
	char name[] = "replace_right (-1) where right is nullptr";
	int keys[] = {0,-1};
	RBTree<int> rb = rb_from_keys<int>(keys, 2);
	print_test(name, keys, 2, &rb);
	Node<int> *new_right = rb.root->left;
	RedBlackTree<int>::Node *old_right = rb.root->replace_right(new_right);
	cout << "result: \n";
	print(&rb);	
	cout << "old right: \n";
	//print<RedBlackTree<int>::Node *>(old_right); //template specialization not called
	print_red_black_tree_node<int>(old_right);
	
	
	return;
}

void test_17() {
	/*	
	0       -> -1
	|-1     -> |-2
	 |*-2   -> |1
	|1      ->  |*0
	*/
	char name[] = "walk_down at root, right child is 2-node, target is greater (1) -> scooch";
	int keys[] = {0,-1,1,-2};
	RBTree<int> rb = rb_from_keys<int>(keys, 4);
	print_test(name, keys, 4, &rb);
	rb.root->walk_down_step_root(1);
	rb.update_root();
	cout << "result: \n";
	print(&rb);
	
	return;
}


void test_18() {
	/*
	walk_down case root #4: left child is 3_node, target on left side -> do nothing
	0    
	|-1  
	 |*-2
	|1 
	*/
	char name[] = "walk_down at root, left child is 3-node, target is less (-2) -> do nothing";
	int keys[] = {0,-1,1,-2};
	RBTree<int> rb = rb_from_keys<int>(keys, 4);
	print_test(name, keys, 4, &rb);
	rb.root->walk_down_step_root(-2);
	cout << "result: \n";
	print(&rb);
	
	return;
}


void test_19() {
	/*
	walk_down case root #5: left child is 2_node, target on left side -> scooch left
	0		-> 1
	|-1  	-> |0
	|2      ->  |*-1
	 |*1   ->  |2
	*/
	char name[] = "walk_down at root, left child is 2-node, target is less (-1) -> scooch left";
	int keys[] = {0,-1,2,1};
	RBTree<int> rb = rb_from_keys<int>(keys, 4);
	print_test(name, keys, 4, &rb);
	rb.root->walk_down_step_root(-1);
	rb.update_root();
	cout << "result: \n";
	print(&rb);
	
	return;
}


void test_20() {
	/*	
	0
	|-2  <------
	 |*-1
	|1
	
	*/
	char name[] = "walk_down between root and leaf, node is 3-node (-1) -> do nothing";
	RBTree<int> rb{0};
	
	rb.root->debug_add_left(-1, false);
	rb.root->debug_add_right(1, false);
	
	rb.root->left->debug_add_left(-2, true); //leaf
	
	
	cout << "test: " << name << '\n';
	print(&rb);
	rb.root->left->walk_down_step(-2);
	
	cout << "result: \n";
	print(&rb);
	
	return;
}

void test_21() {
	/*
	walk_down case in-between #1:
	p		-> p
	|4      -> |4
	 |*2     ->  |2
	  |1    ->   |*1
	  |3    ->   |*3
	 |5     -> |5
	*/
	char name[] = "walk_down between root and leaf: node (1) is 2-node (left child) with 2-node sibling -> squash";
	
	RBTree<int> rb{-10};
	
	rb.root->debug_add_right(4, false);
	rb.root->right->debug_add_left(2, true);
	rb.root->right->debug_add_right(5, false);
	rb.root->right->left->debug_add_left(1, false);
	rb.root->right->left->debug_add_right(3, false);
	
	rb.root->right->left->left->debug_add_left(0, false); //leaf
	
	cout << "test: " << name << '\n';
	print(&rb);
	cout << "current node: 1\n";
	rb.root->right->left->left->walk_down_step(999); //target doesn't matter for in-between case
	
	cout << "result: \n";
	print(&rb);
	
	return;
}

void test_22() {
	/*
	walk_down case in-between #2:
	p		-> p
	|5      -> |5
	 |*2    ->  |*3
	  |1    ->   |2
	  |4    ->    |*1
	   |*3	->	|4
	 |6     -> |6
	*/
	char name[] = "walk_down between root and leaf: node (1) is 2-node (left child) with 3-node sibling -> scooch left";
	
	RBTree<int> rb{-10};
	
	rb.root->debug_add_right(5, false);
	rb.root->right->debug_add_left(2, true);
	rb.root->right->left->debug_add_left(1, false);
	rb.root->right->left->debug_add_right(4, false);
	rb.root->right->left->right->debug_add_left(3, true);
	rb.root->right->debug_add_right(6, false);
	
	rb.root->right->left->left->debug_add_left(0, false); //leaf below
	
	cout << "test: " << name << '\n';
	print(&rb);
	cout << "current node: 1\n";
	rb.root->right->left->left->walk_down_step(999); //target doesn't matter for in-between case
	
	cout << "result: \n";
	print(&rb);
	
	return;
}


void test_23() {
	/*
	walk_down case in-between #3:
	p		-> p
	|4      -> |2
	 |*2    ->  |1
	  |1    ->  |4
	  |3    ->   |*3
	 |5     ->   |*5
	*/
	char name[] = "walk_down between root and leaf: node (5) is 2-node (right child) with 2-node sibling -> squash";
	
	RBTree<int> rb{-10};
	
	rb.root->debug_add_right(4, false);
	rb.root->right->debug_add_left(2, true);
	rb.root->right->debug_add_right(5, false);
	rb.root->right->left->debug_add_left(1, false);
	rb.root->right->left->debug_add_right(3, false);
	
	rb.root->right->right->debug_add_right(10, false); //leaf
	
	cout << "test: " << name << '\n';
	print(&rb);
	cout << "current node: 5\n";
	rb.root->right->right->walk_down_step(999); //target doesn't matter for in-between case
	
	cout << "result: \n";
	print(&rb);
	
	return;
}


void test_24() {
	/*
	walk_down case in-between #4:
	p		-> p
	|5      -> |4
	 |*2    ->  |*2
	  |1    ->   |1
	  |4    ->   |3
	   |*3	->	|6
	 |6     ->   |*5
	*/
	char name[] = "walk_down between root and leaf: node (6) is 2-node (right child) with 3-node sibling -> scooch right";
	
	RBTree<int> rb{-10};
	
	rb.root->debug_add_right(5, false);
	rb.root->right->debug_add_left(2, true);
	rb.root->right->left->debug_add_left(1, false);
	rb.root->right->left->debug_add_right(4, false);
	rb.root->right->left->right->debug_add_left(3, true);
	rb.root->right->debug_add_right(6, false);
	
	rb.root->right->right->debug_add_right(10, false); //leaf below
	
	cout << "test: " << name << '\n';
	print(&rb);
	cout << "current node: 6\n";
	rb.root->right->right->walk_down_step(999); //target doesn't matter for in-between case
	
	cout << "result: \n";
	print(&rb);
	
	return;
}



void test_25() {
	/*
	walk_down case in-between #5:
	p		-> p
	|4      -> |4
	 |*1    ->  |1
	  |0    ->   |*0
	  |3    ->   |*3
	 |5     ->  |5
	*/
	char name[] = "walk_down between root and leaf: node (3) is 2-node (middle) with 2-node (left) sibling -> squash";
	
	RBTree<int> rb{-10};
	
	rb.root->debug_add_right(4, false);
	rb.root->right->debug_add_left(1, true);
	rb.root->right->debug_add_right(5, false);
	rb.root->right->left->debug_add_left(0, false);
	rb.root->right->left->debug_add_right(3, false);
	
	rb.root->right->left->right->debug_add_left(2, false); //leaf
	
	cout << "test: " << name << '\n';
	print(&rb);
	cout << "current node: 3\n";
	rb.root->right->left->right->walk_down_step(999); //target doesn't matter for in-between case
	
	cout << "result: \n";
	print(&rb);
	
	return;
}


void test_26() {
	/*
	walk_down case in-between #6:
	p		-> p
	|5      -> |5
	 |*1    ->  |*1
	  |0    ->   |0
	  |3    ->   |3
	   |*2	->	  |*2
	 |6     ->  |6 
	*/
	char name[] = "walk_down between root and leaf: node (3) is 2-node (middle child) with 3-node (left) sibling -> scooch right";
	
	RBTree<int> rb{-10};
	
	rb.root->debug_add_right(5, false);
	rb.root->right->debug_add_left(1, true);
	rb.root->right->left->debug_add_left(0, false);
	rb.root->right->left->debug_add_right(3, false);
	rb.root->right->left->right->debug_add_left(2, true);
	rb.root->right->debug_add_right(6, false);
	
	rb.root->right->left->right->debug_add_right(4, false); //leaf below
	
	cout << "test: " << name << '\n';
	print(&rb);
	cout << "current node: 3\n";
	rb.root->right->left->right->walk_down_step(999); //target doesn't matter for in-between case
	
	cout << "result: \n";
	print(&rb);
	
	return;
}

void test_27() {
	/*
	walk_down case leaf #0:
	0		-> 0
	|-5     -> |-5 
	 |-10   ->  |-10      <-------leaf = right key in 3-node
	  |*-11 ->   |*-11    
	 |-3    ->  |-3
	|5      -> |5
	 |*2    ->  |*2
	  |1    ->   |1
	  |4    ->   |4
	 |9     ->  |9
	 
	*/
	char name[] = "walk_down at leaf: node (-10) is right key in 3-node";
	
	RBTree<int> rb{0};
	
	rb.root->debug_add_left(-5, false);
	rb.root->left->debug_add_left(-10, false);
	rb.root->left->left->debug_add_left(-11, true);
	rb.root->left->debug_add_right(-3, false);
	rb.root->debug_add_right(5, false);
	rb.root->right->debug_add_left(2, true);
	rb.root->right->left->debug_add_left(1, false);
	rb.root->right->left->debug_add_right(4, false);
	rb.root->right->debug_add_right(9, false);
	
	cout << "test: " << name << '\n';
	print(&rb);
	cout << "current node: 10\n";
	rb.root->left->left->walk_down_step(10); //target doesn't matter for in-between case
	
	cout << "result: \n";
	print(&rb);
	
	return;
}


void test_28() {
	/*
	walk_down case leaf #1:
	0		-> 0
	|-5     -> |-5 
	 |-10   ->  |-10      <-------leaf = right key in 3-node
	  |*-11 ->   |*-11    
	 |-3    ->  |-3
	|5      -> |5
	 |*2    ->  |*2
	  |1    ->   |1
	  |4    ->   |4
	 |9     ->  |9
	 
	*/
	char name[] = "walk_down at leaf: node (-11) is left key in 3-node";
	
	RBTree<int> rb{0};
	
	rb.root->debug_add_left(-5, false);
	rb.root->left->debug_add_left(-10, false);
	rb.root->left->left->debug_add_left(-11, true);
	rb.root->left->debug_add_right(-3, false);
	rb.root->debug_add_right(5, false);
	rb.root->right->debug_add_left(2, true);
	rb.root->right->left->debug_add_left(1, false);
	rb.root->right->left->debug_add_right(4, false);
	rb.root->right->debug_add_right(9, false);
	
	cout << "test: " << name << '\n';
	print(&rb);
	cout << "current node: -11\n";
	rb.root->left->left->left->walk_down_step(-11); //target doesn't matter for in-between case
	
	cout << "result: \n";
	print(&rb);
	
	return;
}


void test_29() {
	/*
	walk_down case leaf #2:
	0		-> 0
	|-5     -> |-5 
	 |-10   ->  |-10      <-------leaf = right key in 3-node
	  |*-11 ->   |*-11    
	 |-3    ->  |-3
	|5      -> |5
	 |*2    ->  |*2
	  |1    ->   |1
	  |4    ->   |4
	 |9     ->  |9
	 
	*/
	char name[] = "walk_down at leaf: 2-node with 2-node sibling: (1) is left child of red node -> squash";
	
	RBTree<int> rb{0};
	
	rb.root->debug_add_left(-5, false);
	rb.root->left->debug_add_left(-10, false);
	rb.root->left->debug_add_right(-3, false);
	rb.root->left->left->debug_add_left(-11, true);
	rb.root->debug_add_right(5, false);
	rb.root->right->debug_add_left(2, true);
	rb.root->right->left->debug_add_left(1, false);
	rb.root->right->left->debug_add_right(4, false);
	rb.root->right->debug_add_right(9, false);
	
	cout << "test: " << name << '\n';
	print(&rb);
	cout << "current node: 1\n";
	rb.root->right->left->left->walk_down_step(1); //target doesn't matter for in-between case
	
	cout << "result: \n";
	print(&rb);
	
	return;
}


void test_30() {
	/*
	walk_down case leaf #3:
	0		-> 0
	|-5     -> |-5 
	 |-10   ->  |-10      <-------leaf = right key in 3-node
	  |*-11 ->   |*-11    
	 |-3    ->  |-3
	|5      -> |5
	 |*2    ->  |*2
	  |1    ->   |1
	  |4    ->   |4
	 |9     ->  |9
	 
	*/
	char name[] = "walk_down at leaf: 2-node with 2-node sibling: (9) is right child of black node -> squash";
	
	RBTree<int> rb{0};
	
	rb.root->debug_add_left(-5, false);
	rb.root->left->debug_add_left(-10, false);
	rb.root->left->debug_add_right(-3, false);
	rb.root->left->left->debug_add_left(-11, true);
	rb.root->debug_add_right(5, false);
	rb.root->right->debug_add_left(2, true);
	rb.root->right->left->debug_add_left(1, false);
	rb.root->right->left->debug_add_right(4, false);
	rb.root->right->debug_add_right(9, false);
	
	cout << "test: " << name << '\n';
	print(&rb);
	cout << "current node: 9\n";
	rb.root->right->right->walk_down_step(9); //target doesn't matter for in-between case
	
	cout << "result: \n";
	print(&rb);
	
	return;
}


void test_31() {
	/*
	walk_down case leaf #4:
	0		-> 0
	|-5     -> |-5 
	 |-10   ->  |-10      <-------leaf = right key in 3-node
	  |*-11 ->   |*-11    
	 |-3    ->  |-3
	|5      -> |5
	 |*2    ->  |*2
	  |1    ->   |1
	  |4    ->   |4
	 |9     ->  |9
	 
	*/
	char name[] = "walk_down at leaf: 2-node with 2-node sibling: (4) is right child of red node -> squash";
	
	RBTree<int> rb{0};
	
	rb.root->debug_add_left(-5, false);
	rb.root->left->debug_add_left(-10, false);
	rb.root->left->debug_add_right(-3, false);
	rb.root->left->left->debug_add_left(-11, true);
	rb.root->debug_add_right(5, false);
	rb.root->right->debug_add_left(2, true);
	rb.root->right->left->debug_add_left(1, false);
	rb.root->right->left->debug_add_right(4, false);
	rb.root->right->debug_add_right(9, false);
	
	cout << "test: " << name << '\n';
	print(&rb);
	cout << "current node: 4\n";
	rb.root->right->left->right->walk_down_step(4); //target doesn't matter for in-between case
	
	cout << "result: \n";
	print(&rb);
	
	return;
}


void test_32() {	
		/*
	walk_down case leaf #5:
	0		-> 0
	|*-5    -> |*-4 
	 |-10   ->  |-5      <-------leaf = right key in 3-node
	 |-3    ->   |*-10    
	  |*-4  ->  |-3
	|5      -> |5
	*/
	char name[] = "walk_down at leaf: 2-node with 3-node sibling: (-10) is left child (of black node) -> scooch left";
	
	RBTree<int> rb{0};
	
	rb.root->debug_add_left(-5, true);
	rb.root->left->debug_add_left(-10, false);
	rb.root->left->debug_add_right(-3, false);
	rb.root->left->right->debug_add_left(-4, true);
	rb.root->debug_add_right(5, false);
	
	cout << "test: " << name << '\n';
	print(&rb);
	cout << "current node: -10\n";
	rb.root->left->left->walk_down_step(-10); //target doesn't matter for in-between case
	
	cout << "result: \n";
	print(&rb);
	
	return;
}

void test_33() {
		/*
	walk_down case leaf #6:
	0		-> 0		
	|*-4 	-> |*-5    
	 |-5   	->  |-10   
	  |*-10	->  |-3    
	 |-3	->   |*-4   <----------
	|5		-> |5      
	*/
	char name[] = "walk_down at leaf: 2-node (-3) is middle child (right child of red parent) with left sibling being a 3-node";
	
	RBTree<int> rb{0};
	
	rb.root->debug_add_left(-4, true);
	rb.root->left->debug_add_left(-5, false);
	rb.root->left->left->debug_add_left(-10, true);
	rb.root->left->debug_add_right(-3, false);
	rb.root->debug_add_right(5, false);
	
	cout << "test: " << name << '\n';
	print(&rb);
	cout << "current node: -3\n";
	rb.root->left->right->walk_down_step(-3); //target doesn't matter for in-between case
	
	cout << "result: \n";
	print(&rb);
	
	return;
}

void test_34() {
		/*
	walk_down case leaf #7:
	0		-> -3
	|*-5    -> |*-5 
	 |-10   ->  |-10
	 |-3    ->  |-4 
	  |*-4  -> |5
	|5      ->  |*0 	<---------------(5)
	*/
	char name[] = "walk_down at leaf: 2-node (5) is right child (of black node) with 3-node sibling: -> scooch right";
	
	RBTree<int> rb{0};
	
	rb.root->debug_add_left(-5, true);
	rb.root->left->debug_add_left(-10, false);
	rb.root->left->debug_add_right(-3, false);
	rb.root->left->right->debug_add_left(-4, true);
	rb.root->debug_add_right(5, false);
	
	cout << "test: " << name << '\n';
	print(&rb);
	cout << "current node: 5\n";
	rb.root->right->walk_down_step(5); //target doesn't matter for in-between case
	rb.update_root();
	
	cout << "result: \n";
	print(&rb);
	
	return;
}


int main() {
	
	test_01();
	test_02();
	test_03();
	test_03();
	test_04();
	test_05();
	test_06();
	test_07();
	test_08();
	test_09();
	test_10();
	test_11();
	
	test_12();
	test_13();
	test_14();
	test_15();
	test_16();
	test_17();
	
	test_18();
	test_19();


	test_20();
	
	test_21();
	test_22();
	test_23();
	test_24();
	
	test_25();
	test_26();
	
	test_27();
	test_28();
	test_29();
	test_30();
	test_31();
	test_32();
	test_33();
	test_34();
	
	return 0;
}

