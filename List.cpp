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

public:
    List() { init(); }
    List(List<T> const& L);
    List(List<T> const& L,Rank r,int n);
    List(ListNodePosi(T) p,int n);
    
    ~List();

    Rank size() const { return _size; }
    bool empty() const { return _size<=0; }
    T& operator[](Rank r) const;

};