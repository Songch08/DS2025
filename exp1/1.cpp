#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <string>
#include "../Vector.h"
#include <windows.h>  
using namespace std;

struct Complex {
    double real, imag;
    Complex(double r = 0, double i = 0): real(r), imag(i) {}

    double mod() const { return sqrt(real * real + imag * imag); }

    bool operator==(const Complex& o) const {
        return real == o.real && imag == o.imag;
    }

    bool operator!=(const Complex& o) const {
        return !(*this == o);
    }

    bool operator<(const Complex& o) const {
        double m1 = mod(), m2 = o.mod();
        if (m1 != m2) return m1 < m2;
        return real < o.real;
    }

    bool operator>(const Complex& o) const {
        double m1 = mod(), m2 = o.mod();
        if (m1 != m2) return m1 > m2;
        return real > o.real;
    }

    bool operator<=(const Complex& o) const {
        return (*this < o) || (*this == o);
    }

    friend ostream& operator<<(ostream& os, const Complex& c) {
        os << '(' << c.real << ',' << c.imag << 'i' << ')';
        return os;
    }
};

Complex randComplex(int maxR = 10) {
    return Complex(rand() % maxR, rand() % maxR);
}

template <typename T>
void printVec(const Vector<T>& V, const char* info = "") {
    cout << info;
    for (int i = 0; i < V.size(); ++i) 
        cout << V[i] << ' ';
    cout << endl;
}

// 使用 clock() 计时
double elapsed(clock_t start, clock_t end) {
    return double(end - start) / CLOCKS_PER_SEC;
}

Vector<Complex> rangeSearch(const Vector<Complex>& V, double m1, double m2) {
    Vector<Complex> res;
    for (int i = 0; i < V.size(); ++i) {
        double m = V[i].mod();
        if (m >= m1 && m <= m2) 
            res.insert(res.size(), V[i]);
    }
    return res;
}

int main() {
    SetConsoleOutputCP(CP_UTF8);

    srand((unsigned)time(nullptr));
    const int N = 20;          
    Vector<Complex> A;

    for (int i = 0; i < N; ++i)
        A.insert(A.size(), randComplex());
    printVec(A, "初始向量（含重复）：\n");

    A.unsort(0, A.size());
    printVec(A, "置乱后：\n");

    Complex key(5, 3);
    int pos = A.find(key, 0, A.size());
    cout << "查找 " << key << " ："
         << (pos >= 0 ? "找到，下标" + to_string(pos) : "未找到") << "\n\n";

    A.insert(3, Complex(99, 99));
    printVec(A, "在位置3插入(99,99i)后：\n");

    A.remove(3);
    printVec(A, "删除位置3后：\n");

    A.deduplicate();
    printVec(A, "deduplicate后：\n");

    Vector<Complex> B = A;

    clock_t start, end;

    cout << "开始执行 bubbleSort...\n";
    start = clock();
    B.bubbleSort(0, B.size());
    end = clock();
    cout << "bubbleSort 耗时：" << elapsed(start, end) << " 秒\n";

    B = A;
    cout << "开始执行 mergeSort...\n";
    start = clock();
    B.mergeSort(0, B.size());
    end = clock();
    cout << "mergeSort 耗时：" << elapsed(start, end) << " 秒\n";

    double m1 = 3.0, m2 = 7.0;
    Vector<Complex> sub = rangeSearch(B, m1, m2);
    printVec(sub, "模在[3,7]的子向量：\n");

    return 0;
}
