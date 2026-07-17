#include <bits/stdc++.h>
#include "graph/grid.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int floor_count, height, width;
    cin >> floor_count >> height >> width;
    vector grid(floor_count, vector<string>(height));
    for (auto& floor : grid) for (auto& row : floor) cin >> row;
    const auto compressed = layered_grid_components(
        floor_count, height, width,
        [&](int floor, int row, int column) { return grid[floor][row][column] != '#'; });

    vector<vector<long long>> coin(floor_count);
    for (int floor = 0; floor < floor_count; ++floor) {
        coin[floor].assign(compressed.layer[floor].count(), 0);
        for (int row = 0; row < height; ++row) for (int column = 0; column < width; ++column) {
            const int component = compressed.layer[floor].component[row][column];
            if (component != -1) coin[floor][component] += grid[floor][row][column] - '0';
        }
    }

    using candidate = pair<long long, int>;
    vector<vector<long long>> dp0(floor_count), dp1(floor_count), down_and_up(floor_count);
    vector<vector<vector<candidate>>> up0(floor_count), down1(floor_count);
    for (int floor = 0; floor < floor_count; ++floor) {
        const int count = compressed.layer[floor].count();
        dp0[floor].assign(count, 0);
        dp1[floor].assign(count, 0);
        down_and_up[floor].assign(count, 0);
        up0[floor].resize(count);
        down1[floor].resize(count);
    }

    auto descending = [](const candidate& left, const candidate& right) {
        return left.first > right.first;
    };
    for (int floor = floor_count - 1; floor >= 0; --floor) {
        for (int component = 0; component < compressed.layer[floor].count(); ++component) {
            long long best0 = 0, best1 = 0;
            if (floor + 1 < floor_count) {
                for (int upper : compressed.up[floor][component]) {
                    best0 = max(best0, dp0[floor + 1][upper]);
                    best1 = max(best1, dp1[floor + 1][upper]);
                }
            }
            dp0[floor][component] = coin[floor][component] + best0;
            dp1[floor][component] = coin[floor][component] + best1;
        }
        if (floor + 1 == floor_count) continue;

        for (int lower = 0; lower < compressed.layer[floor].count(); ++lower) {
            for (int upper : compressed.up[floor][lower]) {
                up0[floor][lower].emplace_back(
                    coin[floor][lower] + dp0[floor + 1][upper], upper);
            }
            ranges::sort(up0[floor][lower], descending);
        }

        for (int upper = 0; upper < compressed.layer[floor + 1].count(); ++upper) {
            for (int lower : compressed.down[floor + 1][upper]) {
                long long best = 0;
                for (int index = 0; index < min<int>(2, up0[floor][lower].size()); ++index) {
                    const auto [value, reached_upper] = up0[floor][lower][index];
                    best = max(best, value + (reached_upper == upper ? 0 : coin[floor + 1][upper]));
                }
                down_and_up[floor + 1][upper] = max(down_and_up[floor + 1][upper], best);
                down1[floor + 1][upper].emplace_back(best, lower);
            }
            ranges::sort(down1[floor + 1][upper], descending);
        }

        for (int lower = 0; lower < compressed.layer[floor].count(); ++lower) {
            for (int upper : compressed.up[floor][lower]) {
                long long best = 0;
                for (int index = 0; index < min<int>(2, down1[floor + 1][upper].size()); ++index) {
                    const auto [value, reached_lower] = down1[floor + 1][upper][index];
                    best = max(best, value + (reached_lower == lower ? 0 : coin[floor][lower]));
                }
                dp1[floor][lower] = max(dp1[floor][lower], best);
            }
        }
    }

    int query_count;
    cin >> query_count;
    while (query_count--) {
        int floor, row, column;
        cin >> floor >> row >> column;
        --floor, --row, --column;
        const int component = compressed.layer[floor].component[row][column];
        cout << max({dp0[floor][component], dp1[floor][component],
                     down_and_up[floor][component]}) << '\n';
    }
}
