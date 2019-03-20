#include <cstdio>
#include <stack>

using namespace std;

struct Han {
    int n;
    char a, b, c;
    bool unsolved;
};

void hanoi(int n, char a, char b, char c) {
    if (n == 1) printf("move %d from %c to %c. \n", n, a, c);
    else {
        hanoi(n - 1, a, c, b);      //将a柱子上的从上到下n-1个盘移到b柱子上
        printf("move %d from %c to %c. \n", n, a, c);
        hanoi(n - 1, b, a, c);      //将b柱子上的n-1个盘子移到c柱子上
    }
}

// exercise 2.4
// 5.c
void nonRecursiveHanoi(Han h) {
    stack<Han> s;
    s.push(h);
    while (!s.empty()) {
        h = s.top();
        s.pop();
        if (h.n == 1) {
            printf("move %d from %c to %c. \n", h.n, h.a, h.c);
            s.top().unsolved = false;
        } else if (h.unsolved) {
            s.push({h.n - 1, h.b, h.a, h.c, true});
            s.push({h.n, h.a, h.b, h.c, false});
            s.push({h.n - 1, h.a, h.c, h.b, true});
        } else {
            printf("move %d from %c to %c. \n", h.n, h.a, h.c);
        }
    }
}

int main() {
    //hanoi(3, 'a', 'b', 'c');
    nonRecursiveHanoi({4, 'a', 'b', 'c', true});
    return 0;
}
