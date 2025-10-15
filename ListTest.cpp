#include <iostream>
#include <cstdlib>
#include <ctime>
#include "List.h"

using namespace std;

// 打印函数
void printElement(int& e) {
    cout << e << " ";
}

// 测试函数
void testConstructors() {
    cout << "=== 测试构造函数 ===" << endl;
    
    // 默认构造函数
    List<int> l1;
    cout << "默认构造函数: size = " << l1.size() << ", empty = " << (l1.empty() ? "true" : "false") << endl;
    
    // 插入一些元素再测试拷贝构造
    l1.insertAsLast(1);
    l1.insertAsLast(2);
    l1.insertAsLast(3);
    
    cout << endl;
}

void testInsertFunctions() {
    cout << "=== 测试插入操作 ===" << endl;
    
    List<int> l;
    
    // 测试在头部插入
    l.insertAsFirst(10);
    cout << "在头部插入10后: ";
    l.traverse(printElement);
    cout << ", size = " << l.size() << endl;
    
    // 测试在尾部插入
    l.insertAsLast(30);
    cout << "在尾部插入30后: ";
    l.traverse(printElement);
    cout << ", size = " << l.size() << endl;
    
    // 测试在节点之后插入
    ListNodePosi(int) p = l.first();
    l.insertB(p, 20);
    cout << "在第一个节点之后插入20后: ";
    l.traverse(printElement);
    cout << ", size = " << l.size() << endl;
    
    // 测试在节点之前插入
    p = l.last();
    l.insertA(p, 25);
    cout << "在最后一个节点之前插入25后: ";
    l.traverse(printElement);
    cout << ", size = " << l.size() << endl << endl;
}

void testRemoveFunction() {
    cout << "=== 测试删除操作 ===" << endl;
    
    List<int> l;
    l.insertAsLast(1);
    l.insertAsLast(2);
    l.insertAsLast(3);
    l.insertAsLast(4);
    l.insertAsLast(5);
    
    cout << "初始链表: ";
    l.traverse(printElement);
    cout << ", size = " << l.size() << endl;
    
    // 删除中间节点
    ListNodePosi(int) p = l.first()->succ; // 第二个节点
    int removedValue = l.remove(p);
    cout << "删除值为 " << removedValue << " 的节点后: ";
    l.traverse(printElement);
    cout << ", size = " << l.size() << endl << endl;
}

void testFindFunction() {
    cout << "=== 测试查找操作 ===" << endl;
    
    List<int> l;
    l.insertAsLast(10);
    l.insertAsLast(20);
    l.insertAsLast(30);
    l.insertAsLast(40);
    l.insertAsLast(50);
    
    cout << "链表内容: ";
    l.traverse(printElement);
    cout << endl;
    
    // 查找存在的元素
    ListNodePosi(int) p = l.find(30);
    if (p) {
        cout << "找到了元素30" << endl;
    } else {
        cout << "未找到元素30" << endl;
    }
    
    // 查找不存在的元素
    p = l.find(35);
    if (p) {
        cout << "找到了元素35" << endl;
    } else {
        cout << "未找到元素35" << endl;
    }
    
    cout << endl;
}

int main() {
    cout << "List类功能测试程序" << endl << endl;
    
    testConstructors();
    testInsertFunctions();
    testRemoveFunction();
    testFindFunction();
    
    cout << "基础测试完成!" << endl;
    return 0;
}