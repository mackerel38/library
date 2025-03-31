#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_2_A"
#include "template"
#include "segtree"

int main() { IO();
    int T=1;
    // cin >> T;
    while (T--) solve();
}

void solve() {
    int n, q; cin >> n >> q;
    segtree<ll, [](ll a, ll b) { return min(a, b); }> seg(n, (1LL<<31)-1);
    while (q--) {
        ll x, y, z; cin >> x >> y >> z;
        if (x == 0) {
            seg.set(y, z);
        } else {
            cout << seg.prod(y, z+1) << nl;
        }
    }
}