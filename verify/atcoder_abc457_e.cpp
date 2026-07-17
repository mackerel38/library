#include <bits/stdc++.h>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    vector<vector<int>> by_left(n + 2), by_right(n + 2);
    map<pair<int, int>, int> count;
    const int inf = n + 1;
    vector<int> minimum_right(n + 3, inf);
    for (int i = 0; i < m; ++i) {
        int left, right;
        cin >> left >> right;
        by_left[left].push_back(right);
        by_right[right].push_back(left);
        ++count[{left, right}];
        minimum_right[left] = min(minimum_right[left], right);
    }
    for (auto& values : by_left) ranges::sort(values);
    for (auto& values : by_right) ranges::sort(values);
    vector<int> suffix_minimum(n + 3, inf);
    for (int left = n; left >= 1; --left) {
        suffix_minimum[left] = min(suffix_minimum[left + 1], minimum_right[left]);
    }

    int q;
    cin >> q;
    while (q--) {
        int left, right;
        cin >> left >> right;
        bool possible = false;
        if (count[{left, right}] > 0) {
            possible |= count[{left, right}] >= 2;
            possible |= suffix_minimum[left + 1] <= right;
            possible |= suffix_minimum[left] <= right - 1;
        } else {
            const auto right_iterator = upper_bound(by_left[left].begin(), by_left[left].end(), right);
            const auto left_iterator = lower_bound(by_right[right].begin(), by_right[right].end(), left);
            if (right_iterator != by_left[left].begin() && left_iterator != by_right[right].end()) {
                possible = *left_iterator <= *prev(right_iterator) + 1;
            }
        }
        cout << (possible ? "Yes" : "No") << '\n';
    }
}
