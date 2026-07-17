#include <bits/stdc++.h>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    map<long long, array<vector<int>, 3>> positions;
    for (int index = 0; index < n; ++index) {
        long long value;
        cin >> value;
        if (value % 7 == 0) positions[value / 7][0].push_back(index);
        if (value % 5 == 0) positions[value / 5][1].push_back(index);
        if (value % 3 == 0) positions[value / 3][2].push_back(index);
    }
    long long answer = 0;
    for (const auto& entry : positions) {
        const auto& group = entry.second;
        const auto& seven = group[0];
        const auto& five = group[1];
        const auto& three = group[2];
        long long count = 1LL * seven.size() * five.size() * three.size();
        for (const int middle : five) {
            const long long seven_before = lower_bound(seven.begin(), seven.end(), middle) - seven.begin();
            const long long three_before = lower_bound(three.begin(), three.end(), middle) - three.begin();
            count -= seven_before * (static_cast<long long>(three.size()) - three_before);
            count -= three_before * (static_cast<long long>(seven.size()) - seven_before);
        }
        answer += count;
    }
    cout << answer << '\n';
}
