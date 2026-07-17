#define PROBLEM "https://judge.yosupo.jp/problem/range_affine_range_sum"

#include "math/modint.hpp"
#include "structure/lazysegtree.hpp"

using namespace std;
using namespace poe;

using mint = modint998244353;

struct range_value {
    mint sum;
    int size;
};

struct affine {
    mint a;
    mint b;
};

range_value combine(range_value left, range_value right) {
    return {left.sum + right.sum, left.size + right.size};
}

range_value empty_range() {
    return {0, 0};
}

range_value apply_affine(affine function, range_value value) {
    return {function.a * value.sum + function.b * value.size, value.size};
}

affine compose_affine(affine newer, affine older) {
    return {newer.a * older.a, newer.a * older.b + newer.b};
}

affine identity_affine() {
    return {1, 0};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    cin >> n >> q;
    vector<range_value> values(n);
    for (auto& value : values) {
        cin >> value.sum;
        value.size = 1;
    }
    lazysegtree<range_value, combine, empty_range, affine, apply_affine,
                     compose_affine, identity_affine>
        seg(values);
    while (q--) {
        int type, left, right;
        cin >> type >> left >> right;
        if (type == 0) {
            affine function;
            cin >> function.a >> function.b;
            seg.apply(left, right, function);
        } else {
            cout << seg.prod(left, right).sum << '\n';
        }
    }
}
