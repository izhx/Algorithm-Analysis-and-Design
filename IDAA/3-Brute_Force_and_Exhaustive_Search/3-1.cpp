#include <cstdio>
#include <vector>
#include <cmath>

using namespace std;

// exercise 3.1
// 4.a
long p(long x, vector<long> a) {
    long p = 0;
    for (int i = 0; i < a.size(); ++i) {
        long pi = a[i];
        for (int j = 0; j < i; ++j) pi *= x;
        p += pi;
    }
    return p;
}

// 4.b
long pp(long x, vector<long> a) {
    long p = 0;
    for (int i = 0; i < a.size(); ++i)
        p += a[i] * (long) pow(x, i);
    return p;
}

// 5.
// a[i][i] = true
// ring: 0, star: 1, full: 2
int topologicalStructure(vector<vector<bool>> a) {
    int n = 0;
    for (int i = 0; i < a.size(); ++i)
        for (int j = 0; j < a.size(); ++j)
            if (!a[i][j]) n++;
    if (n == 3 * a.size()) return 0;
    else if (n > 3 * a.size()) return 2;
    return 1;
}

// 12.b
vector<int> BubbleSortPlus(vector<int> a) {
    for (int i = 0; i < a.size() - 1; ++i) {
        bool nochange = true;
        for (int j = 0; j < a.size() - 1 - i; ++j)
            if (a[j + 1] < a[j]) {
                int t = a[j];
                a[j] = a[j + 1], a[j + 1] = t;
                nochange = false;
            }
        if (nochange) break;
    }
    return a;
}


// P76
vector<int> SelectionSort(vector<int> a) {
    for (int i = 0; i < a.size(); ++i) {
        int min = i, t = a[i];
        for (int j = i + 1; j < a.size(); ++j)
            if (a[j] < a[min]) min = j;
        a[i] = a[min], a[min] = t;
    }
    return a;
}

// P77
vector<int> BubbleSort(vector<int> a) {
    for (int i = 0; i < a.size() - 1; ++i)
        for (int j = 0; j < a.size() - 1 - i; ++j)
            if (a[j + 1] < a[j]) {
                int t = a[j];
                a[j] = a[j + 1], a[j + 1] = t;
            }
    return a;
}

int main() {
    return 0;
}
