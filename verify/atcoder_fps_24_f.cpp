#define PROBLEM "https://atcoder.jp/contests/fps-24/tasks/fps_24_f"
#include <bits/stdc++.h>
#include "math/modint.hpp"

using namespace std;
using namespace poe;

int main() {
    long long n;
    cin >> n;
    modint998244353 answer = modint998244353{3}.pow(n) - (n % 2 == 0 ? 1 : -1);
    cout << answer / 4 << '\n';
}
