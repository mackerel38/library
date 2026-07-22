#include <bits/stdc++.h>
#include "flow/tournament.hpp"

using namespace std;
using namespace poe;

vector<int> brute(int players, const vector<pair<int, int>>& decided) {
    vector played(players, vector<char>(players));
    vector<int> base(players);
    for (const auto& [winner, loser] : decided) {
        played[winner][loser] = played[loser][winner] = true;
        ++base[winner];
    }
    vector<pair<int, int>> undecided;
    for (int left = 0; left < players; ++left) {
        for (int right = left + 1; right < players; ++right) {
            if (!played[left][right]) undecided.push_back({left, right});
        }
    }
    vector<char> possible(players);
    for (int mask = 0; mask < (1 << undecided.size()); ++mask) {
        auto wins = base;
        for (int game = 0; game < static_cast<int>(undecided.size()); ++game) {
            ++wins[(mask >> game & 1) ? undecided[game].second : undecided[game].first];
        }
        for (int candidate = 0; candidate < players; ++candidate) {
            bool unique = true;
            for (int other = 0; other < players; ++other) {
                if (other != candidate && wins[other] >= wins[candidate]) unique = false;
            }
            if (unique) possible[candidate] = true;
        }
    }
    vector<int> result;
    for (int player = 0; player < players; ++player) if (possible[player]) result.push_back(player);
    return result;
}

int main() {
    mt19937 random(0);
    for (int trial = 0; trial < 3000; ++trial) {
        const int players = 2 + random() % 5;
        vector<pair<int, int>> decided;
        for (int left = 0; left < players; ++left) {
            for (int right = left + 1; right < players; ++right) {
                const int state = random() % 3;
                if (state == 1) decided.push_back({left, right});
                if (state == 2) decided.push_back({right, left});
            }
        }
        const auto expected = brute(players, decided);
        assert(possible_unique_roundrobin_winners(players, decided) == expected);
        for (const int winner : expected) {
            const auto completion = complete_roundrobin_for_unique_winner(
                players, decided, winner);
            assert(completion.possible);
            vector<int> wins(players);
            vector seen(players, vector<char>(players));
            for (const auto& [win, lose] : completion.results) {
                assert(!seen[win][lose]);
                seen[win][lose] = seen[lose][win] = true;
                ++wins[win];
            }
            for (const auto game : decided) {
                assert(ranges::find(completion.results, game) != completion.results.end());
            }
            for (int other = 0; other < players; ++other) {
                if (other != winner) assert(wins[winner] > wins[other]);
            }
        }
    }
}
