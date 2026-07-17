#define PROBLEM "https://atcoder.jp/contests/abc407/tasks/abc407_g"
#include <bits/stdc++.h>
#include "flow/matching.hpp"

using namespace std;
using namespace poe;

int main() {
    int height, width;
    cin >> height >> width;
    vector values(height, vector<long long>(width));
    long long answer = 0;
    for (auto& row : values) {
        for (long long& value : row) {
            cin >> value;
            answer += value;
        }
    }
    vector<int> left_id(height * width, -1), right_id(height * width, -1);
    int left_size = 0, right_size = 0;
    for (int row = 0; row < height; ++row) {
        for (int column = 0; column < width; ++column) {
            const int cell = row * width + column;
            if ((row + column) % 2 == 0) left_id[cell] = left_size++;
            else right_id[cell] = right_size++;
        }
    }
    vector<weightedbipartiteedge<long long>> edges;
    constexpr int dr[] = {-1, 0, 1, 0};
    constexpr int dc[] = {0, 1, 0, -1};
    for (int row = 0; row < height; ++row) {
        for (int column = 0; column < width; ++column) {
            if ((row + column) % 2 != 0) continue;
            for (int direction = 0; direction < 4; ++direction) {
                const int next_row = row + dr[direction];
                const int next_column = column + dc[direction];
                if (next_row < 0 || next_row >= height || next_column < 0 || next_column >= width) {
                    continue;
                }
                edges.push_back({
                    left_id[row * width + column],
                    right_id[next_row * width + next_column],
                    values[row][column] + values[next_row][next_column]
                });
            }
        }
    }
    answer -= minimum_weight_bipartite_matching(left_size, right_size, edges);
    cout << answer << '\n';
}
