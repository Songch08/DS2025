/************************************************
*  2.cpp  字符串四则计算器（单文件版）
*  依赖：Stack.h   （你已实现）
*  支持：浮点数、+ - * / ^、括号、一元负号、除零/括号不匹配检测
*************************************************/
#include <iostream>
#include <string>
#include <cmath>
#include <cctype>
#include <sstream>
#include "../Stack.h"

using namespace std;

/* ---------- 计算器部分（原 Calculator.h + Calculator.cpp） ---------- */
static bool isOp(char c) {
    return c=='+'||c=='-'||c=='*'||c=='/'||c=='^';
}
static int prio(char op) {
    switch(op){
    case '+': case '-': return 1;
    case '*': case '/': return 2;
    case '^':           return 3;
    default:            return 0;
    }
}
static double apply(double a, char op, double b) {
    switch(op){
    case '+': return a + b;
    case '-': return a - b;
    case '*': return a * b;
    case '/':
        if (b == 0) throw "Divide by zero";
        return a / b;
    case '^': return pow(a, b);
    }
    throw "Unknown operator";
}

double calculate(const string& expr, string& err) {
    Stack<double> nums;
    Stack<char>   ops;
    bool mayUnary = true;
    try {
        for (size_t i = 0; i < expr.size(); ) {
            char c = expr[i];
            if (isspace(c)) { ++i; continue; }
            /* 数字（含小数） */
            if (isdigit(c) || c == '.') {
                string buf;
                while (i < expr.size() &&
                       (isdigit(expr[i]) || expr[i] == '.'))
                    buf += expr[i++];
                double v = stod(buf);
                nums.push(v);
                mayUnary = false;
                continue;
            }
            /* 左括号 */
            if (c == '(') {
                ops.push(c);
                mayUnary = true;
                ++i;
                continue;
            }
            /* 右括号 */
            if (c == ')') {
                while (!ops.empty() && ops.top() != '(') {
                    char op = ops.pop();
                    double b = nums.pop(), a = nums.pop();
                    nums.push(apply(a, op, b));
                }
                if (ops.empty()) { err = "Mismatched )"; return NAN; }
                ops.pop();                 // pop '('
                mayUnary = false;
                ++i;
                continue;
            }
            /* 运算符 */
            if (isOp(c)) {
                if (c == '-' && mayUnary) {   // 一元负号
                    nums.push(0.0);
                }
                while (!ops.empty() && ops.top() != '(' &&
                       (prio(ops.top()) > prio(c) ||
                        (prio(ops.top()) == prio(c) && c != '^'))) {
                    char op = ops.pop();
                    double b = nums.pop(), a = nums.pop();
                    nums.push(apply(a, op, b));
                }
                ops.push(c);
                mayUnary = true;
                ++i;
                continue;
            }
            /* 其他字符 */
            err = "Invalid char";
            return NAN;
        }
        /* 清空剩余运算符 */
        while (!ops.empty()) {
            char op = ops.pop();
            if (op == '(' || op == ')') { err = "Mismatched ("; return NAN; }
            double b = nums.pop(), a = nums.pop();
            nums.push(apply(a, op, b));
        }
        if (nums.size() != 1) { err = "Stack imbalance"; return NAN; }
        return nums.top();
    } catch (const char* e) {
        err = e;
        return NAN;
    }
}

/* ---------- 主函数 + 测试 ---------- */
void test(const string& s) {
    string err;
    double v = calculate(s, err);
    cout << left << s << "  =  ";
    if (!err.empty()) cout << "[ERROR: " << err << "]";
    else              cout << v;
    cout << endl;
}

int main() {
    cout << "==== 基本四则 + 括号 + 幂 ====\n";
    test("3 + 4 * 2 / (1 - 5) ^ 2 ^ 3");
    test("-4 + (2 - 3) * 5");
    test("3 ^ 2 ^ 2");
    test("(1 + 2) * 3 - 4 / 5");
    cout << "\n==== 异常 case ====\n";
    test("3 + + 4");
    test("3 + 4 )");
    test("3 / 0");
    cout << "\n==== 交互输入（Ctrl+D 结束） ====\n";
    string line;
    while (cout << "> " && getline(cin, line)) {
        string err;
        double v = calculate(line, err);
        if (!err.empty()) cout << "Error: " << err << "\n";
        else              cout << v << "\n";
    }
    return 0;
}