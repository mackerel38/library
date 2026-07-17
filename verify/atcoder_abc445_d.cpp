#include <bits/stdc++.h>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long height, width;
    int n;
    cin >> height >> width >> n;
    vector<long long> piece_height(n), piece_width(n);
    vector<int> by_height(n), by_width(n);
    iota(by_height.begin(), by_height.end(), 0);
    iota(by_width.begin(), by_width.end(), 0);
    for (int i = 0; i < n; ++i) cin >> piece_height[i] >> piece_width[i];
    ranges::sort(by_height, greater{}, [&](int i) { return piece_height[i]; });
    ranges::sort(by_width, greater{}, [&](int i) { return piece_width[i]; });

    vector<bool> used(n);
    vector<pair<long long, long long>> answer(n);
    int height_position = 0, width_position = 0;
    for (int count = 0; count < n; ++count) {
        while (height_position < n && used[by_height[height_position]]) ++height_position;
        while (width_position < n && used[by_width[width_position]]) ++width_position;
        if (height_position < n && piece_height[by_height[height_position]] == height) {
            const int piece = by_height[height_position];
            answer[piece] = {1, width - piece_width[piece] + 1};
            width -= piece_width[piece];
            used[piece] = true;
        } else {
            const int piece = by_width[width_position];
            answer[piece] = {height - piece_height[piece] + 1, 1};
            height -= piece_height[piece];
            used[piece] = true;
        }
    }
    for (const auto [row, column] : answer) cout << row << ' ' << column << '\n';
}
