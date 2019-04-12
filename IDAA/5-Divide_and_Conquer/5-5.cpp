#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

// P150 Closet Pair, 书上的算法有问题
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
    vector<Point> p = {{-1.5, 0},
                       {0,    0},
                       {0,    1.8},
                       {-1.5, 1}};
    double d = EfficientClosetPair(p);
    cout << d;
    return 0;
}
