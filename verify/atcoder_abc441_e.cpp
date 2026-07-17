#include <bits/stdc++.h>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    string s;
    cin >> n >> s;
    vector<long long> count(2 * n + 1);
    const int offset = n;
    int prefix = 0;
    long long less = 0, answer = 0;
    count[offset] = 1;
    for (const char character : s) {
        const int delta = character == 'A' ? 1 : character == 'B' ? -1 : 0;
        if (delta == 1) less += count[offset + prefix];
        if (delta == -1) less -= count[offset + prefix - 1];
        prefix += delta;
        answer += less;
        ++count[offset + prefix];
    }
    cout << answer << '\n';
}
