#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Vector.h"

using namespace std;

// 测试函数
void testConstructors() {
    cout << "=== 测试构造函数 ===" << endl;
    
    // 默认构造函数
    Vector<int> v1;
    cout << "默认构造函数: size = " << v1.size() << ", empty = " << (v1.empty() ? "true" : "false") << endl;
    
    // 指定容量和大小的构造函数
    Vector<int> v2(10, 5, 42);
    cout << "指定容量和大小的构造函数: size = " << v2.size() << ", 第一个元素 = " << v2[0] << endl;
    
    // 数组构造函数
    int arr[] = {1, 2, 3, 4, 5};
    Vector<int> v3(arr, 5);
    cout << "数组构造函数: ";
    for (int i = 0; i < v3.size(); i++) {
        cout << v3[i] << " ";
    }
    cout << endl;
    
    // 拷贝构造函数
    Vector<int> v4(v3);
    cout << "拷贝构造函数: ";
    for (int i = 0; i < v4.size(); i++) {
        cout << v4[i] << " ";
    }
    cout << endl << endl;
}

void testAccessOperators() {
    cout << "=== 测试访问操作符 ===" << endl;
    
    int arr[] = {10, 20, 30, 40, 50};
    Vector<int> v(arr, 5);
    
    cout << "原向量: ";
    for (int i = 0; i < v.size(); i++) {
        cout << v[i] << " ";
    }
    cout << endl;
    
    // 修改元素
    v[2] = 35;
    cout << "修改索引2后的向量: ";
    for (int i = 0; i < v.size(); i++) {
        cout << v[i] << " ";
    }
    cout << endl << endl;
}

void testInsertRemove() {
    cout << "=== 测试插入和删除操作 ===" << endl;
    
    Vector<int> v;
    
    // 插入元素
    v.insert(0, 100);
    v.insert(1, 200);
    v.insert(1, 150);
    v.insert(300); // 在末尾插入
    
    cout << "插入元素后: ";
    for (int i = 0; i < v.size(); i++) {
        cout << v[i] << " ";
    }
    cout << ", size = " << v.size() << endl;
    
    // 删除单个元素
    int removed = v.remove(1);
    cout << "删除索引1的元素(" << removed << "): ";
    for (int i = 0; i < v.size(); i++) {
        cout << v[i] << " ";
    }
    cout << ", size = " << v.size() << endl;
    
    // 删除范围元素
    v.insert(250);
    v.insert(350);
    cout << "添加更多元素后: ";
    for (int i = 0; i < v.size(); i++) {
        cout << v[i] << " ";
    }
    cout << ", size = " << v.size() << endl;
    
    int removedCount = v.remove(1, 3);
    cout << "删除索引1到3的元素(共" << removedCount << "个): ";
    for (int i = 0; i < v.size(); i++) {
        cout << v[i] << " ";
    }
    cout << ", size = " << v.size() << endl << endl;
}

void testFindSearch() {
    cout << "=== 测试查找操作 ===" << endl;
    
    int arr[] = {10, 20, 30, 40, 50};
    Vector<int> v(arr, 5);
    
    // 查找元素
    int index = v.find(30);
    cout << "查找元素30的位置: " << index << endl;
    
    index = v.find(60);
    cout << "查找不存在的元素60的位置: " << index << endl;
    
    // 排序以便测试search
    v.sort();
    cout << "排序后向量: ";
    for (int i = 0; i < v.size(); i++) {
        cout << v[i] << " ";
    }
    cout << endl;
    
    // 搜索元素
    index = v.search(40);
    cout << "搜索元素40的位置: " << index << endl;
    
    index = v.search(15);
    cout << "搜索不存在的元素15的位置: " << index << endl << endl;
}

void testDeduplicateUniquify() {
    cout << "=== 测试去重操作 ===" << endl;
    
    // 测试deduplicate
    int arr1[] = {1, 3, 2, 3, 4, 2, 5};
    Vector<int> v1(arr1, 7);
    cout << "去重前(deduplicate): ";
    for (int i = 0; i < v1.size(); i++) {
        cout << v1[i] << " ";
    }
    cout << ", size = " << v1.size() << endl;
    
    int removed = v1.deduplicate();
    cout << "去重后(deduplicate): ";
    for (int i = 0; i < v1.size(); i++) {
        cout << v1[i] << " ";
    }
    cout << ", size = " << v1.size() << ", 移除元素数 = " << removed << endl;
    
    // 测试uniquify (需要先排序)
    int arr2[] = {1, 1, 2, 2, 3, 4, 4, 5};
    Vector<int> v2(arr2, 8);
    cout << "去重前(uniquify): ";
    for (int i = 0; i < v2.size(); i++) {
        cout << v2[i] << " ";
    }
    cout << ", size = " << v2.size() << endl;
    
    int reduced = v2.uniquify();
    cout << "去重后(uniquify): ";
    for (int i = 0; i < v2.size(); i++) {
        cout << v2[i] << " ";
    }
    cout << ", size = " << v2.size() << ", 减少数 = " << reduced << endl << endl;
}

void testSorting() {
    cout << "=== 测试排序操作 ===" << endl;
    
    srand(time(NULL));
    Vector<int> v;
    for (int i = 0; i < 10; i++) {
        v.insert(rand() % 100);
    }
    
    cout << "随机生成的向量: ";
    for (int i = 0; i < v.size(); i++) {
        cout << v[i] << " ";
    }
    cout << endl;
    
    v.sort();
    cout << "排序后的向量: ";
    for (int i = 0; i < v.size(); i++) {
        cout << v[i] << " ";
    }
    cout << endl << endl;
}

void testUnsort() {
    cout << "=== 测试打乱操作 ===" << endl;
    
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    Vector<int> v(arr, 10);
    
    cout << "原始向量: ";
    for (int i = 0; i < v.size(); i++) {
        cout << v[i] << " ";
    }
    cout << endl;
    
    v.unsort();
    cout << "打乱后的向量: ";
    for (int i = 0; i < v.size(); i++) {
        cout << v[i] << " ";
    }
    cout << endl << endl;
}

int main() {
    cout << "Vector类功能测试程序" << endl << endl;
    
    testConstructors();
    testAccessOperators();
    testInsertRemove();
    testFindSearch();
    testDeduplicateUniquify();
    testSorting();
    testUnsort();
    
    cout << "所有测试完成!" << endl;
    return 0;
}