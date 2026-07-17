#include <bits/stdc++.h>
#include "graph/functionalgraph.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    unsigned long long k;
    cin >> n >> k;
    vector<int> next(n);
    for (int& vertex : next) {
        cin >> vertex;
        --vertex;
    }
    functionalgraph graph(next);
    cout << graph.jump(0, k) + 1 << '\n';
}
