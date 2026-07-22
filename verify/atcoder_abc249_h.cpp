#include <bits/stdc++.h>
#include "math/combination.hpp"
#include "math/modint.hpp"
#include "math/randomwalk.hpp"

using namespace std;
using namespace poe;

using mint = staticmodint<998244353>;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<int> count(n);
    for (int i = 0; i < n; ++i) {
        int color;
        cin >> color;
        ++count[color - 1];
    }

    const combination<mint> comb(n);
    const mint inverse_two = mint{1} / 2;
    vector<mint> inverse_power_two(n + 1, 1);
    for (int i = 1; i <= n; ++i) inverse_power_two[i] = inverse_power_two[i - 1] * inverse_two;
    vector<vector<mint>> transition(n);
    for (int current = 0; current < n; ++current) {
        transition[current].resize(current + 2);
        for (int selected = 0; selected <= current; ++selected) {
            const mint probability = comb.choose(current, selected) * inverse_power_two[current];
            const mint add_probability
                = (mint{selected} + mint{n - current} * inverse_two) / n;
            const int remaining = current - selected;
            transition[current][remaining] += probability * (mint{1} - add_probability);
            transition[current][remaining + 1] += probability * add_probability;
        }
    }

    const auto potential = upward_skipfree_potential(
        transition, vector<mint>(n, mint{1} / n));
    mint answer = -potential[n];
    for (const int frequency : count) answer += potential[frequency];
    cout << answer.val() << '\n';
}
