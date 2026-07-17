#define PROBLEM "https://atcoder.jp/contests/fps-24/tasks/fps_24_w"
#include <bits/stdc++.h>
#include "fps/setfps.hpp"

using namespace std;
using namespace poe;

int main() {
    int vertices, edges;
    cin >> vertices >> edges;
    vector<int> adjacent(vertices);
    for (int i = 0; i < edges; ++i) {
        int left, right;
        cin >> left >> right;
        --left;
        --right;
        adjacent[left] |= 1 << right;
        adjacent[right] |= 1 << left;
    }
    const int size = 1 << vertices;
    vector<modint998244353> all(size);
    all[0] = 1;
    for (int mask = 1; mask < size; ++mask) {
        const int vertex = countr_zero(static_cast<unsigned int>(mask));
        const int rest = mask ^ (1 << vertex);
        all[mask] = all[rest] * modint998244353{2}.pow(popcount(static_cast<unsigned int>(adjacent[vertex] & rest)));
    }

    auto biconnected = setfps_log<998244353>(all);
    for (int vertex = 0; vertex < vertices; ++vertex) {
        setfps_articulation_transform<998244353>(biconnected, vertex, false);
    }
    const int terminals = 1 | (1 << (vertices - 1));
    for (int mask = 0; mask < size; ++mask) {
        if ((mask & terminals) == terminals && popcount(static_cast<unsigned int>(mask)) >= 3) {
            biconnected[mask] = 0;
        }
    }
    for (int vertex = vertices - 1; vertex >= 0; --vertex) {
        setfps_articulation_transform<998244353>(biconnected, vertex, true);
    }
    const auto without_cycle = setfps_exp<998244353>(biconnected);
    cout << all.back() - without_cycle.back() << '\n';
}
