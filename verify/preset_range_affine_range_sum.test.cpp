#define PROBLEM "https://judge.yosupo.jp/problem/range_affine_range_sum"

#include "math/modint.hpp"
#include "utility/presetsegtree.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    using mint = modint998244353;
    int n, query_count;
    cin >> n >> query_count;
    vector<mint> values(n);
    for (mint& value : values) cin >> value;
    range_affine_range_sum<mint> data(values);
    while (query_count--) {
        int type, left, right;
        cin >> type >> left >> right;
        if (type == 0) {
            mint multiply, add;
            cin >> multiply >> add;
            data.affine(left, right, multiply, add);
        } else {
            cout << data.sum(left, right) << '\n';
        }
    }
}
