#include <iostream>
#include <random>
#include "../Vector.h"
#include "../Stack.h"
#include <windows.h> 
using namespace std;

mt19937 rng(random_device{}());
uniform_int_distribution<int> lenD(1, 100000);
uniform_int_distribution<int> valD(0, 10000);

int largestRectangleArea(const Vector<int>& heights) {
    int n = heights.size();
    Stack<int> stk;         
    int maxA = 0;
    for (int i = 0; i <= n; ++i) {
        int h = (i == n ? 0 : heights[i]);
        while (!stk.empty() && heights[stk.top()] > h) {
            int topIdx = stk.pop();
            int leftBound = stk.empty() ? -1 : stk.top();
            int width = i - leftBound - 1;
            maxA = max(maxA, heights[topIdx] * width);
        }
        stk.push(i);
    }
    return maxA;
}

void printHead(const Vector<int>& v, int k = 20) {
    cout << "[";
    int lim = min(k, v.size());
    for (int i = 0; i < lim; ++i) {
        cout << v[i];
        if (i + 1 < lim) cout << ", ";
    }
    if (v.size() > lim) cout << ", ...";
    cout << "]";
}

void randomTests() {
    for (int t = 1; t <= 10; ++t) {
        int n = lenD(rng);
        Vector<int> h;
        for (int i = 0; i < n; ++i) h.insert(h.size(), valD(rng));
        cout << "Case " << t << "  n=" << n << "  heights=";
        printHead(h);
        cout << "  -> 最大面积 = " << largestRectangleArea(h) << '\n';
    }
}

int main() {
    SetConsoleOutputCP(CP_UTF8);

    int a1[] = {2, 1, 5, 6, 2, 3};
    Vector<int> ex1;
    for (int v : a1) ex1.insert(ex1.size(), v);
    cout << "示例1: ";
    printHead(ex1);
    cout << "  -> " << largestRectangleArea(ex1) << endl;   

    int a2[] = {2, 4};
    Vector<int> ex2;
    for (int v : a2) ex2.insert(ex2.size(), v);
    cout << "示例2: ";
    printHead(ex2);
    cout << "  -> " << largestRectangleArea(ex2) << endl;
    randomTests();
    return 0;
}