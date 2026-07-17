#include <bits/stdc++.h>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, left_distance, right_distance;
    string s;
    cin >> n >> left_distance >> right_distance >> s;
    array<int, 26> count{};
    long long answer = 0;
    for (int right = 0; right < n; ++right) {
        if (right >= left_distance) ++count[s[right - left_distance] - 'a'];
        if (right > right_distance) --count[s[right - right_distance - 1] - 'a'];
        answer += count[s[right] - 'a'];
    }
    cout << answer << '\n';
}
