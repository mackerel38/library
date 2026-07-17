#include <bits/stdc++.h>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    long long distance;
    cin >> n >> distance;
    vector<long long> values(n);
    for (long long& value : values) cin >> value;

    long long answer = 0;
    set<long long> active;
    int right = 0;
    for (int left = 0; left < n; ++left) {
        while (right < n) {
            const auto next = active.lower_bound(values[right]);
            if (next != active.end() && *next - values[right] < distance) break;
            if (next != active.begin() && values[right] - *prev(next) < distance) break;
            active.insert(values[right++]);
        }
        answer += right - left;
        active.erase(values[left]);
    }
    cout << answer << '\n';
}
