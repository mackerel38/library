#include <bits/stdc++.h>
#include "math/modint.hpp"
#include "math/occupancy.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, a, b, c;
    cin >> n >> a >> b >> c;
    cout << count_nonempty_subset_occupancies<modint998244353>(n, {a, b, c}) << '\n';
}
