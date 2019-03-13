#include <iostream>
#include <vector>

using namespace std;

vector<double> intersectionOf2lines(vector<double> p1, vector<double> q1, vector<double> p2, vector<double> q2);

class Line {
public:
    double a;
    double b;
    double c;

    Line(vector<double> p, vector<double> q) {
        this->a = 1, this->b = 0, this->c = 0;
        if (p[0] == q[0]) {
            this->c = -p[0];
        } else {
            this->b = (q[0] - p[0]) / (p[1] - q[1]);
            this->c = -p[0] - this->b * p[1];
        }
    };
};

// exercises 1.3
// 1. comparison counting sort
vector<int> ComparisonCountingSort(vector<int> a) {
    vector<int> s, count;
    for (int i = 0; i < a.size(); ++i) {
        count.push_back(0);
        s.push_back(0);
    }
    for (int i = 0; i < a.size() - 1; ++i)
        for (int j = i + 1; j < a.size(); ++j)
            if (a[i] < a[j]) count[j] += 1;
            else count[i] += 1;
    for (int i = 0; i < a.size(); ++i)
        s[count[i]] = a[i];
    return s;
}

// 3. sample string match
int sampleStrMatch(string str, string model) {
    for (unsigned int i = 0; i < str.length(); ++i)
        if (str.substr(i, model.length()) == model) return i;
    return -1;
}

// 9. same cycle
double dis2(double x1, double y1, double x2, double y2) {
    return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
}

bool sameCycle(vector<vector<double>> points) {
    if (points.size() < 4) return true;
    vector<double> p = intersectionOf2lines(points[0], points[1], points[2], points[3]);
    if (p.empty()) return false;
    double rr = dis2(p[0], p[1], points[0][0], points[0][1]);
    for (int i = 2; i < points.size(); ++i)
        if (dis2(p[0], p[1], points[i][0], points[i][1]) != rr) return false;
    return true;
}

// 10. intersectionOf2lines (use Cramer's Rule)
vector<double> intersectionOf2lines(vector<double> p1, vector<double> q1, vector<double> p2, vector<double> q2) {
    Line l1 = Line(p1, q1), l2 = Line(p2, q2);
    double d = l1.a * l2.b - l1.b * l2.a;
    if (d == 0) return {};
    else return {l2.c * l1.b - l1.c * l2.b, l1.c * l2.a - l2.c * l1.a};
}


int main() {
    vector<int> l = ComparisonCountingSort({60, 35, 81, 98, 14, 47});
    string a = "123456789", b = a.substr(2, 3);
    int i = sampleStrMatch(a, b);
    bool x = sameCycle({{3,  4},
                        {5,  0},
                        {-3, -4},
                        {3,  -4}});
    return 0;
}
