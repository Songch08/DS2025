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
template<typename T>
ListNodePosi(T) ListNode<T>::insertAsPred(T const& e){
    ListNodePosi(T) x = new ListNode(e,pred,this);
    pred->_succ = x;pred = x;
    return x;
}
template <typename T>
ListNodePosi(T) ListNode<T>::insertAsSucc(T const& e){
    ListNodePosi(T) x = new ListNode(e,this,succ);
    succ->_pred = x;succ = x;
    return x;
}
template <typename T>
void List<T>::copyNodes(ListNodePosi(T) p,int n){ 
    init();
    while(n--) {insertAsLast(p->data);p = p->_succ;}
}
template <typename T>
List<T>::List(ListNodePosi(T) p,int n){ copyNodes(p,n);}

template <typename T>
List<T>::List(List<T> const& L){ copyNodes(L.first(),L._size());}

template <typename T>
List<T>::List(List<T> const& L,int r,int n){ copyNodes(L[r],n);}
template <typename T> T List<T>::remove(ListNodePosi(T) p){ 
    T e = p->data;
    p->_pred->_succ = p->_succ;p->_succ->_pred = p->_pred;
    delete p;_size--;
    return e;
}
template <typename T> List<T>::~List()
{ clear();delete header;delete trailer;}
template <typename T> int List<T>::clear(){ 
    int oldSize = _size;
    while(0<_size) remove(header->_succ);
    return oldSize;
}
template <typename T> int List<T>::deduplicate(){
    if(_size<2) return 0;
    int oldSize = _size;
    ListNodePosi(T) p = header;Rank r = 0;
    while(trailer != (p = p->_succ)){
        ListNodePosi(T) q = find(p->data,r,p);
        q?remove(q):r++;
    }
    return oldSize-_size;
}
template <typename T> void List<T>::traverse (void(*visit)(T*))
{ for (ListNodePosi(T) p=header->_succ;p!=trailer;p=p->_succ) visit(&p->data);}

template <typename T> template <typename VST>
void List<T>::traverse(VST& visit)
{ for (ListNodePosi(T) p=header->_succ;p!=trailer;p=p->_succ) visit(p->data);}