#include <iostream>
#include <vector>

using namespace std;

// 1.3
// P36
int SequentialSearch(vector<int> a, int k) {
    int i = 0, n = a.size();
    while (i < n && a[i] != k) i++;
    return i < n ? i : -1;
}


int main() {
    int n = SequentialSearch({60, 35, 81, 98, 14, 47},35);
    return 0;
}
