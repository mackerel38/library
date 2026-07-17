#define PROBLEM "https://atcoder.jp/contests/abc455/tasks/abc455_f"
#include <bits/stdc++.h>
#include "math/modint.hpp"
#include "utility/presetsegtree.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    using mint = staticmodint<998244353>;
    int n, query_count;
    cin >> n >> query_count;
    range_add_range_sum_square<mint> data(n);
    const mint inverse_two = mint{1} / 2;
    while (query_count--) {
        int left, right;
        long long add;
        cin >> left >> right >> add;
        --left;
        data.add(left, right, mint{add});
        const mint sum = data.sum(left, right);
        const mint square_sum = data.square_sum(left, right);
        cout << ((sum * sum - square_sum) * inverse_two).val() << '\n';
    }
}
