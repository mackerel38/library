#define PROBLEM "https://atcoder.jp/contests/abc426/tasks/abc426_d"
#include <bits/stdc++.h>
#include "string/runlength.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int tests;
    cin >> tests;
    while (tests--) {
        int n;
        string source;
        cin >> n >> source;
        array<int, 2> count{}, longest{};
        for (const auto& [character, length] : runlength(source)) {
            const int value = character - '0';
            count[value] += length;
            longest[value] = max(longest[value], length);
        }
        int answer = numeric_limits<int>::max();
        for (int value = 0; value < 2; ++value) {
            answer = min(answer, 2 * (count[value] - longest[value])
                                      + (n - count[value]));
        }
        cout << answer << '\n';
    }
}
