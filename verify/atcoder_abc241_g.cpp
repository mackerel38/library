#include <bits/stdc++.h>
#include "flow/tournament.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    vector<pair<int, int>> results(m);
    for (auto& [winner, loser] : results) {
        cin >> winner >> loser;
        --winner;
        --loser;
    }
    const auto winners = possible_unique_roundrobin_winners(n, results);
    for (int i = 0; i < static_cast<int>(winners.size()); ++i) {
        if (i) cout << ' ';
        cout << winners[i] + 1;
    }
    cout << '\n';
}
