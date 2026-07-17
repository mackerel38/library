#define PROBLEM "https://atcoder.jp/contests/abc461/tasks/abc461_d"
#include <bits/stdc++.h>
#include "algorithm/twopointer.hpp"

using namespace std;
using namespace poe;

int main() {
    int height, width, target;
    cin >> height >> width >> target;
    vector matrix(height, vector<int>(width));
    for (auto& row : matrix) {
        string line;
        cin >> line;
        for (int column = 0; column < width; ++column) row[column] = line[column] - '0';
    }
    cout << count_subrectangles_sum_nonnegative(matrix, target) << '\n';
}
