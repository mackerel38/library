#include <bits/stdc++.h>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, sake_count;
    long long target;
    cin >> n >> sake_count >> target;
    vector<long long> amount(n);
    for (auto& value : amount) cin >> value;
    ranges::sort(amount, greater{});
    long long guaranteed = 0;
    for (int selected = n - sake_count + 1; selected <= n; ++selected) {
        guaranteed += amount[selected - 1];
        if (guaranteed >= target) {
            cout << selected << '\n';
            return 0;
        }
    }
    cout << -1 << '\n';
}
