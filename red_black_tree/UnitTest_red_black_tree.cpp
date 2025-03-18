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
using Node = RBTree<T>;

using std::cout;

void test_01() {
	int test_value = 1;
	RBTree<int> rb{test_value};
	Node<int> *root = rb.find(test_value);
	
	bool OK = true;
	OK &= root != nullptr;
	OK &= root->value == test_value;
	
	cout << "test: find root\n";	
	cout << (OK ? "OK" : "ERROR") << "\n";	
	cout << "\n";
	
	return;
}


void test_02() {
	int test_value = 1;
	RBTree<int> rb{test_value};
	rb.add(-1);
		
	bool OK = true;
	OK &= rb.find(1) != nullptr;	
	OK &= rb.find(-1) != nullptr;
	
	cout << "test: add and find 2 elements\n";
	cout << (OK ? "OK" : "ERROR") << "\n";	
	cout << "\n";
	
	return;
}


void test_03() {
	int test_value = 0;
	RBTree<int> rb{test_value};
	rb.add(-1);
	rb.add(1);
		
	bool OK = true;
	OK &= rb.find(0) != nullptr;	
	OK &= rb.find(-1) != nullptr;
	OK &= rb.find(1) != nullptr;	
	
	int exp[] = {-1,0,1};
	int out[ array_count(exp) ];
	
	rb.inorder_to_buf(out);
	for (int i = 0 ; i < array_count(out) ; ++i) {
		OK &= out[i] == exp[i];
	}	
	
	cout << "test: add 3 elements and output the values in-order\n";
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
	
	rb.inorder_to_buf(out);
	
	int rb_count = rb.count();
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
	
	rb.inorder_to_buf(out);
	
	cout << "test: add 6 elements unsorted, and output the values in-order\n";
	cout << "result  : " << array_to_string<int>(out, array_count(out)) << "\n";
	cout << "expected: " << array_to_string<int>(exp, array_count(exp)) << "\n";	
	cout << "\n";
}

void test_06(){
	RBTree<int> rb{0};	
	
	rb.add(-1);
	rb.add(-2);
	rb.add(-3);	
	rb.add(-4);
	rb.add(-5);
	
	
	bool OK = true;
	
	int out[6] = {};
	rb.inorder_to_buf(out);
	
	int shortest_path_length = get_shortest_path_length(&rb);
	int longest_path_length = get_longest_path_length(&rb);
	
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

	RBTree<int> *temp = &rb;
	temp->is_red = false;
	temp = rb.find(-1);
	
	bool OK = true;
	
	int out[6] = {};
	rb.inorder_to_buf(out);
	
	int shortest_path_length = get_shortest_path_length(&rb);
	int longest_path_length = get_longest_path_length(&rb);
	
	cout << "test: shortest and longest path\n";
	cout << array_to_string(out,array_count(out)) << "\n";
	cout << "shortest: " << shortest_path_length << "\n";
	cout << "longest: " << longest_path_length << "\n";
	cout << "\n";
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
	
	
	return 0;
}