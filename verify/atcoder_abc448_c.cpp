#include <bits/stdc++.h>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<pair<int, int>> values(n);
    for (int i = 0; i < n; ++i) {
        cin >> values[i].first;
        values[i].second = i;
    }
    sort(values.begin(), values.end());

    while (q--) {
        int k;
        cin >> k;
        set<int> removed;
        while (k--) {
            int index;
            cin >> index;
            removed.insert(index - 1);
        }
        for (int i = 0; i < 6; ++i) {
            if (!removed.contains(values[i].second)) {
                cout << values[i].first << '\n';
                break;
            }
        }
    }
}
