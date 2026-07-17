#include <bits/stdc++.h>
#include "math/modint.hpp"
#include "structure/affinemap.hpp"
using namespace std;
using namespace poe;

using mint = staticmodint<998244353>;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    vector<vector<int>> enter(n + 3), leave(n + 3);
    for (int color = 0; color < m; ++color) {
        int left, right;
        cin >> left >> right;
        for (int parity = 0; parity < 2; ++parity) {
            int first = left + 1;
            if (first % 2 != parity) ++first;
            int last = right;
            if (last % 2 != parity) --last;
            if (first <= last) {
                enter[first].push_back(color);
                leave[last + 2].push_back(color);
            }
        }
    }

    vector<mint> prefix(n + 1, 1);
    array<affinemap<int, mint>, 2> states;
    for (int index = 2; index <= n; ++index) {
        auto& state = states[index % 2];
        state.apply(-1, index >= 4 ? prefix[index - 4] : mint(0));
        for (int color : leave[index]) state.erase(color);
        for (int color : enter[index]) state.set(color, 0);
        prefix[index] = prefix[index - 1]
            + mint(state.size()) * prefix[index - 2] - state.sum();
    }
    cout << prefix[n] << '\n';
}
