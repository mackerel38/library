#define PROBLEM "https://atcoder.jp/contests/dp/tasks/dp_z"
#include <bits/stdc++.h>
#include "structure/linecontainer.hpp"

using namespace std;
using namespace poe;

int main() {
    int count;
    long long constant;
    cin >> count >> constant;
    vector<long long> height(count);
    for (auto& value : height) cin >> value;
    vector<long long> dp(count);
    minlinecontainer<long long> lines(height);
    lines.add_line({-2 * height[0], height[0] * height[0]});
    for (int i = 1; i < count; ++i) {
        dp[i] = height[i] * height[i] + constant + lines.get(height[i]);
        lines.add_line({-2 * height[i], dp[i] + height[i] * height[i]});
    }
    cout << dp.back() << '\n';
}
