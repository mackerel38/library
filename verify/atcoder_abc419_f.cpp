#include <bits/stdc++.h>
#include "string/trie.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    constexpr int mod = 998244353;
    int n, length;
    cin >> n >> length;
    ahocorasick<char> automaton('a', 'z');
    for (int id = 0; id < n; ++id) {
        string pattern;
        cin >> pattern;
        automaton.add(pattern);
    }
    automaton.build();

    vector<unsigned> matched(automaton.size());
    for (int state = 0; state < automaton.size(); ++state) {
        matched[state] = automaton.pattern_mask<unsigned>(state);
    }
    const int masks = 1 << n;
    vector<vector<int>> dp(masks, vector<int>(automaton.size()));
    dp[0][0] = 1;
    for (int position = 0; position < length; ++position) {
        vector<vector<int>> next_dp(masks, vector<int>(automaton.size()));
        for (int mask = 0; mask < masks; ++mask) {
            for (int state = 0; state < automaton.size(); ++state) {
                if (dp[mask][state] == 0) continue;
                for (char symbol = 'a'; symbol <= 'z'; ++symbol) {
                    const int next_state = automaton.next(state, symbol);
                    const int next_mask = mask | matched[next_state];
                    int& value = next_dp[next_mask][next_state];
                    value += dp[mask][state];
                    if (value >= mod) value -= mod;
                }
            }
        }
        dp.swap(next_dp);
    }
    int answer = 0;
    for (int value : dp.back()) {
        answer += value;
        if (answer >= mod) answer -= mod;
    }
    cout << answer << '\n';
}
