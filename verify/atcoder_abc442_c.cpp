#include <bits/stdc++.h>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    vector<long long> available(n, n - 1);
    while (m--) {
        int first, second;
        cin >> first >> second;
        --available[--first];
        --available[--second];
    }
    for (int researcher = 0; researcher < n; ++researcher) {
        const long long count = available[researcher];
        if (researcher) cout << ' ';
        cout << count * (count - 1) * (count - 2) / 6;
    }
    cout << '\n';
}
