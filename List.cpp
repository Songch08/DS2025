#include "listNode.h"

template <typename T> class List { 

    private:
    int _size;ListNodePosi(T) _header;ListNodePosi(T) _tailer;

    protected:
    void init();
    int clear();
    void copyNodes(ListNodePosi(T),int);
    void merge(ListNodePosi(T)&,int,List<T>&,ListNodePosi(T),int);
    void mergeSort(ListNodePosi(T)&,int);
    void selectionSort(ListNodePosi(T),int);
    void insertionSort(ListNodePosi(T),int);
};