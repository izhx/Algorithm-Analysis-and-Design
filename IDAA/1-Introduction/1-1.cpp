#include <iostream>
#include <vector>

using namespace std;

// exercises 1.1
// 4. floor(sqrt(n))
int floorOfSqrt(int n) {
    int i = 1;
    while (i * i <= n) ++i;
    return --i;
}

// 5. check same elements  最大比较次数？
vector<int> sortedCheck(vector<int> a, vector<int> b) {
    vector<int> same = {};
    int m = a.size(), n = b.size(), i = 0, j = 0;
    while (i < m && j < n) {
        if (a[i] == b[j]) {
            same.push_back(a[i]);
            i++, j++;
        } else if (a[i] < b[j]) {
            i++;
        } else {
            j++;
        }
    }
    return same;
}

// 11.a. extend euclid alg
int extendEuclid(int a, int b, int &x, int &y) {
    if (b == 0) {
        x = 1, y = 0;
        return a;
    }
    int d = extendEuclid(b, a % b, x, y), t = x;
    x = y, y = t - a / b * y;
    return d;
}

// 11.b. solve diophantine equation
bool diophantineEquation(int a, int b, int c, int &x, int &y) {
    int d = extendEuclid(a, b, x, y);
    if (c % d) return false;
    int k = c / d;
    x *= k, y *= k;
    return true;
}

// P3 Euclid gcd(m,n)
// input: m>n and not all is equal to 0
// output: gcd(m,n)
int Euclid(int m, int n) {
    while (n != 0) {
        int r = m % n;
        m = n;
        n = r;
    }
    return m;
}

// P5 sieve of Eratosthenes
// input: n>1
// output: vector
vector<int> Sieve(int n) {
    vector<int> L = {}, A = {0, 0};
    for (int p = 2; p < n; ++p)
        A.push_back(p);
    int fsn = floorOfSqrt(n);
    for (int p = 2; p < fsn; ++p) {
        if (A[p] != 0) {
            int j = p * p;
            while (j <= n) {
                A[j] = 0;
                j += p;
            }
        }
    }
    for (int p = 2; p < n; ++p)
        if (A[p] != 0) L.push_back(A[p]);
    return L;
}

int main() {
    int x = Euclid(60, 24);
    vector<int> l = Sieve(25);
    int y = floorOfSqrt(x);
    vector<int> a = {2, 5, 5, 5}, b = {2, 2, 3, 5, 5, 7};
    a = sortedCheck(a, b);
    int d = extendEuclid(15, 12, x, y);
    bool e = diophantineEquation(15, 12, 6, x, y);
    return 0;
}
