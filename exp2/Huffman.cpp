#include <cstdio>
#include <cstring>
#include <cctype>

#include "../BinTree.h"
#include "../BinNode.h"

#include "../List.h"
#include "../Bitmap.h"
#include "../HashTable.h"   // 注意名称是 HashTable

// --------------------------------------------------------
// Huffman 节点数据（只定义一次）
// --------------------------------------------------------
struct HuffChar {
    char ch;
    int weight;
    HuffChar(char c='^', int w=0): ch(c), weight(w) {}
    bool operator<(const HuffChar& hc) const { return weight > hc.weight; }
    bool operator==(const HuffChar& hc) const { return weight == hc.weight; }
};

// --------------------------------------------------------
// HuffTree 是以 HuffChar 为节点数据的 BinTree
// --------------------------------------------------------
#define HuffTree BinTree<HuffChar>

// --------------------------------------------------------
// 依赖 HuffTree，因此必须写在 HuffTree 之后
// --------------------------------------------------------
typedef List<HuffTree*> HuffForest;
typedef Bitmap HuffCode;
typedef HashTable<char, char*> HuffTable;

#define N_CHAR  (0x80 - 0x20)

//-------------------------------------------------------------
// 统计字符频率
//-------------------------------------------------------------
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


//-------------------------------------------------------------
// 初始化森林：每个字符一棵树
//-------------------------------------------------------------
HuffForest* initForest(int* freq) {
    HuffForest* forest = new HuffForest;
    for (int i = 0; i < N_CHAR; i++) {
        HuffTree* T = new HuffTree;
        T->insertAsRoot(HuffChar((char)(0x20 + i), freq[i]));
        forest->insertAsLast(T);
    }
    return forest;
}


//-------------------------------------------------------------
// 找到权值最小的一棵树并从森林中移除
//-------------------------------------------------------------
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


//-------------------------------------------------------------
// 构建 Huffman 树
//-------------------------------------------------------------
HuffTree* generateTree(HuffForest* forest) {
    while (forest->size() > 1) {               // <- 注意 size()
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


//-------------------------------------------------------------
// 递归生成编码表
//-------------------------------------------------------------
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


//-------------------------------------------------------------
// 生成 Huffman 编码表
//-------------------------------------------------------------
HuffTable* generateTable(HuffTree* tree) {
    HuffTable* table = new HuffTable;
    Bitmap* code = new Bitmap;

    generateCT(code, 0, table, tree->root());

    delete code;
    return table;
}


//-------------------------------------------------------------
// 编码字符串 s
//-------------------------------------------------------------
int encode(HuffTable* table, Bitmap* codeString, char* s) {
    int n = 0;

    for (size_t i = 0, m = strlen(s); i < m; i++) {
        char ch = s[i];
        char** pCode = table->get(ch);

        if (!pCode && isalpha((unsigned char)ch)) {
            // 大小写转换
            char ch2 = islower((unsigned char)ch) ? toupper((unsigned char)ch) : tolower((unsigned char)ch);
            pCode = table->get(ch2);
        }

        if (!pCode) pCode = table->get(' ');   // fallback

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


//-------------------------------------------------------------
// 解码
//-------------------------------------------------------------
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


//-------------------------------------------------------------
// main
//-------------------------------------------------------------
int main(int argc, char* argv[]) {

    if (argc < 3) {
        printf("Usage: huffman textFile word1 [word2 ...]\n");
        return 0;
    }

    // 1. 统计频率
    int* freq = statistics(argv[1]);

    // 2. 构造森林
    HuffForest* forest = initForest(freq);
    delete[] freq;

    // 3. 构建 Huffman 树
    HuffTree* tree = generateTree(forest);
    delete forest;

    // 4. 编码表
    HuffTable* table = generateTable(tree);

    // 5. 编码+解码每个输入单词
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
