#include <bits/stdc++.h>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;
    long long answer = 0;
    for (int i = 0; i < static_cast<int>(s.size()); ++i) {
        if (s[i] == 'C') answer += min(i + 1, static_cast<int>(s.size()) - i);
    }
    cout << answer << '\n';
}
