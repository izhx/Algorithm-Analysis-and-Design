#include <iostream>
#include <vector>

using namespace std;

// P136 quick sort
template<typename T>
int Partition(vector<T> &A, int l, int r) {
    T p = A[l];
    while (l < r) {
        while (l < r && A[r] > p) r--;
        if (l < r) A[l++] = A[r];
        while (l < r && A[l] < p) l++;
        if (l < r) A[r--] = A[l];
    }
    A[l] = p;
    return l;
}

template<typename T>
void QuickSort(vector<T> &A, int l, int r) {
    if (l >= r) return;
    int s = Partition(A, l, r);
    QuickSort(A, l, s - 1);
    QuickSort(A, s + 1, r);
}

int main() {
    vector<int> a = {8, 3, 2, 9, 7, 1, 5, 4};
    QuickSort(a, 0, a.size() - 1);
    return 0;
}
