#define PROBLEM "https://judge.yosupo.jp/problem/enumerate_primes"
#include "template"
#include "prime"

int main() { IO();
    int T=1;
    // cin >> T;
    while (T--) solve();
}

void solve() {
    int n, a, b; cin >> n >> a >> b;
    int cnt = enumprimes(n);
    vi ans;
    for (int i=b; i<cnt; i+=a) ans.pb(primes[i]);
    cout << cnt << sp << ans.size() << nl;
    cout << ans;
}