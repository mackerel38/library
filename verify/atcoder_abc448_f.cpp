#include <bits/stdc++.h>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<int> x(n), y(n);
    for (int i = 0; i < n; ++i) cin >> x[i] >> y[i];

    const int width = static_cast<int>(2e7 / sqrt(n));
    vector<int> order(n);
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int first, int second) {
        const int first_strip = x[first] / width;
        const int second_strip = x[second] / width;
        if (first_strip != second_strip) return first_strip < second_strip;
        if (first_strip % 2 == 0) return y[first] < y[second];
        return y[first] > y[second];
    });
    rotate(order.begin(), find(order.begin(), order.end(), 0), order.end());
    for (int i = 0; i < n; ++i) cout << order[i] + 1 << " \n"[i + 1 == n];
}
