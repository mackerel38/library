#define PROBLEM "https://judge.yosupo.jp/problem/staticrmq"

#include "structure/sparsetable.hpp"

using namespace std;
using namespace poe;

int minimum(int left, int right) {
    return min(left, right);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<int> values(n);
    for (int& value : values) cin >> value;
    sparsetable<int, minimum> table(values);
    while (q--) {
        int left, right;
        cin >> left >> right;
        cout << table.prod(left, right) << '\n';
    }
}
