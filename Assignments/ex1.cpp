#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

/**
 * experiment 1.1  Tournament scheduling
 */
void copyTable(vector<vector<int>> &table, int n) {
    int m = n / 2, i, j;
    for (i = 0; i < m; i++)
        for (j = 0; j < m; j++) {
            table[i][j + m] = table[i][j] + m;
            table[i + m][j] = table[i][j] + m;
            table[i + m][j + m] = table[i][j];
        }
}

void Tournament(vector<vector<int>> &table, int n) {
    if (n == 1) {
        table[0][0] = 1;
        return;
    }
    Tournament(table, n / 2);
    copyTable(table, n);
}

/**
 * experiment 1.2 k-th smallest element problem
 * @tparam T: a comparable type
 * @param A: a vector
 * @param k: 0 <= k < a.size()
 * @return k-th smallest element
 */
template<typename T>
T SelectKth(vector<T> A, int k) {
    if (A.size() < 75) {
        sort(A.begin(), A.end());
        return A[k];
    }
    int m = ceil(A.size() / 5);
    vector<vector<T>> G(m, vector<T>{});
    for (auto a : A) {  // divide
        G[m].push_back(a);
        if (G[m].size() == 5)m--;
    }
    vector<T> M, L, R;
    for (auto g : G) {  // find median set
        sort(g.begin(), g.end());
        M.push_back(g[g.size() / 2]);
    }
    T x = SelectKth(M, G.size() / 2);
    for (auto a: A) {
        if (a < x) L.push_back(a);
        else R.push_back(a);
    }
    if (k <= L.size()) return SelectKth(L, k);
    else return SelectKth(R, k - L.size());
}

// experiment 1.3  closet pair see EfficientClosetPair()
struct Point {
    double x, y;

    double d2(Point p) {
        return (x - p.x) * (x - p.x) + (y - p.y) * (y - p.y);
    }
};

double EfficientClosetPair(vector<Point> P) {
    double d = 2147483647;
    int n = P.size(), i = 0, j = 1;
    if (n <= 3) {  // brute force
        for (i = 0; i < n; ++i)
            for (j = 0; j < n; ++j)
                if (j != i)
                    d = min(P[i].d2(P[j]), d);
    } else {  // divide and conquer
        sort(P.begin(), P.end(), [](Point &a, Point &b) -> bool { return a.x > b.x; });
        vector<Point> pl, pr, s;
        for (i = 0; i < n / 2; ++i) pl.push_back(P[i]);
        for (i = n / 2; i < n; ++i) pr.push_back(P[i]);
        double dl = EfficientClosetPair(pl);
        double dr = EfficientClosetPair(pr);
        d = dl < dr ? dl : dr;
        double m = P[(unsigned int) ceil(n / 2) - 1].x;
        for (i = 0; i < n; ++i)
            if (abs(P[i].x - m) < d) s.push_back(P[i]);
        sort(s.begin(), s.end(), [](Point &a, Point &b) -> bool { return a.y > b.y; });
        d *= d;
        for (i = 0; i < s.size() - 1; ++i, j = i + 1)
            while (j < s.size() && (s[j].y - s[i].y) * (s[j].y - s[i].y) < d)
                d = min(s[j++].d2(s[i]), d);
    }
    return sqrt(d);
}

int main() {
    vector<int> a = {8, 3, 2, 9, 7, 1, 5, 4};
    int x = SelectKth(a, 0);
    vector<vector<int>> t(8, vector<int>(8, 0));
    Tournament(t, 8);
    vector<Point> p = {{-1.5, 0},
                       {0,    0},
                       {0,    1.8},
                       {-1.5, 1}};
    double d = EfficientClosetPair(p);
    cout << x << endl << d;
    return 0;
}
