#include <bits/stdc++.h>
#include "structure/orderstatistic.hpp"

using namespace std;
using namespace poe;

struct query {
    int type;
    int first;
    int second;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<int> values(n), universe;
    for (int& value : values) cin >> value;
    universe = values;
    vector<query> queries(q);
    for (auto& [type, first, second] : queries) {
        cin >> type >> first >> second;
        if (type == 1) universe.push_back(second);
    }

    orderstatisticmultiset<long long> data(vector<long long>(universe.begin(), universe.end()));
    for (const int value : values) data.add(value);
    for (const auto [type, first, second] : queries) {
        if (type == 1) {
            const int index = first - 1;
            data.erase(values[index]);
            values[index] = second;
            data.add(values[index]);
        } else {
            cout << data.clamp_sum(first, second) << '\n';
        }
    }
}
