#include <bits/stdc++.h>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    long long end;
    cin >> n >> end;
    long long open = 0, answer = 0;
    for (int i = 0; i < n; ++i) {
        long long time;
        cin >> time;
        if (open < time) {
            answer += time - open;
            open = time + 100;
        }
    }
    if (open < end) answer += end - open;
    cout << answer << '\n';
}
