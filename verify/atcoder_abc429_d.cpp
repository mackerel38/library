#define PROBLEM "https://atcoder.jp/contests/abc429/tasks/abc429_d"
#include <bits/stdc++.h>
#include "algorithm/twopointer.hpp"

using namespace std;
using namespace poe;

int main() {
    int n, required;
    long long circumference;
    cin >> n >> circumference >> required;
    vector<long long> people(n);
    for (auto& position : people) cin >> position;
    ranges::sort(people);
    vector<long long> positions;
    vector<int> frequency;
    for (const long long position : people) {
        if (positions.empty() || positions.back() != position) {
            positions.push_back(position);
            frequency.push_back(0);
        }
        ++frequency.back();
    }
    const auto windows = circular_threshold_windows(frequency, required);
    long long answer = 0;
    for (int index = 0; index < static_cast<int>(positions.size()); ++index) {
        const long long starts = index == 0
            ? circumference + positions[0] - positions.back()
            : positions[index] - positions[index - 1];
        answer += starts * windows[index].sum;
    }
    cout << answer << '\n';
}
