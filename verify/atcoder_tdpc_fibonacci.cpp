#include <bits/stdc++.h>
#include "fps/bostanmori.hpp"

using namespace std;
using namespace poe;

using mint = modint1000000007;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int k;
    long long n;
    cin >> k >> n;
    vector<mint> initial(k, 1);
    vector<mint> coefficient(k, 1);
    cout << fast_linear_recurrence<1000000007>(initial, coefficient, n - 1).val() << '\n';
}
