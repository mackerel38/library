#include <bits/stdc++.h>
#include "math/modint.hpp"
#include "structure/dsu.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    weighted_dsu<long long> constraints(n + 1);
    for (int i = 0; i < m; ++i) {
        int left, right;
        long long sum;
        cin >> left >> right >> sum;
        --left;
        sum -= right - left;
        if (sum < 0 || !constraints.merge(left, right, sum)) {
            cout << 0 << '\n';
            return 0;
        }
    }

    vector<int> interval_masks;
    vector<long long> sums;
    int used_mask = 0;
    for (int left = 0; left < n; ++left) {
        for (int right = left + 1; right <= n; ++right) {
            if (!constraints.same(left, right)) continue;
            const long long sum = constraints.diff(left, right);
            if (sum < 0) {
                cout << 0 << '\n';
                return 0;
            }
            const int mask = ((1 << right) - 1) ^ ((1 << left) - 1);
            interval_masks.push_back(mask);
            sums.push_back(sum);
            used_mask |= mask;
            break;
        }
    }

    const int equation_count = interval_masks.size();
    vector<vector<int>> bit_sums(equation_count, vector<int>(1 << n));
    for (int equation = 0; equation < equation_count; ++equation) {
        for (int bits = 0; bits < (1 << n); ++bits) {
            bit_sums[equation][bits] = popcount(static_cast<unsigned>(bits & interval_masks[equation]));
        }
    }

    struct countinfo {
        modint998244353 count;
        bool exists = false;
    };
    unordered_map<int, countinfo> dp;
    dp[0] = {1, true};
    for (int bit = 0; bit < 30; ++bit) {
        unordered_map<int, countinfo> next;
        for (const auto& [state, information] : dp) {
            for (int bits = 0; bits < (1 << n); ++bits) {
                int next_state = 0;
                bool valid = true;
                for (int equation = 0; equation < equation_count; ++equation) {
                    const int carry = state >> (3 * equation) & 7;
                    const int required = sums[equation] >> bit & 1;
                    const int total = carry + bit_sums[equation][bits];
                    if ((total & 1) != required) {
                        valid = false;
                        break;
                    }
                    next_state |= (total - required) / 2 << (3 * equation);
                }
                if (!valid) continue;
                auto& destination = next[next_state];
                destination.count += information.count;
                destination.exists |= information.exists;
            }
        }
        dp = std::move(next);
    }

    const auto iterator = dp.find(0);
    if (iterator == dp.end() || !iterator->second.exists) {
        cout << 0 << '\n';
    } else if (used_mask != (1 << n) - 1) {
        cout << "Infinity\n";
    } else {
        cout << iterator->second.count << '\n';
    }
}
