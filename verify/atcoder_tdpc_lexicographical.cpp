#define PROBLEM "https://atcoder.jp/contests/tdpc/tasks/tdpc_lexicographical"
#include <bits/stdc++.h>
#include "string/subsequence.hpp"

using namespace std;
using namespace poe;

int main() {
    string text;
    unsigned long long k;
    cin >> text >> k;
    const auto answer = kth_distinct_subsequence(text, k);
    cout << (answer ? *answer : "Eel") << '\n';
}
