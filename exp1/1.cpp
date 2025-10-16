#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <chrono>  // 使用 chrono 来进行更高精度的计时
#include <string>
#include "../Vector.h"
#include <windows.h>  // 需要包含这个头文件来使用 MessageBox
using namespace std;
using namespace std::chrono;

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

double elapsed(steady_clock::time_point st, steady_clock::time_point ed) {
    return duration_cast<duration<double>>(ed - st).count();  // 返回秒数
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

    // 使用 std::chrono 进行计时
    auto t0 = steady_clock::now();  // 使用 steady_clock 开始计时
    cout << "开始执行 bubbleSort...\n";
    B.bubbleSort(0, B.size());
    auto t1 = steady_clock::now();  // 使用 steady_clock 结束计时
    cout << "bubbleSort 耗时：" << elapsed(t0, t1) << "秒\n";

    B = A;
    t0 = steady_clock::now();  // 再次使用 steady_clock
    cout << "开始执行 mergeSort...\n";
    B.mergeSort(0, B.size());
    t1 = steady_clock::now();
    cout << "mergeSort 耗时：" << elapsed(t0, t1) << "秒\n";

    /*---- 3. 区间查找 ----*/
    double m1 = 3.0, m2 = 7.0;
    Vector<Complex> sub = rangeSearch(B, m1, m2);
    printVec(sub, "模在[3,7]的子向量：\n");

    return 0;
}
