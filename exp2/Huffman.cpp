#include <cstdio>
#include <cstring>
#include <cctype>

#include "../BinTree.h"
#include "../BinNode.h"

#include "../List.h"
#include "../Bitmap.h"
#include "../HashTable.h"  

struct HuffChar {
    char ch;
    int weight;
    HuffChar(char c='^', int w=0): ch(c), weight(w) {}
    bool operator<(const HuffChar& hc) const { return weight > hc.weight; }
    bool operator==(const HuffChar& hc) const { return weight == hc.weight; }
};


#define HuffTree BinTree<HuffChar>

typedef List<HuffTree*> HuffForest;
typedef Bitmap HuffCode;
typedef HashTable<char, char*> HuffTable;

#define N_CHAR  (0x80 - 0x20)

int* statistics(char* sample_text_file) {
    int* freq = new int[N_CHAR];
    memset(freq, 0, sizeof(int) * N_CHAR);

    FILE* fp = fopen(sample_text_file, "r");
    if (!fp) {
        printf("Cannot open file: %s\n", sample_text_file);
        return freq;
    }

    for (char ch; fscanf(fp, "%c", &ch) > 0; )
        if (ch >= 0x20) freq[(unsigned char)ch - 0x20]++;

    fclose(fp);
    return freq;
}


HuffForest* initForest(int* freq) {
    HuffForest* forest = new HuffForest;
    for (int i = 0; i < N_CHAR; i++) {
        HuffTree* T = new HuffTree;
        T->insertAsRoot(HuffChar((char)(0x20 + i), freq[i]));
        forest->insertAsLast(T);
    }
    return forest;
}


HuffTree* minHChar(HuffForest* forest) {
    auto p = forest->first();
    auto minNode = p;
    int minWeight = p->data->root()->data.weight;

    while (forest->valid(p = p->succ)) {
        int w = p->data->root()->data.weight;
        if (w < minWeight) {
            minWeight = w;
            minNode = p;
        }
    }
    return forest->remove(minNode);
}

HuffTree* generateTree(HuffForest* forest) {
    while (forest->size() > 1) {            
        HuffTree* T1 = minHChar(forest);
        HuffTree* T2 = minHChar(forest);

        HuffTree* S = new HuffTree;
        S->insertAsRoot(
            HuffChar('^', T1->root()->data.weight + T2->root()->data.weight)
        );
        S->attachAsLC(S->root(), T1);
        S->attachAsRC(S->root(), T2);

        forest->insertAsLast(S);
    }
    return forest->first()->data;
}


static void generateCT(Bitmap* code, int length,
                       HuffTable* table, BinNodePosi(HuffChar) v)
{
    if (IsLeaf(*v)) {
        table->put(v->data.ch, code->bits2string(length));
        return;
    }

    if (HasLChild(*v)) {
        code->clear(length);
        generateCT(code, length+1, table, v->lc);
    }

    if (HasRChild(*v)) {
        code->set(length);
        generateCT(code, length+1, table, v->rc);
    }
}


HuffTable* generateTable(HuffTree* tree) {
    HuffTable* table = new HuffTable;
    Bitmap* code = new Bitmap;

    generateCT(code, 0, table, tree->root());

    delete code;
    return table;
}


int encode(HuffTable* table, Bitmap* codeString, char* s) {
    int n = 0;

    for (size_t i = 0, m = strlen(s); i < m; i++) {
        char ch = s[i];
        char** pCode = table->get(ch);

        if (!pCode && isalpha((unsigned char)ch)) {
            char ch2 = islower((unsigned char)ch) ? toupper((unsigned char)ch) : tolower((unsigned char)ch);
            pCode = table->get(ch2);
        }

        if (!pCode) pCode = table->get(' ');  
        if (pCode) {
            printf("%s", *pCode);

            size_t L = strlen(*pCode);
            for (size_t j = 0; j < L; j++) {
                if ((*pCode)[j] == '1') codeString->set(n++);
                else codeString->clear(n++);
            }
        }
    }
    printf("\n");

    return n;
}


void decode(HuffTree* tree, Bitmap* code, int n) {
    BinNodePosi(HuffChar) x = tree->root();

    for (int i = 0; i < n; i++) {
        x = code->test(i) ? x->rc : x->lc;

        if (IsLeaf(*x)) {
            printf("%c", x->data.ch);
            x = tree->root();
        }
    }
    printf("\n");
}


int main(int argc, char* argv[]) {

    if (argc < 3) {
        printf("Usage: huffman textFile word1 [word2 ...]\n");
        return 0;
    }

    int* freq = statistics(argv[1]);

    HuffForest* forest = initForest(freq);
    delete[] freq;

    HuffTree* tree = generateTree(forest);
    delete forest;

    HuffTable* table = generateTable(tree);

    for (int i = 2; i < argc; i++) {
        Bitmap* codeString = new Bitmap;

        int n = encode(table, codeString, argv[i]);
        decode(tree, codeString, n);

        delete codeString;
    }

    delete table;
    delete tree;

    return 0;
}
