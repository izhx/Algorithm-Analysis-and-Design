#include <iostream>
#include <vector>

using namespace std;

// 2.3
// 4. 1^2+2^2+3^2+ ... n^2
int Mystery(int n) {
    int s = 0;
    for (int i = 1; i <= n; ++i)
        s += i * i;
    return s;
}

// 5.
double Secret(vector<double> a) {
    double min = a[0], max = a[0];
    for (int i = 0; i < a.size(); ++i) {
        min = a[i] < min ? a[i] : min;
        max = a[i] > max ? a[i] : max;
    }
    return max - min;
}

// 6.
bool Enigma(vector<vector<double>> a) {
    for (int i = 0; i < a.size() - 1; ++i)
        for (int j = i + 1; j < a.size(); ++j)
            if (a[i][j] != a[j][i]) return false;
    return true;
}

// 11.
void GE(vector<vector<double>> &a) {
    for (int i = 0; i < a.size() - 1; ++i)
        for (int j = i + 1; j < a.size(); ++j)
            for (int k = a.size(); k >= i; --k)
                a[j][k] -= a[i][k] * a[j][i] / a[i][i];
}

// P48
int MaxElement(vector<int> a) {
    int max_val = a[0];
    for (int i = 0; i < a.size(); ++i)
        if (a[i] > max_val) max_val = a[i];
    return max_val;
}

// P49
bool UniqueElements(vector<int> a) {
    for (int i = 0; i < a.size(); ++i)
        for (int j = i + 1; j < a.size(); ++j)
            if (a[i] == a[j]) return false;
    return true;
}

// P50
vector<vector<double>> MatrixMultiplication(vector<vector<double>> a, vector<vector<double>> b) {
    unsigned int n = a.size();
    vector<vector<double>> c(n, vector<double>(n, 0));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            for (int k = 0; k < n; ++k)
                c[i][j] += a[i][k] * b[k][j];
    return c;
}

// P51
int Binary(int n) {
    int count = 1;
    while (n > 1) {
        count++;
        n /= 2;
    }
    return count;
}


int main() {
    int a = Mystery(2);
    double b = Secret({1, 4, 2, 6});
    bool c = Enigma({{1, 2, 3}, {2, 1, 4}, {3, 4, 2}});
    int d = MaxElement({1, 9, 2});
    bool e = UniqueElements({1, 3, 5, 5});
    vector<vector<double>> f = MatrixMultiplication({{1, 1}, {1, 1}}, {{1, 1}, {1, 1}});
    return 0;
}
