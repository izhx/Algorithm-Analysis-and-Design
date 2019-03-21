#include <cstdio>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

// exercise 3.3
// 3.a
int aPostOffice(vector<double> x) {
    double p = 0;
    for (double xi : x) p += xi;
    p /= x.size();
    int mi = 0;
    double min = abs(x[mi] - p), d = 0;
    for (int i = 0; i < x.size(); ++i) {
        d = abs(x[i] - p);
        if (d < min) {
            min = d;
            mi = i;
        }
    }
    return mi;
}

// 3.b
int bPostOffice(vector<double> x) {
    sort(x.begin(), x.end());
    int index = 0;
    double maximum = abs(x.front() - x.back()), m = 0;
    for (int i = 0; i < x.size(); ++i) {
        m = max(abs(x[i] - x.front()), abs(x[i] - x.back()));
        if (m < maximum) {
            maximum = m;
            index = i;
        }
    }
    return index;
}

// 9.
vector<int> twoPoleOfConvexHull(vector<vector<double>> ps) {
    int l = 0, r = 0;
    double max = ps[0][0], min = ps[0][0];
    for (int i = 1; i < ps.size(); ++i) {
        if (ps[i][0] < min) {
            l = i;
            min = ps[i][0];
        }
        if (ps[i][0] > max) {
            r = i;
            max = ps[i][0];
        }
    }
    return {l, r};
}

// 11.
vector<vector<int>> bruteForceConvexHull(vector<vector<double>> ps) {
    vector<vector<int>> edge;
    for (int i = 0; i < ps.size(); ++i) {
        for (int j = i + 1; j < ps.size(); ++j) {
            double a = ps[j][1] - ps[i][1];
            double b = ps[i][0] - ps[j][0];
            double c = ps[i][0] * ps[j][1] - ps[i][1] * ps[j][0];
            bool flag = true, former = true, now = true;
            for (int k = 0; k < ps.size(); ++k) {
                if (k == i || k == j) continue;
                now = a * ps[k][0] + b * ps[k][1] < c;
                if (flag) {
                    former = now;
                    flag = false;
                }
                if (now != former) break;
                former = now;
            }
            if (former == now) edge.push_back({i, j});
        }
    }
    return edge;
}

int main() {
    vector<double> x = {1, 2, -1, -3, 3};
    int a = aPostOffice(x), b = bPostOffice(x);
    vector<vector<double >> ps = {{0,  2},
                                  {1,  1},
                                  {-2, 0},
                                  {1,  0},
                                  {0,  1}};
    vector<int> p1 = twoPoleOfConvexHull(ps);
    vector<vector<int >> p2 = bruteForceConvexHull(ps);
    return 0;
}
