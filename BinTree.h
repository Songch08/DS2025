#include "BinNode.h"
template <typename T> class BinTree { 
protected:
   int _size;BinNodePosi(T) _root;
   virtual int updateHeight(BinNodePosi(T) x);
   void updateHeightAbove(BinNodePosi(T) x);
public:
   BinTree() : _size(0), _root(NULL) {}
   ~BinTree(){if(0<_size) remove(_root);}
   int size() const { return _size; }
   bool empty() const { return !_size; }
   BinNodePosi(T) root() const { return _root; }
   BinNodePosi(T) insertAsRoot(T const& e);
   BinNodePosi(T) insertAsLC(BinNodePosi(T) x, T const& e);
   BinNodePosi(T) insertAsRC(BinNodePosi(T) x, T const& e);
   BinNodePosi(T) attachAsLC(BinNodePosi(T) x, BinTree<T>* &T);
   BinNodePosi(T) attachAsRC(BinNodePosi(T) x, BinTree<T>* &T);
   int remove(BinNodePosi(T) x);
   BinTree<T>* secede(BinNodePosi(T) x);
   template <typename VST>
   void travLevel(VST& visit){if (_root) _root->travLevel(visit);}
   template <typename VST>
   void travPre(VST& visit){if (_root) _root->travPre(visit);}
   template <typename VST>
   void travIn(VST& visit){if (_root) _root->travIn(visit);}
   template <typename VST>
   void travPost(VST& visit){if (_root) _root->travPost(visit);}
   bool operator< (BinTree<T> const& t)
   {return _root && t._root && lt (_root, t._root);}
   bool operator== (BinTree<T> const& t)
   {return _root && t._root && (_root == t._root);}
};
template <typename T> int BinTree<T>::updateHeight(BinNodePosi(T) x)
{ return x->height = 1 + max(stature(x->lc), stature(x->rc));}

template <typename T> void BinTree<T>::updateHeightAbove(BinNodePosi(T) x)
{ while (x) {updateHeight(x); x = x->parent; } }