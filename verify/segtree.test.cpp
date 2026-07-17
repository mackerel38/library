#define PROBLEM "https://judge.yosupo.jp/problem/point_set_range_composite"

#include "math/modint.hpp"
#include "structure/segtree.hpp"

using namespace std;
using namespace poe;

using mint = modint998244353;

struct affine {
    mint a;
    mint b;
};

affine compose(affine left, affine right) {
    return {right.a * left.a, right.a * left.b + right.b};
}

affine identity() {
    return {1, 0};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    cin >> n >> q;
    vector<affine> functions(n);
    for (auto& [a, b] : functions) {
        cin >> a >> b;
    }
    segtree<affine, compose, identity> seg(functions);
    while (q--) {
        int type;
        cin >> type;
        if (type == 0) {
            int index;
            affine function;
            cin >> index >> function.a >> function.b;
            seg.set(index, function);
        } else {
            int left, right;
            mint x;
            cin >> left >> right >> x;
            const affine function = seg.prod(left, right);
            cout << function.a * x + function.b << '\n';
        }
    }
}
