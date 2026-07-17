#include <bits/stdc++.h>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int test_cases;
    cin >> test_cases;
    while (test_cases--) {
        int n, expiration;
        cin >> n >> expiration;
        vector<int> purchased(n), used(n);
        for (int& value : purchased) cin >> value;
        for (int& value : used) cin >> value;
        deque<pair<int, int>> stock;
        int total = 0;
        for (int day = 0; day < n; ++day) {
            stock.push_back({day, purchased[day]});
            total += purchased[day];
            int consume = used[day];
            total -= consume;
            while (consume > 0) {
                const int take = min(consume, stock.front().second);
                consume -= take;
                stock.front().second -= take;
                if (stock.front().second == 0) stock.pop_front();
            }
            while (!stock.empty() && stock.front().first <= day - expiration) {
                total -= stock.front().second;
                stock.pop_front();
            }
        }
        cout << total << '\n';
    }
}
