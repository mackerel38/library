#define PROBLEM "https://judge.yosupo.jp/problem/point_add_range_sum"
#include "template"
#include "BIT"

int main() { IO();
    int T=1;
    // cin >> T;
    while (T--) solve();
}

void solve() {
    int n, q; cin >> n >> q;
    BIT<ll> s(n);
    rep(i, n) {
        int x; cin >> x;
        s.set(i, x);
    }
    while (q--) {
        int x, y, z; cin >> x >> y >> z;
        if (x == 0) {
            s.add(y, z);
        } else {
            cout << s.sum(y, z) << nl;
        }
    }
}