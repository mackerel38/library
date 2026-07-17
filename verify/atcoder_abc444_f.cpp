#include <bits/stdc++.h>
#include "algorithm/balancedsplit.hpp"
#include "algorithm/binarysearch.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int test_count;
    cin >> test_count;
    while (test_count--) {
        int n;
        long long operations;
        cin >> n >> operations;
        vector<long long> length(n);
        long long total = 0, maximum = 0;
        for (long long& value : length) {
            cin >> value;
            total += value;
            maximum = max(maximum, value);
        }
        const long long required = (n + operations + 1) / 2;
        auto feasible = [&](long long median) {
            vector<pair<long long, long long>> pieces;
            long long count = 0;
            const long long initial = ranges::count_if(
                length, [&](long long value) { return value >= median; });
            if (required - min(required, initial) > operations) return false;
            const long long threshold = max(2LL, 2 * median - 1);
            for (long long value : length) {
                for (const auto [piece, frequency] :
                     balanced_split_leaves(value, threshold)) {
                    if (piece >= median) {
                        pieces.emplace_back(piece, frequency);
                        count += frequency;
                    }
                }
            }
            if (count < required) return false;
            ranges::sort(pieces);
            long long need = required;
            long long selected_sum = 0;
            for (const auto [piece, frequency] : pieces) {
                const long long take = min(need, frequency);
                selected_sum += piece * take;
                need -= take;
                if (need == 0) break;
            }
            return total - selected_sum >= required - 1;
        };
        cout << *last_true(1LL, maximum + 1, feasible) << '\n';
    }
}
