#include "heap.h"
#include <iostream>

using std::cout;

int main() {

    using MyNode = Node<int,float>;
    using MyHeap = Heap<int,float>;
    MyHeap heap;
    heap.add(MyNode{1,2.f});
    heap.add(MyNode{2,4.f});
    heap.add(MyNode{3,4.f});

    cout << "check: " << std::boolalpha << heap._check() << "\n";

    return 0;
}
