#define PROBLEM "https://judge.yosupo.jp/problem/point_set_range_composite"

#include "math/modint.hpp"
#include "utility/presetsegtree.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    using mint = modint998244353;
    using data_type = point_set_range_composite<mint>;
    int n, query_count;
    cin >> n >> query_count;
    vector<data_type::affine> functions(n);
    for (auto& function : functions) cin >> function.multiply >> function.add;
    data_type data(functions);
    while (query_count--) {
        int type;
        cin >> type;
        if (type == 0) {
            int index;
            mint multiply, add;
            cin >> index >> multiply >> add;
            data.set(index, multiply, add);
        } else {
            int left, right;
            mint value;
            cin >> left >> right >> value;
            cout << data.apply(left, right, value) << '\n';
        }
    }
}
