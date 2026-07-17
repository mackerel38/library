#define PROBLEM "https://atcoder.jp/contests/tdpc/tasks/tdpc_grid"
#include <bits/stdc++.h>
#include "recipe/connectivityfrontier.hpp"
#include "math/modint.hpp"

using namespace std;
using namespace poe;

int main() {
    int height, width;
    cin >> height >> width;
    using mint = modint1000000007;
    connectivityfrontierdp<mint> dp(height, 0);
    for (int column = 1; column < width; ++column) dp.advance_all();
    cout << dp.sum_marked(height - 1).val() << '\n';
}
