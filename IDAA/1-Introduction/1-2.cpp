#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

// exercises 1.2
// 4. solve quadratic equation
bool quadraticEquation(double a, double b, double c, double &x1, double &x2) {
    double d = sqrt(b * b - 4 * a * c);
    if (d < 0) return false;
    x1 = (-b + d) / (2 * a);
    x2 = (-b - d) / (2 * a);
    return true;
}

// 5. convert dec to bin
int dec2bin(int n) {
    int b = 1, m = 0;
    while (n > 0) {
        m += (n % 2) * b;
        n /= 2;
        b *= 10;
    }
    return m;
}

// 9. min distance
int MinDistance(vector<int> a) {
    int d_min = INFINITY, n = a.size();
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i != j && abs(a[i] - a[j]) < d_min)
                d_min = abs(a[i] - a[j]);
        }
    }
    return d_min;
}

int myMinDistance(vector<int> a) {
    int d_min = INFINITY, n = a.size(), d = INFINITY;
    sort(a.begin(), a.end());  // sort ha ha ha ha
    for (int i = 1; i < n; ++i) {
        d = abs(a[i] - a[i - 1]);
        d_min = (d < d_min) ? d : d_min;
    }
    return d_min;
}

int main() {
    double x1 = 0, x2 = 0;
    quadraticEquation(4, 4, 1, x1, x2);
    int t = dec2bin(7);
    vector<int> a = {3, 6, 4, 9};
    int m = MinDistance(a), n = myMinDistance(a);
    return 0;
}
