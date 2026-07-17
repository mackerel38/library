#define PROBLEM "https://atcoder.jp/contests/abc422/tasks/abc422_g"
#include <bits/stdc++.h>
#include "fps/combinatorial.hpp"

using namespace std;
using namespace poe;

int main() {
    int n, first, second, third;
    cin >> n >> first >> second >> third;
    const auto answer = divisible_group_counts<998244353>(n, {first, second, third});
    cout << answer.ordinary << '\n' << answer.labeled << '\n';
}
