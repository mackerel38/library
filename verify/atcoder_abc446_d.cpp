#include <bits/stdc++.h>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    unordered_map<long long, int> best;
    int answer = 0;
    while (n--) {
        long long value;
        cin >> value;
        best[value] = max(best[value], best[value - 1] + 1);
        answer = max(answer, best[value]);
    }
    cout << answer << '\n';
}
