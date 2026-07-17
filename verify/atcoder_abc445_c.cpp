#include <bits/stdc++.h>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<int> next(n), answer(n);
    for (int& vertex : next) cin >> vertex, --vertex;
    for (int vertex = n - 1; vertex >= 0; --vertex) {
        answer[vertex] = next[vertex] == vertex ? vertex : answer[next[vertex]];
    }
    for (int vertex = 0; vertex < n; ++vertex) {
        if (vertex) cout << ' ';
        cout << answer[vertex] + 1;
    }
    cout << '\n';
}
