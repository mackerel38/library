#include <bits/stdc++.h>
#include "dp/intervalinsertion.hpp"
#include "math/modint.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    string directions;
    cin >> n >> directions;
    cout << circular_directional_removal_expectation<modint998244353>(directions) << '\n';
}
