#include <cstdio>

// exercise 2.5
// 8.
int Fib(int n) {
    int f_1 = 1, f_2 = 1, t = 1;
    while (n-- > 2) {
        t = f_1 + f_2;
        f_1 = f_2;
        f_2 = t;
    }
    return t;
}

// 12.a
int rf(int n) {
    if (n < 3) return 1;
    int result = rf(n - 1) + rf(n - 2);
    if (result > 99999) result -= 99999;
    return result;
}

// 12.b
int nrf(int n) {
    int f_1 = 1, f_2 = 1, t = 1;
    while (n-- > 2) {
        t = f_1 + f_2;
        if (t > 99999) t -= 99999;
        f_1 = f_2;
        f_2 = t;
    }
    return t;
}

int main() {
    int t = Fib(4), tt = rf(30), ttt = nrf(30);
    return 0;
}
