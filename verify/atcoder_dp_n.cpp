#define PROBLEM "https://atcoder.jp/contests/dp/tasks/dp_n"
#include <bits/stdc++.h>
#include "dp/optimization.hpp"

using namespace std;
using namespace poe;

int main() {
    int count;
    cin >> count;
    vector<long long> size(count);
    for (auto& value : size) cin >> value;
    cout << optimal_merge_cost(size) << '\n';
}
