#include <bits/stdc++.h>
#include "math/modint.hpp"
#include "math/randomwalk.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long n, m;
    cin >> n >> m;
    using mint = staticmodint<1000000007>;
    cout << uniform_multiplicative_stopping_expectation<mint>(n, m) << '\n';
}
