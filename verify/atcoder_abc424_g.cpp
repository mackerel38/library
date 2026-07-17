#define PROBLEM "https://atcoder.jp/contests/abc424/tasks/abc424_g"
#include <bits/stdc++.h>
#include "graph/matching.hpp"

using namespace std;
using namespace poe;

int main() {
    int left_size, right_size;
    cin >> left_size >> right_size;
    vector<int> capacity(left_size), degree(right_size);
    vector<long long> reward(right_size);
    for (int& value : capacity) cin >> value;
    for (int right = 0; right < right_size; ++right) cin >> degree[right] >> reward[right];
    cout << max_bipartite_degree_reward(capacity, degree, reward) << '\n';
}
