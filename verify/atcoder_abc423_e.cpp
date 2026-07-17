#define PROBLEM "https://atcoder.jp/contests/abc423/tasks/abc423_e"
#include <bits/stdc++.h>
#include "structure/prefixsum.hpp"

using namespace std;
using namespace poe;

int main() {
    int n, queries;
    cin >> n >> queries;
    vector<long long> values(n);
    for (auto& value : values) cin >> value;
    subarrayprefixsum<long long> sums(values);
    while (queries--) {
        int left, right;
        cin >> left >> right;
        cout << sums.sum(left - 1, right) << '\n';
    }
}
