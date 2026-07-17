#include <bits/stdc++.h>
#include "structure/segtree.hpp"
#include "structure/topk.hpp"

using namespace std;
using namespace poe;

using value_type = long long;
using state = topkdistinct<value_type, int, 4>;
constexpr value_type negative_infinity = -(1LL << 60);

state merge_state(state first, const state& second) {
    return merge_topkdistinct(first, second);
}

state empty_state() {
    state result;
    for (int key = -4; key < 0; ++key) result.add(negative_infinity, key);
    return result;
}

state single_state(int key, value_type value) {
    state result = empty_state();
    result.add(max(value, negative_infinity), key);
    return result;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<int> genre(n);
    vector<value_type> interest(n);
    vector<vector<int>> positions(n);
    for (int i = 0; i < n; ++i) {
        cin >> genre[i] >> interest[i];
        --genre[i];
        positions[genre[i]].push_back(i);
    }

    vector<state> prefix(n + 1), suffix(n + 1);
    prefix[0] = suffix[n] = empty_state();
    for (int i = 0; i < n; ++i) {
        prefix[i + 1] = prefix[i];
        prefix[i + 1].add(interest[i], genre[i]);
    }
    for (int i = n - 1; i >= 0; --i) {
        suffix[i] = suffix[i + 1];
        suffix[i].add(interest[i], genre[i]);
    }

    vector<value_type> original(n);
    vector<state> initial(n);
    vector<vector<pair<int, value_type>>> changes(n);
    for (int i = 0; i < n; ++i) {
        vector<pair<value_type, int>> candidates;
        for (int j = 0; j < prefix[i].size(); ++j) {
            const auto candidate = prefix[i][j];
            if (candidate.second != genre[i]) candidates.push_back(candidate);
        }
        original[i] = interest[i] + candidates[0].first + candidates[1].first;
        initial[i] = single_state(genre[i], original[i]);
        for (int j = 0; j < 2; ++j) {
            if (candidates[j].second < 0) continue;
            const value_type changed = original[i] - candidates[j].first + candidates[2].first;
            changes[candidates[j].second].push_back({i, changed});
        }
    }

    segtree<state, merge_state, empty_state> tree(initial);
    value_type answer = -1;
    for (int banned_genre = 0; banned_genre < n; ++banned_genre) {
        for (const auto [index, value] : changes[banned_genre])
            tree.set(index, single_state(genre[index], value));
        for (int middle = 0; middle < static_cast<int>(positions[banned_genre].size()); ++middle) {
            const int i = positions[banned_genre][middle];
            vector<pair<value_type, int>> candidates;
            for (int j = 0; j < suffix[i + 1].size(); ++j) {
                const auto candidate = suffix[i + 1][j];
                if (candidate.second != genre[i]) candidates.push_back(candidate);
            }
            const state best = tree.prod(0, i);
            for (int j = 0; j < best.size(); ++j) {
                const auto [left_value, left_genre] = best[j];
                if (left_genre == genre[i]) continue;
                value_type right_value;
                if (left_genre == candidates[0].second)
                    right_value = candidates[1].first + candidates[2].first;
                else if (left_genre == candidates[1].second)
                    right_value = candidates[0].first + candidates[2].first;
                else
                    right_value = candidates[0].first + candidates[1].first;
                answer = max(answer, left_value + interest[i] + right_value);
            }
        }
        for (const auto [index, value] : changes[banned_genre]) {
            static_cast<void>(value);
            tree.set(index, single_state(genre[index], original[index]));
        }
    }
    cout << answer << '\n';
}
