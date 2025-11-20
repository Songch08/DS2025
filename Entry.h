#ifndef ENTRY_H
#define ENTRY_H

template <typename K, typename V>
struct Entry {
    K key;
    V value;
    Entry(K k = K(), V v = V()) : key(k), value(v) {}
};

#endif
