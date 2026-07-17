#include <bits/stdc++.h>
#include "fps/powerprojection.hpp"
#include "fps/rational.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    using mint = modint998244353;
    using series = fps998244353;
    int n, side_count, player_count;
    cin >> n >> side_count >> player_count;
    series die(n), ones(n, mint{1});
    const mint probability = mint{side_count}.inv();
    for (int i = 0; i < side_count; ++i) {
        int step;
        cin >> step;
        if (step < n) die[step] = probability;
    }

    series survive = power_projection(die, ones);
    survive.push_back(0);
    vector<rationalfps<998244353>> terms;
    terms.reserve(n);
    mint last_answer = 0;
    for (int turn = 1; turn <= n; ++turn) {
        const mint finish = survive[turn - 1] - survive[turn];
        last_answer += finish * survive[turn].pow(player_count - 1);
        if (survive[turn - 1] == mint{}) continue;
        const mint weight = finish * survive[turn - 1].pow(player_count - 1);
        const mint ratio = survive[turn] / survive[turn - 1];
        terms.push_back({series{weight}, series{1, -ratio}});
    }
    const series first_answers = rational_sum(std::move(terms)).prefix(player_count - 1);
    for (const mint answer : first_answers) cout << answer.val() << '\n';
    cout << last_answer.val() << '\n';
}
