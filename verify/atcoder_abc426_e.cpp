#define PROBLEM "https://atcoder.jp/contests/abc426/tasks/abc426_e"
#include <bits/stdc++.h>
#include "geometry/point.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cout << fixed << setprecision(15);
    int tests;
    cin >> tests;
    while (tests--) {
        point<long long> first_start, first_goal, second_start, second_goal;
        cin >> first_start.x >> first_start.y >> first_goal.x >> first_goal.y;
        cin >> second_start.x >> second_start.y >> second_goal.x >> second_goal.y;
        cout << minimum_distance_segment_walks(
            first_start, first_goal, second_start, second_goal) << '\n';
    }
}
