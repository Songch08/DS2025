#define BinNodePosi(T) BinNode<T>*
#define stature(p) ((p)?(p)->height: -1)
typedef enum {RB_RED, RB_BLACK} RBColor;

template <typename T> struct BinNode {
    T data;
    BinNodePosi(T) parent; BinNodePosi(T) lc;BinNodePosi(T) rc;
    int height;
    int npl;
    RBColor color;

    BinNode() : 
       parent(NULL), lc(NULL), rc(NULL), height(0), npl(0), color(RB_RED) {}
    BinNode(T e,BinNodePosi(T) p=NULL,BinNodePosi(T) lc=NULL,BinNodePosi(T) rc=NULL,
            int h=0,int n=0,RBColor c=RB_RED):
        data(e), parent(p), lc(lc), rc(rc), height(h), npl(n), color(c) {}
};