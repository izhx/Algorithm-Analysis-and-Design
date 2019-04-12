#include <iostream>
#include <vector>

using namespace std;

// P133 merge sort
template<typename T>
void Merge(vector<T> B, vector<T> C, vector<T> &A) {
    int i = 0, j = 0, k = 0, p = B.size(), q = C.size();
    while (i < p && j < q)
        if (B[i] <= C[j]) A[k++] = B[i++];
        else A[k++] = C[j++];
    if (i == p) while (j < q) A[k++] = C[j++];
    else while (i < p) A[k++] = B[i++];
}

template<typename T>
void MergeSort(vector<T> &A) {
    int n = A.size();
    if (n < 2) return;
    vector<T> B, C;
    for (int i = 0; i < n / 2; ++i) B.push_back(A[i]);
    for (int i = n / 2; i < n; ++i) C.push_back(A[i]);
    MergeSort(B);
    MergeSort(C);
    Merge(B, C, A);
}


int main() {
    vector<int> a = {8, 3, 2, 9, 7, 1, 5, 4};
    MergeSort(a);
    return 0;
}
