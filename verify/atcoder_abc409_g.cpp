#define PROBLEM "https://atcoder.jp/contests/abc409/tasks/abc409_g"
#include <bits/stdc++.h>
#include "fps/transform.hpp"
#include "math/modint.hpp"

using namespace std;
using namespace poe;

int main() {
    using mint = modint998244353;
    int n, probability_percent;
    cin >> n >> probability_percent;
    const mint probability = mint{probability_percent} / 100;
    const mint repeat_probability = 1 - probability;

    vector<mint> growth(n - 1, 1);
    for (int remaining = 1; remaining < n - 1; ++remaining) {
        growth[remaining] = growth[remaining - 1]
            * (mint{n - remaining + 1} - probability) / (n - remaining);
    }
    fps998244353 first_time_value(n - 1);
    for (int index = 0; index < n - 1; ++index) {
        first_time_value[index] = growth[n - index - 2];
    }
    const auto transformed = upper_binomial_transform<998244353>(
        first_time_value, repeat_probability
    );

    vector<mint> answer(n);
    mint power = probability;
    for (int label = 2; label <= n; ++label) {
        answer[label - 1] = power * transformed[label - 2];
        power *= probability;
    }
    answer[0] = n;
    for (int label = 2; label <= n; ++label) answer[0] -= answer[label - 1];
    for (const mint value : answer) cout << value << '\n';
}
