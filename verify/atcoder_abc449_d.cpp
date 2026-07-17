#include <bits/stdc++.h>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int left, right, down, up;
    cin >> left >> right >> down >> up;
    long long answer = 0;
    for (int x = left; x <= right; ++x) {
        if (x % 2 != 0) continue;
        const int low = max(down, -abs(x) + 1);
        const int high = min(up, abs(x) - 1);
        answer += max(0, high - low + 1);
    }
    for (int y = down; y <= up; ++y) {
        if (y % 2 != 0) continue;
        const int low = max(left, -abs(y));
        const int high = min(right, abs(y));
        answer += max(0, high - low + 1);
    }
    cout << answer << '\n';
}
