#include <iostream>
#include <cstdio>
#include <vector>
#include <fstream>

#define INF 65536  // 2^16

using namespace std;

int _cost(int des, vector<int> &cost, const vector<vector<int>> &G, const vector<int> &level);

int _mf(int i, int w, vector<vector<int>> &K, const vector<int> &weight, const vector<int> &value);

int _c(int i, int j, vector<vector<int>> &C, vector<vector<int>> &Ck, const vector<int> &shape);

void SplitString(const std::string &s, std::vector<std::string> &v, const std::string &c) {
    std::string::size_type pos1, pos2;
    pos1 = 0, pos2 = s.find(c);
    while (std::string::npos != pos2) {
        v.push_back(s.substr(pos1, pos2 - pos1));
        pos1 = pos2 + c.size();
        pos2 = s.find(c, pos1);
    }
    if (pos1 != s.length())
        v.push_back(s.substr(pos1));
}

string &trim(string &str, string::size_type pos = 0) {
    static const string delim = " \t";  //删除空格或者tab字符
    pos = str.find_first_of(delim, pos);
    if (pos == string::npos)
        return str;
    return trim(str.erase(pos, 1));
}

// 0. fibonacci number
int Fib(int n) {
    int f_1 = 1, f_2 = 1, t = 1;
    while (n-- > 2) {
        t = f_1 + f_2;
        f_1 = f_2;
        f_2 = t;
    }
    return t;
}


/**
 * 1. Multistage Graph problem，一维DP 记忆化搜索
 * @param G NxN matrix
 * @return minimum of cost
 */
int multi_graph_min_path(const vector<vector<int>> &G) {
    vector<int> cost(G.size(), -1);
    vector<int> level(G.size(), 0);
    for (int i = 0; i < G.size(); ++i)
        for (int j = i + 1; j < G.size(); ++j)
            if (G[i][j] > 0)
                level[j] = level[i] + 1;
    return _cost(G.size() - 1, cost, G, level);
}

int _cost(const int des, vector<int> &cost, const vector<vector<int>> &G,
          const vector<int> &level) {
    if (cost[des] == -1) {
        if (level[des] == 1) cost[des] = G[0][des];
        else if (level[des] > 1) {  // 如果des在第三层及以后
            int minimum = INF;
            for (int k = 0; k < des; ++k) {
                if (level[k] == level[des] - 1 && G[k][des] > 0) {  // 如果k点是j的前驱
                    int c = _cost(k, cost, G, level) + G[k][des];
                    minimum = minimum < c ? minimum : c;
                }
            }
            cost[des] = minimum;
        }
    }
    return cost[des];
}

/**
 * 2. shortest path of all pairs 二维DP
 * @param G
 * @return
 */
vector<vector<int>> floyd(vector<vector<int>> G) {
    int path = 0, n = G.size();
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i == j) continue;
            for (int k = 0; k < n; ++k) {
                if (k == j) continue;
                path = G[j][i] + G[i][k];
                G[j][k] = path < G[j][k] ? path : G[j][k];
            }
        }
    }
    return G;
}

/**
 * 3. Binomial coefficient.
 * @param n
 * @param k
 * @return
 */
int binomial(const int n, const int k) {
    vector<vector<int>> C(n + 1, vector<int>(k + 1, 0));
    for (int i = 0; i <= n; ++i) {
        for (int j = 0; j <= min(i, k); ++j) {
            if (j == 0 || j == i) C[i][j] = 1;
            else C[i][j] = C[i - 1][j - 1] + C[i - 1][j];
        }
    }
    return C[n][k];
}


/**
 * 4. Longest path in DAG.
 * @param G
 * @return
 */
int longest_path(const vector<vector<int>> &G) {
    const int N = G.size();
    vector<int> dil(N, 0), dad(N, -1), in_degree(N, 0), zero_degree;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (G[j][i] > 0) in_degree[i]++;
        }
    }
    while (zero_degree.size() < N) {
        for (int v = 0; v < N; ++v) {
            if (in_degree[v] == 0) {
                zero_degree.push_back(v);
                in_degree[v] = -1;
                for (int u = 0; u < N; ++u) {
                    if (G[v][u] > 0) in_degree[u]--;
                    if (dil[v] < dil[u] + G[u][v]) {
                        dil[v] = dil[u] + G[u][v];
                        dad[v] = u;
                    }
                }
            }
        }
    }
    int maxi = 0;
    for (auto d: dil)
        maxi = d > maxi ? d : maxi;
    return maxi;
}


/**
 * 5. Edit distance
 * @param x
 * @param y
 * @return
 */
int edit_dis(const string &x, const string &y) {
    const int m = x.length(), n = y.length();
    vector<vector<int>> E(m + 1, vector<int>(n + 1, -1));
    for (int i = 0; i <= m; ++i) E[i][0] = i;
    for (int j = 0; j <= n; ++j) E[0][j] = j;
    for (int i = 1; i <= m; ++i) {
        int diff = 0;
        for (int j = 1; j <= n; ++j) {
            E[i][j] = E[i - 1][j] < E[i][j - 1] ?
                      E[i - 1][j] + 1 : E[i][j - 1] + 1;
            if (x[i] == y[j]) diff = 0;
            else diff = 1;
            E[i][j] = E[i][j] < E[i - 1][j - 1] + diff ?
                      E[i][j] : E[i - 1][j - 1] + diff;
        }
    }
    return E[m][n];
}

/**
 * 6. knapsack
 * @param W
 * @param weight
 * @param value
 * @return
 */
int knapsack(const int W, const vector<int> &weight, const vector<int> &value) {
    vector<vector<int>> K(weight.size(), vector<int>(W + 1, -1));
    for (int i = 0; i < weight.size(); ++i) K[i][0] = 0;
    for (int w = 0; w <= W; ++w) K[0][w] = 0;
    return _mf(weight.size() - 1, W, K, weight, value);
}

int _mf(const int i, const int w, vector<vector<int>> &K,
        const vector<int> &weight, const vector<int> &value) {
    if (K[i][w] == -1) {
        if (w < weight[i]) K[i][w] = _mf(i - 1, w, K, weight, value);
        else {
            K[i][w] = value[i] + _mf(i - 1, w - weight[i], K, weight, value);
            K[i][w] = K[i][w] > _mf(i - 1, w, K, weight, value) ?
                      K[i][w] : K[i - 1][w];
        }
    }
    return K[i][w];
}


/**
 * 7. OptimalBST
 * @param P
 * @return
 */
double OptimalBST(const vector<double> &P, vector<vector<int>> &R) {
    const int n = P.size();
    vector<vector<double>> C(n + 1, vector<double>(n + 1, 0));
//    vector<vector<int>> R(n + 1, vector<int>(n + 1, -1));
    for (int i = 0; i < n; ++i) {
        C[i][i] = P[i];
        R[i][i] = i;
    }
    for (int d = 1; d <= n - 1; ++d) {
        for (int i = 0; i < n - d; ++i) {
            int j = i + d, k_min = INF;
            double min_val = INF;
            for (int k = i; k <= j; ++k) {
                if (C[i][k - 1] + C[k + 1][j] < min_val) {
                    min_val = C[i][k - 1] + C[k + 1][j];
                    k_min = k;
                }
            }
            R[i][j] = k_min;
            double sum = P[i];
            for (int s = i + 1; s <= j; ++s) sum += P[s];
            C[i][j] = min_val + sum;
        }
    }
    return C[0][n - 1];
}


/**
 * 8. chain matrix multiplication
 * @param shape
 * @return
 */
int chain_matrix_multiplication(const vector<int> &shape) {
    vector<vector<int>> C(shape.size() - 1, vector<int>(shape.size() - 1, -1)),
            Ck(shape.size() - 1, vector<int>(shape.size() - 1, -1));
    for (int i = 0; i < shape.size() - 2; ++i) {
//        C[i][i] = 0;
        C[i][i + 1] = shape[i] * shape[i + 1] * shape[i + 2];
    }
    return _c(0, shape.size() - 2, C, Ck, shape);
}

int _c(const int i, const int j, vector<vector<int>> &C,
       vector<vector<int>> &Ck, const vector<int> &shape) {
    if (C[i][j] == -1) {
        int mini = INF, tc = 0;
        for (int k = i + 1; k < j; ++k) {
            tc = _c(i, k, C, Ck, shape) + _c(k + 1, j, C, Ck, shape) +
                 shape[i] * shape[k + 1] * shape[j + 1];
            mini = mini < tc ? mini : tc;
            Ck[i][j] = mini < tc ? Ck[i][j] : k;
        }
        C[i][j] = mini;
    }
    return C[i][j];
}

///////////////////////////////////////////////////
long fac(long n, vector<long> &vec) {
    if (n >= vec.size())
        for (long i = vec.size(); i <= n; ++i)
            vec.push_back(vec[i - 1] * i);
    return vec[n];
}

long dp(long n, long m, vector<vector<long>> &DP) {
    if (DP[n][m] == -1) {
        DP[n][m] = dp(n - 1, m, DP) * (n - 1) + dp(n - 1, m - 1, DP);
    }
    return DP[n][m];
}

/**
 * n个人排队，从前往后看，高的会挡住矮的，已知看到了m个人，问有几种站法
 * 示例： n=3, m=2 : 132 231 213  三种
 * @param n
 * @param m
 * @return
 */
long see(long n, long m) {
    vector<vector<long>> DP(n + 1, vector<long>(m + 1, -1));
    vector<long> factorial(1, 1);
    for (long i = 0; i <= m; ++i) DP[i][i] = 1;
    for (long i = 1; i <= n; ++i) DP[i][1] = fac(i - 1, factorial);
    return dp(n, m, DP);
}

int main() {
    see(3, 2);
    cout << "0: " << Fib(4) << endl;
    ifstream ifstream1(R"(C://Workspace/luogu/data/g1.csv)");
    string line;
    vector<vector<int>> g1;
    while (getline(ifstream1, line)) {
        line = trim(line);
        vector<string> sv;
        SplitString(line, sv, ",");
        vector<int> row(0);
        for (const auto &s:sv)row.push_back(stoi(s));
        g1.push_back(row);
    }
    int a = multi_graph_min_path(g1);
    cout << "1: " << a << endl;  // 16
    vector<vector<int>> wg = {{0,   INF, 4, INF},
                              {1,   0,   6, 3},
                              {INF, INF, 0, INF},
                              {INF, 5,   1, 0}};
    vector<vector<int>> b = floyd(wg);
    cout << "2: " << endl;
    int c = binomial(3, 1);
    cout << "3: " << c << endl;
    vector<vector<int>> g2 = {{0, 6, 0, 0, 0, 0},
                              {0, 0, 0, 1, 2, 0},
                              {4, 0, 0, 3, 0, 0},
                              {0, 0, 0, 0, 1, 0},
                              {0, 0, 0, 0, 0, 0},
                              {1, 0, 2, 0, 0, 0}};
    int d = longest_path(g2);
    cout << "4: " << d << endl;
    string x = "EXPONENTIAL", y = "POLYNOMIAL";
    int e = edit_dis(x, y);
    cout << "5: " << e << endl;
    int f = knapsack(10, {2, 1, 3, 2}, {12, 10, 20, 15});
    cout << "6: " << f << endl;
    vector<double> p = {0.1, 0.2, 0.4, 0.3};
    vector<vector<int>> r(4, vector<int>(4, -1));
    double g = OptimalBST(p, r);
    cout << "7: " << g << endl;
    int h = chain_matrix_multiplication({50, 20, 1, 10, 100});
    cout << "8: " << h << endl;
    return 0;
}