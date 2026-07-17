#define PROBLEM "https://atcoder.jp/contests/abc462/tasks/abc462_g"
#include <bits/stdc++.h>
#include "fps/polynomial.hpp"
#include "math/combination.hpp"

using namespace std;
using namespace poe;

int main() {
    int n;
    cin >> n;
    vector<int> ball_count(n + 1), goal_count(n + 1);
    for (int i = 0, color; i < n; ++i) {
        cin >> color;
        ++ball_count[color];
    }
    for (int i = 0, color; i < n; ++i) {
        cin >> color;
        ++goal_count[color];
    }

    combination<modint998244353> comb(n);
    vector<fps998244353> factors;
    for (int color = 1; color <= n; ++color) {
        const int degree = min(ball_count[color], goal_count[color]);
        if (degree == 0) continue;
        fps998244353 factor(degree + 1);
        for (int selected = 0; selected <= degree; ++selected) {
            factor[selected] = comb.choose(ball_count[color], selected)
                * comb.choose(goal_count[color], selected)
                * comb.factorial(selected);
            if (selected & 1) factor[selected] = -factor[selected];
        }
        factors.push_back(std::move(factor));
    }
    const auto product = polynomial_product<998244353>(std::move(factors), n + 1);
    modint998244353 answer = 0;
    for (int selected = 0; selected < product.size_int(); ++selected) {
        answer += product[selected] * comb.factorial(n - selected);
    }
    cout << answer / comb.factorial(n) << '\n';
}
