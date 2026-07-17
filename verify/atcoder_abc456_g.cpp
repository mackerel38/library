#include <bits/stdc++.h>
#include "math/combination.hpp"
#include "math/modint.hpp"

using namespace std;
using namespace poe;

using mint = staticmodint<998244353>;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    string s;
    cin >> n >> s;
    combination<mint> comb(n);
    vector<mint> power2(n + 1, 1);
    for (int i = 1; i <= n; ++i) power2[i] = power2[i - 1] * 2;

    auto bounded = [&](int length, int limit) {
        mint result = 0;
        for (int count = 0; (limit + 2) * count - 1 <= length; ++count) {
            const int rest = length - (limit + 2) * count;
            mint term = 0;
            if (rest >= 0) {
                term += power2[rest] * comb.choose(length - (limit + 1) * count, count);
            }
            if (rest + 1 >= 0) {
                term += power2[rest + 1] *
                        comb.choose(length - (limit + 1) * count, count - 1);
            }
            result += (count % 2 == 0 ? term : -term);
        }
        return result;
    };

    const int optional = count(s.begin(), s.end(), '.');
    vector<mint> at_most(n + 1, power2[optional]);
    for (int left = 0; left < n;) {
        if (s[left] == 'x') {
            ++left;
            continue;
        }
        int right = left;
        while (right < n && s[right] == '.') ++right;
        const int length = right - left;
        const mint inverse_all = mint{1} / power2[length];
        for (int limit = 1; limit < length; ++limit) {
            at_most[limit] *= bounded(length, limit) * inverse_all;
        }
        left = right;
    }
    at_most[0] = 1;
    for (int limit = 1; limit <= n; ++limit) {
        cout << (at_most[limit] - at_most[limit - 1]).val() << '\n';
    }
}
