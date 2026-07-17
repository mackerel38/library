#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    vector<int> a(n);
    for (int& value : a) cin >> value;
    int reach = 1;
    for (int index = 0; index < n && index < reach; ++index) {
        reach = max(reach, index + a[index]);
    }
    cout << min(n, reach) << '\n';
}
