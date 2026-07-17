#include <bits/stdc++.h>
#include "math/combination.hpp"
#include "math/modint.hpp"
#include "structure/mo.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int test_count;
    cin >> test_count;
    vector<pair<int, int>> input(test_count);
    int limit = 0;
    for (auto& [n, x] : input) {
        cin >> n >> x;
        limit = max(limit, n);
    }
    combination<modint998244353> combinations(limit);
    Mo2d mo(limit, limit);
    vector<int> query_id(test_count, -1);
    vector<int> query_x;
    vector<modint998244353> answers(test_count);
    for (int id = 0; id < test_count; ++id) {
        const auto [n, x] = input[id];
        if (abs(x) >= n) {
            answers[id] = abs(x);
        } else {
            const int m = (n + x + 1) / 2;
            query_id[id] = mo.add(n, m);
            query_x.push_back(x);
        }
    }

    int n = 0, m = 0;
    modint998244353 f = 0, g = 0;
    vector<modint998244353> query_answer(mo.size());
    const modint998244353 inverse_two = modint998244353{1} / 2;
    mo.run(
        [&](int old_n) {
            assert(old_n == n);
            const auto coefficient = combinations.choose(n, m - 1);
            const auto old_f = f;
            g = 2 * g + old_f - modint998244353(m) * coefficient;
            f = 2 * old_f - coefficient;
            ++n;
        },
        [&](int old_n) {
            assert(old_n == n);
            --n;
            const auto coefficient = combinations.choose(n, m - 1);
            const auto new_f = (f + coefficient) * inverse_two;
            g = (g - new_f + modint998244353(m) * coefficient) * inverse_two;
            f = new_f;
        },
        [&](int old_m) {
            assert(old_m == m);
            const auto coefficient = combinations.choose(n, m);
            f += coefficient;
            g += modint998244353(m) * coefficient;
            ++m;
        },
        [&](int old_m) {
            assert(old_m == m);
            --m;
            const auto coefficient = combinations.choose(n, m);
            f -= coefficient;
            g -= modint998244353(m) * coefficient;
        },
        [&](int id) {
            const int x = query_x[id];
            query_answer[id] = -modint998244353(x) +
                ((modint998244353(n + x) * f - 2 * g) /
                 modint998244353{2}.pow(n - 1));
        }
    );
    for (int id = 0; id < test_count; ++id) {
        if (query_id[id] != -1) answers[id] = query_answer[query_id[id]];
        cout << answers[id] << '\n';
    }
}
