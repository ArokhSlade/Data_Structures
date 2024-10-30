#ifndef HEAP_H
#define HEAP_H

#include <vector>
#include <iostream>
#include <cmath> //pow

#include "utils.h"

using std::vector;
using std::ostream;

template <typename Key, typename Value>
struct Node{
    Key key;
    Value value;

    bool operator<(Node& other) {
        return key < other.key;
    }

    bool operator>(Node& other) {
        return key > other.key;
    }
};

template<typename K, typename V>
ostream& operator<<(ostream& os, Node<K,V>& node) {
    os << "(" << node.key << ", " << node.value << ")";
    return os;
}

template <typename Key, typename Value>
struct Heap{
    using T_Node = Node<Key,Value>;

    T_Node *add(const T_Node& node);
    T_Node pop();

    vector<T_Node> _data;

    bool is_empty() {
        return _data.empty();
    }

    T_Node *_root() {
        return &_data.front();
    }


    size_t _index(T_Node *me);
    bool _is_root(T_Node *me);
    bool _is_last(T_Node *me);
    size_t _row_idx(T_Node *me);
    size_t _col_idx(T_Node *me);
    bool _is_right_sibling(T_Node *me);
    T_Node *_get_sibling(T_Node *me);
    T_Node *_get_parent(T_Node *me);
    T_Node *_swap(T_Node *first, T_Node *second);
    bool _has_children(T_Node *me);
    static bool _has_prio_over(T_Node *first, T_Node *second);
    T_Node *_get_left_child(T_Node *parent);
    T_Node *_get_right_child(T_Node *parent);
    void _raise(T_Node *node);
    void _sink(T_Node *node);
    bool _check();
    size_t _size() {
        return _data.size();
    }
    bool _is_empty() {
        return _data.empty();
    }

    Heap() : _data{}
    {}
};

template <typename K, typename V>
ostream& operator<<(ostream& os, Heap<K,V>& heap) {
    os << "{";
    if (!heap._is_empty()) {
        os << heap._data[0];
    }
    for (auto i = 1 ; i < heap._size() ; ++i) {
        os << ", " << heap._data[i];
    }

    os << " }";

    return os;
}

template <typename K, typename V>
size_t Heap<K,V>::_index(T_Node *me) {
    if (!me) return -1ull;
    size_t result = me - _root();
    return result;
}

template <typename K, typename V>
bool Heap<K,V>::_is_root(T_Node *me) {
    if (!me) return false;
    return me == _root();
}

template <typename K, typename V>
bool Heap<K,V>::_has_prio_over(T_Node *first, T_Node *second) {
    if (!first) return false;
    if (!second) return true;
    return *first < *second;
}

template <typename K, typename V>
bool Heap<K,V>::_check() {

    int half_size = _size()/2;

    bool result = true;
    for (int i = 0 ; i < half_size; ++i) {
        T_Node *parent = &_data[i];
        if (T_Node *left_child = _get_left_child(parent)) {
            result &= !_has_prio_over(left_child, parent);
            if (T_Node *right_child = _get_right_child(parent)) {
                result &= !_has_prio_over(right_child, parent);
            }
        }
    }
    return result;
};

template<typename K, typename V>
Node<K,V> *Heap<K,V>::_get_left_child(T_Node *parent) {
    if (is_empty() || !parent) return nullptr;

    T_Node *left = nullptr;
    auto parent_idx = parent - _root();
    auto left_idx = (parent_idx + 1) * 2 - 1;
    if (left_idx < _size()) {
        left = &_data[left_idx];
    }
    return left;
}


template<typename K, typename V>
Node<K,V> *Heap<K,V>::_get_right_child(T_Node *parent) {
    if (is_empty() || !parent) return nullptr;

    T_Node *right = nullptr;
    auto parent_idx = parent - _root();
    auto left_idx = (parent_idx + 1) * 2;
    if (left_idx < _size()) {
        right = &_data[left_idx];
    }
    return right;
}




template<typename K, typename V>
//heap.h|73|error: deduced class type 'T_Node' in function return type|
//||error: 'T_Node' does not name a type; did you mean 'Node'?||
//heap.h|16|note: 'template<class Key, class Value> using T_Node = Node<Key, Value>' declared here|
//T_Node& Heap<K,V>::add(const T_Node& new_node) {
Node<K,V> *Heap<K,V>::add(const T_Node& new_node) {
    _data.push_back(new_node);
    T_Node *added = &_data.back();
    _raise(added);
    return added;
}

template<typename K, typename V>
bool Heap<K,V>::_is_last(T_Node *me) {
    if (!me) return false;
    bool result = _index(me) == _size()-1;
    return result;
}

template<typename K, typename V>
size_t Heap<K,V>::_row_idx(T_Node *me) {
    auto my_idx = _index(me) + 1;
    int row_idx = static_cast<int>(log_2(my_idx));
    return row_idx;
}

template<typename K, typename V>
size_t Heap<K,V>::_col_idx(T_Node *me) {
    auto my_idx = _index(me);
    auto row_idx = _row_idx(me);
    auto col_idx = my_idx - pow_2(row_idx);
    return col_idx;
}

template<typename K, typename V>
bool Heap<K,V>::_is_right_sibling(T_Node *me) {
    if (!me || me == _root()) return false;
    bool result = is_even(_col_idx(me));
    return result;
}

template<typename K, typename V>
Node<K,V> *Heap<K,V>::_get_sibling(T_Node *me) {
    if (!me || me == _root()) return nullptr;
    T_Node *sibling = nullptr;
    auto my_idx = me - _root();
    if (_is_right_sibling()) {
        sibling = &_data[my_idx-1];
    } else if (!_is_last(me)) {
        sibling = &_data[my_idx+1];
    }
    return sibling;
}

template<typename K, typename V>
Node<K,V> *Heap<K,V>::_get_parent(T_Node *me) {
    if (!me || me == _root()) return nullptr;
    auto my_idx = _index(me) + 1;
    auto parent_idx = my_idx / 2 - 1;
    T_Node *parent = &_data[parent_idx];
    return parent;
}

template<typename K, typename V>
Node<K,V> *Heap<K,V>::_swap(T_Node *first, T_Node *second) {
    T_Node temp = *first;
    *first = *second;
    *second = temp;
    return second;
}

template<typename K, typename V>
void Heap<K,V>::_raise(T_Node *me) {
    if (_is_root(me)) return;

    T_Node *parent = _get_parent(me);

    if (_has_prio_over(me, parent)) {
        me = _swap(me, parent);
        _raise(me);
    }
    return;
}

template<typename K, typename V>
bool Heap<K,V>::_has_children(T_Node *me) {
    if (!me) return false;
    auto my_idx = _index(me);
    auto left_idx = (my_idx+1)*2-1;
    bool result = left_idx < _size();
    return result;
}

template<typename K, typename V>
void Heap<K,V>::_sink(T_Node *me) {
    if (!_has_children(me)) return;

    T_Node *left = _get_left_child(me);
    T_Node *right = _get_right_child(me);

    T_Node *swapper = nullptr;
    if (_has_prio_over(left, me)) {
        swapper = left;
    }
    if (_has_prio_over(right, me)) {
        if (!swapper) swapper = right;
        else swapper = _has_prio_over(left, right) ? left : right;
    }
    if (swapper) {
        me = _swap(me, swapper);
        _sink(me);
    }

    return;
}

template<typename K, typename V>
Node<K,V> Heap<K,V>::pop() {

    T_Node top{_data[0]};
    _data.front() = _data.back();
    _data.pop_back();
    _sink(&_data.front());
    return top;
}

#endif //HEAP_H
