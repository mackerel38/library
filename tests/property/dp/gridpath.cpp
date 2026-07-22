#include <bits/stdc++.h>
#include "dp/gridpath.hpp"

using namespace std;
using namespace poe;

int main() {
    mt19937 random(259);
    for (int trial = 0; trial < 3000; ++trial) {
        const int height = 1 + random() % 6;
        const int width = 1 + random() % 6;
        const int group_count = 1 + random() % 6;
        vector<vector<gridpathpoint>> groups(group_count);
        for (int row = 0; row < height; ++row) {
            for (int column = 0; column < width; ++column) {
                groups[random() % group_count].push_back({row, column});
            }
        }

        modint998244353 expected = 0;
        for (const auto& group : groups) {
            vector<vector<char>> belongs(height, vector<char>(width));
            for (const auto [row, column] : group) belongs[row][column] = true;
            for (const auto start : group) {
                vector<vector<modint998244353>> dp(height, vector<modint998244353>(width));
                dp[start.row][start.column] = 1;
                for (int row = start.row; row < height; ++row) {
                    for (int column = start.column; column < width; ++column) {
                        if (row == start.row && column == start.column) continue;
                        if (row > start.row) dp[row][column] += dp[row - 1][column];
                        if (column > start.column) dp[row][column] += dp[row][column - 1];
                    }
                }
                for (const auto finish : group) expected += dp[finish.row][finish.column];
            }
        }
        assert(count_grouped_monotone_grid_paths<998244353>(height, width, groups)
               == expected);
    }
}
