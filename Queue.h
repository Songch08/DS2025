#ifndef QUEUE_H
#define QUEUE_H

#include "List.h"
struct Customer {
    int window;
    unsigned int time;
};

template <typename T>
class Queue {
private:
    List<T> _list;

public:
    typedef typename List<T>::Posi Posi;

    void enqueue(T const& e) { _list.insertAsLast(e); }
    T dequeue() { return _list.remove(_list.first()); }
    T& front() { return _list.first()->data; }

    bool empty() const { return _list.empty(); }
    int size() const { return _list.size(); }
};

#endif