#include <bits/stdc++.h>

#include "structure/fenwicktree.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    vector<int> initial(n), frequency(m);
    for (int& value : initial) {
        cin >> value;
        --value;
        ++frequency[value];
    }
    vector<pair<int, int>> order(m);
    for (int value = 0; value < m; ++value) order[value] = {frequency[value], value};
    ranges::sort(order);
    vector<unsigned long long> boundary(m + 1);
    boundary[0] = n;
    for (int size = 1; size < m; ++size) {
        const unsigned long long add = 1ULL * size * (order[size].first - order[size - 1].first);
        boundary[size] = min(1000000000000000001ULL, boundary[size - 1] + add);
    }
    boundary[m] = 1000000000000000001ULL;

    int q;
    cin >> q;
    vector<int> answer(q, -1);
    vector<tuple<int, int, int>> offline;
    for (int query = 0; query < q; ++query) {
        unsigned long long position;
        cin >> position;
        if (position <= static_cast<unsigned long long>(n)) {
            answer[query] = initial[position - 1];
            continue;
        }
        const int size = lower_bound(boundary.begin() + 1, boundary.end(), position) - boundary.begin();
        const int rank = static_cast<int>((position - boundary[size - 1] - 1) % size);
        offline.emplace_back(size, rank, query);
    }
    ranges::sort(offline);
    fenwicktree<int> active(m);
    int inserted = 0;
    for (auto [size, rank, query] : offline) {
        while (inserted < size) active.add(order[inserted++].second, 1);
        answer[query] = active.lower_bound(rank + 1);
    }
    for (int value : answer) cout << value + 1 << '\n';
}
