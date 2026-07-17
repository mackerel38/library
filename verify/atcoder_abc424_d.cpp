#define PROBLEM "https://atcoder.jp/contests/abc424/tasks/abc424_d"
#include <bits/stdc++.h>
#include "dp/profile.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int tests;
    cin >> tests;
    while (tests--) {
        int height, width;
        cin >> height >> width;
        vector<uint64_t> rows(height);
        for (auto& mask : rows) {
            string row;
            cin >> row;
            for (int column = 0; column < width; ++column) {
                if (row[column] == '#') mask |= uint64_t{1} << column;
            }
        }
        const auto result = minimum_submask_profile(
            rows, width, [](uint64_t upper, uint64_t lower) {
                const uint64_t common = upper & lower;
                return (common & (common << 1)) == 0;
            });
        cout << result.cost << '\n';
    }
}
