class Fib{
private:
    int f,g;
public:
    Fib ( int n)
    {f = 1;g = 0; while ( g < n) next();}
    int get() { return g;}
    int next() { g += f; f = g - f; return g;}//转至下一项，O(1)时间
    int prev() { f = g - f; g -=f; return g;}//转至上一项，O(1)时间
};
