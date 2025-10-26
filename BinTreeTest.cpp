#include <iostream>
#include <cstdlib>
#include <ctime>
#include "BinTree.h"

using namespace std;

// 打印函数
template <typename T>
void printElement(T& e) {
    cout << e << " ";
}

// 测试函数
void testConstructors() {
    cout << "=== 测试构造函数 ===" << endl;
    
    // 默认构造函数
    BinTree<int> tree;
    cout << "默认构造函数: size = " << tree.size() << ", empty = " << (tree.empty() ? "true" : "false") << endl;
    
    // 插入根节点
    tree.insertAsRoot(10);
    cout << "插入根节点后: size = " << tree.size() << ", empty = " << (tree.empty() ? "true" : "false") << endl;
    
    cout << endl;
}

void testInsertFunctions() {
    cout << "=== 测试插入操作 ===" << endl;
    
    BinTree<int> tree;
    tree.insertAsRoot(10);
    cout << "创建根节点值为10的树" << endl;
    
    // 插入左子节点
    BinNodePosi(int) root = tree.root();
    tree.insertAsLC(root, 5);
    cout << "插入左子节点5" << endl;
    
    // 插入右子节点
    tree.insertAsRC(root, 15);
    cout << "插入右子节点15" << endl;
    
    // 在左子树上再插入节点
    BinNodePosi(int) lc = root->lc;
    tree.insertAsLC(lc, 3);
    tree.insertAsRC(lc, 7);
    cout << "在节点5下插入左子节点3和右子节点7" << endl;
    
    // 在右子树上再插入节点
    BinNodePosi(int) rc = root->rc;
    tree.insertAsLC(rc, 12);
    tree.insertAsRC(rc, 18);
    cout << "在节点15下插入左子节点12和右子节点18" << endl;
    
    cout << "当前树的大小: " << tree.size() << endl << endl;
}

void testTraversals() {
    cout << "=== 测试遍历操作 ===" << endl;
    
    BinTree<int> tree;
    tree.insertAsRoot(10);
    
    BinNodePosi(int) root = tree.root();
    tree.insertAsLC(root, 5);
    tree.insertAsRC(root, 15);
    
    BinNodePosi(int) lc = root->lc;
    tree.insertAsLC(lc, 3);
    tree.insertAsRC(lc, 7);
    
    BinNodePosi(int) rc = root->rc;
    tree.insertAsLC(rc, 12);
    tree.insertAsRC(rc, 18);
    
    cout << "层序遍历: ";
    tree.travLevel(printElement<int>);
    cout << endl;
    
    cout << "先序遍历: ";
    tree.travPre(printElement<int>);
    cout << endl;
    
    cout << "中序遍历: ";
    tree.travIn(printElement<int>);
    cout << endl;
    
    cout << "后序遍历: ";
    tree.travPost(printElement<int>);
    cout << endl << endl;
}

void testAttachAndSecede() {
    cout << "=== 测试连接和分离操作 ===" << endl;
    
    // 创建主树
    BinTree<int> tree;
    tree.insertAsRoot(10);
    BinNodePosi(int) root = tree.root();
    tree.insertAsLC(root, 5);
    tree.insertAsRC(root, 15);
    
    cout << "主树创建完成，大小: " << tree.size() << endl;
    
    // 创建子树
    BinTree<int>* subTree = new BinTree<int>;
    subTree->insertAsRoot(20);
    BinNodePosi(int) subRoot = subTree->root();
    subTree->insertAsLC(subRoot, 18);
    subTree->insertAsRC(subRoot, 25);
    
    cout << "子树创建完成，大小: " << subTree->size() << endl;
    
    // 将子树连接到主树的左子节点上
    tree.attachAsLC(root->lc, subTree);
    cout << "将子树连接到主树的节点5上" << endl;
    cout << "连接后主树大小: " << tree.size() << endl;
    
    // 从主树中分离一个子树
    BinTree<int>* secededTree = tree.secede(root->rc);
    cout << "从主树分离右子树" << endl;
    cout << "分离后主树大小: " << tree.size() << endl;
    cout << "分离出来的子树大小: " << secededTree->size() << endl;
    
    delete secededTree;
    
    cout << endl;
}

void testRemove() {
    cout << "=== 测试删除操作 ===" << endl;
    
    BinTree<int> tree;
    tree.insertAsRoot(10);
    BinNodePosi(int) root = tree.root();
    tree.insertAsLC(root, 5);
    tree.insertAsRC(root, 15);
    
    BinNodePosi(int) lc = root->lc;
    tree.insertAsLC(lc, 3);
    tree.insertAsRC(lc, 7);
    
    BinNodePosi(int) rc = root->rc;
    tree.insertAsLC(rc, 12);
    tree.insertAsRC(rc, 18);
    
    cout << "初始树的大小: " << tree.size() << endl;
    cout << "中序遍历: ";
    tree.travIn(printElement<int>);
    cout << endl;
    
    // 删除节点5及其子树
    int removed = tree.remove(root->lc);
    cout << "删除节点5及其子树，共删除" << removed << "个节点" << endl;
    cout << "删除后树的大小: " << tree.size() << endl;
    cout << "删除后中序遍历: ";
    tree.travIn(printElement<int>);
    cout << endl << endl;
}

int main() {
    cout << "BinTree类功能测试程序" << endl << endl;
    
    testConstructors();
    testInsertFunctions();
    testTraversals();
    testAttachAndSecede();
    testRemove();
    
    cout << "所有测试完成!" << endl;
    return 0;
}