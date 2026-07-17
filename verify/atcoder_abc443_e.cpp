#include <bits/stdc++.h>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int test_count;
    cin >> test_count;
    while (test_count--) {
        int n, start;
        cin >> n >> start;
        --start;
        vector<string> grid(n);
        vector<int> lowest_wall(n, -1);
        for (int row = 0; row < n; ++row) {
            cin >> grid[row];
            for (int column = 0; column < n; ++column) {
                if (grid[row][column] == '#') lowest_wall[column] = row;
            }
        }
        vector<vector<bool>> reachable(n, vector<bool>(n));
        for (int row = 0; row < n; ++row) reachable[row][start] = true;
        for (int row = n - 2; row >= 0; --row) {
            for (int column = 0; column < n; ++column) {
                if (reachable[row][column]) continue;
                bool enter = reachable[row + 1][column];
                if (column > 0) enter = enter || reachable[row + 1][column - 1];
                if (column + 1 < n) enter = enter || reachable[row + 1][column + 1];
                if (!enter) continue;
                if (grid[row][column] == '.') {
                    reachable[row][column] = true;
                } else if (lowest_wall[column] == row) {
                    for (int upper = 0; upper <= row; ++upper) {
                        reachable[upper][column] = true;
                    }
                }
            }
        }
        for (bool possible : reachable[0]) cout << possible;
        cout << '\n';
    }
}
