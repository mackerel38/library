#define PROBLEM "https://atcoder.jp/contests/abc312/tasks/abc312_h"
#include <bits/stdc++.h>
#include "string/periodic.hpp"

using namespace std;
using namespace poe;

int main() {
    int n;
    cin >> n;
    vector<string> strings(n);
    for (string& text : strings) cin >> text;
    const auto answer = minimum_unique_repetition_counts(strings);
    for (int index = 0; index < n; ++index) {
        if (index > 0) cout << ' ';
        cout << answer[index];
    }
    cout << '\n';
}
