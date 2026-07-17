#define PROBLEM "https://atcoder.jp/contests/tdpc/tasks/tdpc_contest"
#include <bits/stdc++.h>
#include "dp/subsetsum.hpp"

using namespace std;
using namespace poe;

int main() {
    int count;
    cin >> count;
    vector<int> score(count);
    for (auto& value : score) cin >> value;
    const auto possible = subsetsum(score, reduce(score.begin(), score.end()));
    cout << ranges::count(possible, true) << '\n';
}
