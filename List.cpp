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
    
    ListNodePosi(T) insertAsFirst(T const& e);
    ListNodePosi(T) insertAsLast(T const& e);
    ListNodePosi(T) insertA(ListNodePosi(T) p,T const& e);
    ListNodePosi(T) insertB(ListNodePosi(T) p,T const& e);
    T remove(ListNodePosi(T) p);
    void merge(List<T>& L){ merge(first(), size,L,L.first(),L._size); }
    void sort(ListNodePosi(T) p,int n);
    void sort() { sort(first(),_size); }
    int deduplicate()
    int uniquify();
    void reverse();

    void traverse(void (*)(T&));
    template <typename VST> 
    void traverse(VST&);
};
template <typename T> void List<T>::init(){ 
    header = new ListNode<T>;
    trailer = new ListNode<T>;
    header->_succ = trailer;header->_pred = NULL;
    trailer->_pred = header;trailer->_succ = NULL;
    _size = 0;
}
template <typename T>
T& List<T>::operator[](Rank r) const{ 
    ListNodePosi(T) p = first();
    while(0<r--) p = p->_succ;
    return p->data;
}
template <typename T>
ListNodePosi(T) List<T>::find(T const& e,int n,ListNodePosi(T) p)const{ 
    while (0<n--)
      if(e==(p=p->pred)->data) return p;
    return NULL;
}
template <typename T> ListNodePosi(T) List<T>::insertAsFirst(T const& e)
{ _size++; return header->insertAsSucc(e);}

template <typename T> ListNodePosi(T) List<T>::insertAsLast(T const& e)
{ _size++; return trailer->insertAsPred(e);}

template <typename T> ListNodePosi(T) List<T>::insertA(ListNodePosi(T) p,T const& e)
{ _size++; return p->insertAsPred(e);}

template <typename T> ListNodePosi(T) List<T>::insertB(ListNodePosi(T) p,T const& e)
{ _size++; return p->insertAsSucc(e);}