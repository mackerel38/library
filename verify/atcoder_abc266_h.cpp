#include <bits/stdc++.h>
#include "algorithm/dominance.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<weightedpoint3d<long long, long long>> points;
    points.reserve(n);
    for (int i = 0; i < n; ++i) {
        long long time, x, y, weight;
        cin >> time >> x >> y >> weight;
        const long long first = time - x - y;
        const long long second = time + x - y;
        if (first >= 0 && second >= 0) {
            points.push_back({first, second, y, weight});
        }
    }
    cout << maximum_weight_dominance_chain_3d(points).weight << '\n';
}
