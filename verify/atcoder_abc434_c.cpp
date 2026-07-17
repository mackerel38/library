#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int test_count;
    cin >> test_count;
    while (test_count--) {
        int n;
        long long height;
        cin >> n >> height;
        long long previous_time = 0, lower = height, upper = height;
        bool possible = true;
        for (int i = 0; i < n; ++i) {
            long long time, goal_lower, goal_upper;
            cin >> time >> goal_lower >> goal_upper;
            const long long elapsed = time - previous_time;
            lower = max(goal_lower, lower - elapsed);
            upper = min(goal_upper, upper + elapsed);
            possible &= lower <= upper;
            previous_time = time;
        }
        cout << (possible ? "Yes" : "No") << '\n';
    }
}
