#define PROBLEM "https://atcoder.jp/contests/fps-24/tasks/fps_24_j"
#include <bits/stdc++.h>
#include "fps/online.hpp"

using namespace std;
using namespace poe;

int main() {
    int goal, faces, trap_count;
    cin >> goal >> faces >> trap_count;
    vector<modint998244353> source(goal), kernel(goal);
    source[0] = 1;
    const modint998244353 probability = modint998244353{1} / faces;
    for (int i = 0, value; i < faces; ++i) {
        cin >> value;
        if (value < goal) kernel[value] += probability;
    }
    vector<bool> trap(goal);
    for (int i = 0, position; i < trap_count; ++i) {
        cin >> position;
        trap[position] = true;
    }
    modint998244353 lose{};
    online_convolution<998244353>(source, kernel, [&](int position, modint998244353 value) {
        if (trap[position]) {
            lose += value;
            return modint998244353{};
        }
        return value;
    });
    cout << 1 - lose << '\n';
}
