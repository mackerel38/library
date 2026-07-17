#include <bits/stdc++.h>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, query_count;
    cin >> n >> query_count;
    vector<long long> values(n), sum(n + 1);
    for (int i = 0; i < n; ++i) cin >> values[i], sum[i + 1] = sum[i] + values[i];
    while (query_count--) {
        int type;
        cin >> type;
        if (type == 1) {
            int index;
            cin >> index;
            --index;
            swap(values[index], values[index + 1]);
            sum[index + 1] = sum[index] + values[index];
        } else {
            int left, right;
            cin >> left >> right;
            cout << sum[right] - sum[left - 1] << '\n';
        }
    }
}
