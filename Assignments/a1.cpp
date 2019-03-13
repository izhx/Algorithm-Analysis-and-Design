#include <iostream>
#include <vector>

using namespace std;

template<typename T>

vector<T> ListReverse(vector<T> list) {
    int n = list.size();
    T t;
    for (int i = 0; i < n / 2; ++i) {
        t = list[i];
        list[i] = list[n - i - 1];
        list[n - i - 1] = t;
    }
    return list;
}

int main() {
    vector<int> a = {1, 2, 3}, b = ListReverse(a);
    return 0;
}
