#define PROBLEM "https://atcoder.jp/contests/abc410/tasks/abc410_g"
#include <bits/stdc++.h>
#include "dp/sequence.hpp"

using namespace std;
using namespace poe;

int main() {
    int n;
    cin >> n;
    vector<pair<int, int>> intervals;
    intervals.reserve(2 * n);
    for (int i = 0; i < n; ++i) {
        int first, second;
        cin >> first >> second;
        if (first > second) swap(first, second);
        intervals.emplace_back(first, second);
        intervals.emplace_back(second, first + 2 * n);
    }
    cout << longest_nested_intervals(intervals).size() << '\n';
}
