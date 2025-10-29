#include "BinTree.h"
typedef BinTree<char> PFCTree;

#include "Vector.h"
typedef Vector<PFCTree*> PFCForest;

#include "Bitmap.h"
#include "Skiplist.h"
typedef Skiplist<char,char*> PFCTable;

#define N_CHAR  (0x80-0x20)

PFCForest* initForest(){ 
    PFCForest* forest = new PFCForest;
    for (int i=0;i<N_CHAR;i++){
        forest->insert(1, new PFCTree());
        (*forest)[i]->insertAsRoot(0x20+i);
    }
    return forest;
}

PFCTree* generateTree(PFCForest* forest){ 
    srand((unsigned int)time(NULL));
    while (1<forest->size()){
        PFCTree* s=new PFCTree;s->insertAsRoot('^');
        Rank r1 =rand()%forest->size();
        s->insertAsLC(s->root(),(*forest)[r1]);
        forest->remove(r1);
        Rank r2 =rand()%forest->size();
        s->insertAsRC(s->root(),(*forest)[r2]);
        forest->remove(r2);
        forest->insert(forest->size(),s);
    }
    return (*forest)[0];
}

void generateCT
(Bitmap* code,int length,PFCTable* table,BinNodePosi(char) v ){
    if (IsLeaf(*v))
       {table->put(v->data,code->bits2string(length));return;}
    if (HasLChild(*v))
       {code->clear(length);generateCT(code,length+1,table,v->lc);}
    if (HasRChild(*v))
       {code->set(length);generateCT(code,length+1,table,v->rc);}
}
PFCTable* generateTable (PFCTree* tree){
    PFCTable* table = new PFCTable;
    Bitmap* code = new Bitmap;
    generateCT(code,0,table,tree->root());
    release(code);return table;
}

int encode(PFCTable* table,Bitmap& codeString,char* str){ 
    int n = 0;
    for(size_t m=strlen(s),i=0;i<m;i++){
        char** pCharCode =table->get(s[i]);
        if(!pCharCode) pCharCode = table->get(s[i]+'A'-'a');
        if(!pCharCode) pCharCode = table->get(' ');
        printf("%s",*pCharCode);
        for(size_t m=strlen(*pCharCode),j=0;j<m;j++)
           '1'==*(*pCharCode+j)?codeString.set(n++):codeString.clear(n++);
    }
    return n;
}

void decode(PFCTree* tree,Bitmap& codeString,int n){ 
    BinNodePosi(char) x = tree->root();
    for(int i=0;i<n;i++){
        x=code.test(i)?x->rc:x->lc;
        if(IsLeaf(*x)){printf("%c",x->data);x=tree->root();}
    }
}

int main(int argc, char* argv[]){
    PFCForest* forest =initForest();
    PFCTree* tree = generateTree(forest);release(forest);
    PFCTable* table = generateTable(tree);
    for (int i=1;i<argc;i++){
        Bitmap codeString;
        int n = encode(table,codeString,argv[i]);
        decode(tree,codeString,n);
    }
    release(table);release(tree);return 0;
}