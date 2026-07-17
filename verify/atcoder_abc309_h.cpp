#define PROBLEM "https://atcoder.jp/contests/abc309/tasks/abc309_h"
#include <bits/stdc++.h>
#include "fps/cyclic.hpp"

using namespace std;
using namespace poe;

int main() {
    long long rows;
    int columns, start_count, goal_count;
    cin >> rows >> columns >> start_count >> goal_count;
    const int cycle = 2 * columns + 2;
    vector<modint998244353> initial(cycle);
    for (int index = 0; index < start_count; ++index) {
        int position;
        cin >> position;
        ++initial[position];
        --initial[cycle - position];
    }
    vector<int> goals(goal_count);
    for (int& position : goals) cin >> position;

    vector<modint998244353> step(cycle);
    step[0] = step[1] = step[cycle - 1] = 1;
    const auto transition = cyclic_power<998244353>(std::move(step), rows - 1);
    const auto result = cyclic_convolution<998244353>(initial, transition);
    modint998244353 answer = 0;
    for (const int position : goals) answer += result[position];
    cout << answer << '\n';
}

