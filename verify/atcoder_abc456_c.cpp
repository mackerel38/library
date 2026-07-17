#include <bits/stdc++.h>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    constexpr long long mod = 998244353;
    string s;
    cin >> s;
    long long answer = 0;
    long long length = 0;
    for (int i = 0; i < static_cast<int>(s.size()); ++i) {
        length = (i == 0 || s[i - 1] == s[i]) ? 1 : length + 1;
        answer += length;
    }
    cout << answer % mod << '\n';
}
