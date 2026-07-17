#define PROBLEM "https://judge.yosupo.jp/problem/point_add_range_sum"

#include "structure/fenwicktree.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    fenwicktree<long long> sums(n);
    for (int index = 0; index < n; ++index) {
        long long value;
        cin >> value;
        sums.add(index, value);
    }

    while (q--) {
        int type;
        cin >> type;
        if (type == 0) {
            int index;
            long long delta;
            cin >> index >> delta;
            sums.add(index, delta);
        } else {
            int left, right;
            cin >> left >> right;
            cout << sums.sum(left, right) << '\n';
        }
    }
}
