#define PROBLEM "https://atcoder.jp/contests/abc416/tasks/abc416_g"
#include <bits/stdc++.h>
#include "string/concatenation.hpp"

using namespace std;
using namespace poe;

int main() {
    int n, k;
    cin >> n >> k;
    vector<string> strings(n);
    for (auto& value : strings) cin >> value;
    cout << minimum_repeated_concatenation(strings, k) << '\n';
}
