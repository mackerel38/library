#include <bits/stdc++.h>
#include "graph/matching.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<pair<int, int>> red(n), blue(n);
    for (auto& [x, y] : red) cin >> x >> y;
    for (auto& [x, y] : blue) cin >> x >> y;

    bipartitematching graph(n, n);
    for (int left = 0; left < n; ++left) {
        for (int right = 0; right < n; ++right) {
            if (red[left].first < blue[right].first &&
                red[left].second < blue[right].second) {
                graph.add_edge(left, right);
            }
        }
    }
    cout << graph.solve().size() << '\n';
}
