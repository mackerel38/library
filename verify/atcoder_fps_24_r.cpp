#define PROBLEM "https://atcoder.jp/contests/fps-24/tasks/fps_24_r"
#include <bits/stdc++.h>
#include "fps/cyclic.hpp"

using namespace std;
using namespace poe;

int main() {
    int exponent, vertex;
    long long steps;
    cin >> exponent >> steps >> vertex;
    const int length = 1 << exponent;
    vector<modint998244353> transition(length << 1);
    transition[1] = transition.back() = modint998244353{1} / 2;
    const auto probability = cyclic_power<998244353>(std::move(transition), steps);
    const int position = vertex - 1;
    modint998244353 answer = probability[position];
    if (position != 0 && position != length) answer += probability[2 * length - position];
    cout << answer << '\n';
}
