#include <bits/stdc++.h>
#include "string/rollinghash.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    string text;
    cin >> n >> text;
    rollinghash<char> hash(text);
    int answer = 0;
    for (int first = 0; first < n; ++first) {
        for (int second = first + 1; second < n; ++second) {
            answer = max(answer, min(second - first,
                                     hash.lcp(first, n, hash, second, n)));
        }
    }
    cout << answer << '\n';
}
