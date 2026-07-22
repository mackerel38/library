#define PROBLEM "https://judge.yosupo.jp/problem/queue_operate_all_composite"

#include "math/modint.hpp"
#include "structure/swag.hpp"

using namespace std;
using namespace poe;

using mint = staticmodint<998244353>;

struct affinevalue {
    mint a, b;
};

affinevalue compose(affinevalue first, affinevalue second) {
    return {second.a * first.a, second.a * first.b + second.b};
}

affinevalue affine_identity() {
    return {1, 0};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int q;
    cin >> q;
    swag<affinevalue, compose, affine_identity> functions;
    while (q--) {
        int type;
        cin >> type;
        if (type == 0) {
            int a, b;
            cin >> a >> b;
            functions.push({a, b});
        } else if (type == 1) {
            functions.pop();
        } else {
            int x;
            cin >> x;
            const auto [a, b] = functions.prod();
            cout << a * mint(x) + b << '\n';
        }
    }
}
