#include <bits/stdc++.h>
#include "string/runlength.hpp"
using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string source;
    cin >> source;
    const auto runs = runlength(source);
    long long answer = 0;
    for (int i = 1; i < static_cast<int>(runs.size()); ++i) {
        if (runs[i - 1].first + 1 == runs[i].first) {
            answer += min(runs[i - 1].second, runs[i].second);
        }
    }
    cout << answer << '\n';
}
