#define PROBLEM "https://atcoder.jp/contests/abc427/tasks/abc427_f"
#include <bits/stdc++.h>
#include "dp/subsetsum.hpp"

using namespace std;
using namespace poe;

int main() {
    int n;
    long long modulo;
    cin >> n >> modulo;
    vector<long long> values(n);
    for (auto& value : values) cin >> value;
    cout << count_nonadjacent_subsets_mod(values, modulo) << '\n';
}
