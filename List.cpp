#include "listNode.h"

template <typename T> class List { 

private:
    int _size;ListNodePosi(T) header;ListNodePosi(T) trailer;

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
    ListNodePosi(T) first() const { return _header->_succ; }
    ListNodePosi(T) last() const { return _tailer->_pred; }
    bool vaild (ListNodePosi(T) p)
    {return p && (trailer != p)&& (header != p);}
    int disordered() const;
    ListNodePosi(T) find(T const& e) const
    {return find(e,_size,trailer);}
    ListNodePosi(T) find (T const& e,int n,ListNodePosi(T) p)const;
    ListNodePosi(T) search(T const& e) const
    {return search(e,_size,trailer);}
    ListNodePosi search(T const& e,int n,ListNodePosi(T) p)const;
    ListNodePosi(T) selectMax(ListNodePosi(T) p,int n);
    ListNodePosi(T) selectMax() { return selectMax(header->succ,_size); }
    


};