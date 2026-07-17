#include <bits/stdc++.h>
#include "math/modint.hpp"
#include "structure/dsu.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    vector<int> u(m), v(m);
    for (int i = 0; i < m; ++i) {
        cin >> u[i] >> v[i];
        --u[i];
        --v[i];
    }
    using mint = staticmodint<998244353>;
    vector<mint> cost(m);
    mint power = 2;
    for (int i = 0; i < m; ++i) {
        cost[i] = power;
        power *= 2;
    }

    dsu components(n);
    mint answer = 0;
    for (int i = m - 1; i >= 0; --i) {
        if (components.same(u[i], v[i])) continue;
        if (components.count() > 2) components.merge(u[i], v[i]);
        else answer += cost[i];
    }
    cout << answer.val() << '\n';
}
