#ifndef UTILS_H
#define UTILS_H

inline bool is_odd(int n) {
    return n&1;
}

inline bool is_even(int n) {
    return !n&1;
}

template <typename T_UInt>
inline int log_2(T_UInt n) {
    int index = (sizeof(T_UInt)*8-1);
    T_UInt mask = 1ull << index;
    while (!(mask&n)) {
        mask >>= 1;
        --index;
    }
    return index;
}

template <typename T_UInt>
inline int pow_2(int n) {
    if (n == 0) return 1;

    constexpr T_UInt invalid = ~0ull;
    if (n < 0) return invalid;
    if (n >= sizeof(T_UInt) * 8) return invalid;

    T_UInt result = 1ull;
    result <<= n;
    return result;
}

#endif //UTILS_H
