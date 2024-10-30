#include "heap.h"

#include <iostream>
#include <random>
#include <concepts>

using std::cout;

using MyNode = Node<int,float>;
using MyHeap = Heap<int,float>;

void generate_ints(int *dest, int count, int min = 0x8000'0000, int max = 0x7fff'ffff) {
    std::mt19937 gen{};
    std::uniform_int_distribution<> dist{min, max};
    int *cur = dest;
    for (int *cur = dest, *limit = dest + count; cur != limit; ++cur) {
        *cur = dist(gen);
    }
    return;
}

void generate_floats(float *dest, int count, float min = 0x1.p-149f, float max = 0x1.fffffep+127f) {
    std::mt19937 gen{};
    std::uniform_real_distribution<> dist{min, max};
    float *cur = dest;
    for (float *cur = dest, *limit = dest + count; cur != limit; ++cur) {
        *cur = dist(gen);
    }
    return;
}

void generate_random_nodes(MyNode *dest, int *keys, float *vals, int count) {
    generate_ints(keys, count, 0, 100);
    generate_floats(vals, count, -1000.f, 1000.f);
    int *cur_key = keys;
    float *cur_val = vals;
    MyNode *cur_nod = dest;
    for ( int i = 0 ; i < count ; ++i ) {
        new(cur_nod++) MyNode{ *cur_key++, *cur_val++ };
    }
    return;
}


int main() {

    int count = 0b1000000'0000000000'0000000000;
    int *keys = new int[count];
    float *vals = new float[count];
    MyNode *nodes= new MyNode[count];
    generate_random_nodes(nodes, keys, vals, count);

//    {
//        MyNode *node = nodes;
//        cout << *node;
//        for (int i = 1 ; i < count ; ++i ){
//            cout << ", " << *++node;
//        }
//        cout << "\n";
//    }

    MyHeap heap;

    for (int i = 0 ; i < count ; ++i) {
        heap.add(nodes[i]);
    }

//    cout << heap << "\n";

    bool is_heap = heap._check();
    cout << "check: " << (is_heap?"OK":"ERROR") << "\n";

    {
        bool OK = true;
        MyNode top = heap.pop();
        MyNode last = top;
//        cout << top.key;
        for (int i = 1 ; i < count ; ++i) {
            top = heap.pop();
//            cout << (top.key==last.key?", ":"\n") << top.key;
            OK &= !MyHeap::_has_prio_over(&top, &last);
            if (!OK) break;
            last = top;
        }
        cout << "\n\n" << "all popped in order? " << (OK?"OK":"ERROR") << "\n";
        delete nodes;
    }

    return 0;
}
