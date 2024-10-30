#ifndef HEAP_H
#define HEAP_H

#include <vector>

template <typename Key, typename Value>
struct Node{
    Key key;
    Value value;

    bool operator<=(Node& other);
};

template <typename Key, typename Value>
struct Heap{
    using T_Node = Node<Key,Value>;

    T_Node& add(T_Node& node);
    T_Node& pop_min();

    vector<node> _data;

    void _raise(T_Node& node);
    void _sink(T_Node& node);
    bool _check();
    bool _size() {
        return _data.size();
    }
    Heap() : _data{}
    {}
};

bool Heap::_check() {

    int half_size = _size()/2;

    bool result = true;
    for (int i = 0 ; i < half_size; ++i) {
        T_Node& parent = _data[i];

        int i_child = 2*(i+1)-1;
        if (i_child < _size()) {
            T_Node& child = _data[i_child];
            result &= child<=parent;
            if (i_child+i < _size()) {
                T_Node& child = _data[i_child+1];
                result &= child<=parent;
            }
        }
    }
    return result;
}

T_Node& Heap::add(T_Node& new_node) {
    //TODO: impl
    return new_node;
}

void Heap::_raise(T_Node& start) {
    //TODO: impl
    return;
}

void Heap::_sink(T_Node& start) {
    //TODO: impl
    return;
}

T_Node& Heap::pop_min() {

    //TODO: impl
    T_Node& min_elm = _data[0];
    return min_elm;
}

#endif //HEAP_H
