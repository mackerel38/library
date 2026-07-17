#define PROBLEM "https://judge.yosupo.jp/problem/static_range_sum"

#include "structure/prefixsum.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<long long> values(n);
    for (auto& value : values) cin >> value;
    prefixsum<long long> sums(values);
    while (q--) {
        int left, right;
        cin >> left >> right;
        cout << sums.sum(left, right) << '\n';
    }
}
