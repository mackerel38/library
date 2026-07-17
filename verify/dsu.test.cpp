#define PROBLEM "https://judge.yosupo.jp/problem/unionfind"

#include "structure/dsu.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    cin >> n >> q;
    dsu uf(n);
    while (q--) {
        int type, u, v;
        cin >> type >> u >> v;
        if (type == 0) {
            uf.merge(u, v);
        } else {
            cout << uf.same(u, v) << '\n';
        }
    }
}
