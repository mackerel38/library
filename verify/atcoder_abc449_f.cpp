#include <bits/stdc++.h>

#include "geometry/rectangleunion.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    long long height, width, rectangle_height, rectangle_width;
    int black_count;
    cin >> height >> width >> rectangle_height >> rectangle_width >> black_count;
    const long long row_count = height - rectangle_height + 1;
    const long long column_count = width - rectangle_width + 1;
    vector<axisalignedrectangle<long long>> forbidden;
    for (int i = 0; i < black_count; ++i) {
        long long row, column;
        cin >> row >> column;
        const long long row_left = max(1LL, row - rectangle_height + 1);
        const long long row_right = min(row, row_count);
        const long long column_left = max(1LL, column - rectangle_width + 1);
        const long long column_right = min(column, column_count);
        if (row_left <= row_right && column_left <= column_right)
            forbidden.push_back({row_left, row_right + 1, column_left, column_right + 1});
    }
    cout << row_count * column_count - rectangle_union_area(forbidden) << '\n';
}
