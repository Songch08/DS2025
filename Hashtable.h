/**********************************************************************
 * HashTable (Dictionary 实现之一)
 * 邓俊辉《数据结构（C++版）》源码
 **********************************************************************/

#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "Entry.h"
#include "Bitmap.h"
#include "Vector.h"

template <typename K, typename V>
class HashTable {
protected:
    int M;               // hash table size
    int N;               // number of actual records
    Entry<K, V>** ht;    // table
    Bitmap* lazyRemoval; // lazy deletion mask

protected:
    int probe4Hit(const K& k) const {
        int r = hashCode(k) % M;
        while (lazyRemoval->test(r) || (ht[r] && (k != ht[r]->key)))
            r = (r + 1) % M;
        return r;
    }

    int probe4Free(const K& k) const {
        int r = hashCode(k) % M;
        while (ht[r] && !(k == ht[r]->key))
            r = (r + 1) % M;
        return r;
    }

    void rehash() {
        int oldM = M;
        Entry<K, V>** oldHt = ht;
        Bitmap* oldRemoval = lazyRemoval;

        M <<= 1;
        ht = new Entry<K, V>*[M];
        memset(ht, 0, sizeof(Entry<K, V>*) * M);
        lazyRemoval = new Bitmap(M);

        N = 0;
        for (int i = 0; i < oldM; i++) {
            if (oldHt[i] && !oldRemoval->test(i)) {
                put(oldHt[i]->key, oldHt[i]->value);
            }
        }

        for (int i = 0; i < oldM; i++)
            if (oldHt[i]) delete oldHt[i];

        delete[] oldHt;
        delete oldRemoval;
    }

public:
    HashTable(int c = 5) {
        M = c;
        N = 0;
        ht = new Entry<K, V>*[M];
        memset(ht, 0, sizeof(Entry<K, V>*) * M);
        lazyRemoval = new Bitmap(M);
    }

    ~HashTable() {
        for (int i = 0; i < M; i++)
            if (ht[i]) delete ht[i];
        delete[] ht;
        delete lazyRemoval;
    }

    int size() const { return N; }

    bool put(K key, V value) {
        if (ht[probe4Hit(key)])
            return false;

        int r = probe4Free(key);
        ht[r] = new Entry<K, V>(key, value);
        N++;

        while (N * 2 > M)
            rehash();

        return true;
    }

    V* get(K key) {
        int r = probe4Hit(key);
        if (!ht[r]) return nullptr;
        return &(ht[r]->value);
    }

    bool remove(K key) {
        int r = probe4Hit(key);
        if (!ht[r]) return false;

        delete ht[r];
        ht[r] = nullptr;
        lazyRemoval->set(r);
        N--;
        return true;
    }
};

#endif // HASHTABLE_H
