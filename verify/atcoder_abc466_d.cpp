#include <bits/stdc++.h>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    vector<pair<int, int>> operations(m);
    for (auto& [row, column] : operations) cin >> row >> column;
    vector<char> used_row(n + 1), used_column(n + 1);
    int answer = 0;
    for (int i = m - 1; i >= 0; --i) {
        const auto [row, column] = operations[i];
        if (!used_row[row] && !used_column[column]) ++answer;
        used_row[row] = true;
        used_column[column] = true;
    }
    cout << answer << '\n';
}
