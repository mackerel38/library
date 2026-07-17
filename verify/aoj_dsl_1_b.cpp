#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/DSL_1_B"

#include "structure/dsu.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    weighted_dsu<long long> uf(n);
    while (q--) {
        int type, x, y;
        cin >> type >> x >> y;
        if (type == 0) {
            long long difference;
            cin >> difference;
            uf.merge(x, y, difference);
        } else if (uf.same(x, y)) {
            cout << uf.diff(x, y) << '\n';
        } else {
            cout << "?\n";
        }
    }
}
