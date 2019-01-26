#include <iostream>
#include "function.h"

using std::cout;

struct heavy_callable {
    int operator () (int a, int b, int c) {
        return a * a + b * b - c * c;
    }

private:
    int big_boi[1000];
};

int main() {
    auto a = [](int a, int b) -> int {
        return a + b;
    };
    function<int(int, int)> f(a);
    int result = f(2, 3);
    cout << result;

    heavy_callable hc;
    function<int(int, int, int)> f2(hc);
    f2(2, 3, 4);

    return 0;
}