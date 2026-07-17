#include <bits/stdc++.h>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<int> previous(n + 1), current(n + 1), suffix(n + 1);
    string row;
    cin >> row;
    previous[0] = ranges::count(row, '.');
    for (int white = 1; white <= n; ++white) {
        previous[white] = previous[white - 1] + (row[white - 1] == '#' ? 1 : -1);
    }
    for (int index = 1; index < n; ++index) {
        cin >> row;
        suffix[n] = previous[n];
        for (int white = n - 1; white >= 0; --white) {
            suffix[white] = min(previous[white], suffix[white + 1]);
        }
        int repaint = ranges::count(row, '.');
        current[0] = repaint + suffix[0];
        for (int white = 1; white <= n; ++white) {
            repaint += row[white - 1] == '#' ? 1 : -1;
            current[white] = repaint + suffix[white];
        }
        previous.swap(current);
    }
    cout << *ranges::min_element(previous) << '\n';
}
