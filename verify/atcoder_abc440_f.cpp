#include <bits/stdc++.h>
#include "structure/orderstatistic.hpp"

using namespace std;
using namespace poe;

struct query {
    int index;
    long long mood;
    int tidiness;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<long long> mood(n), universe;
    vector<int> tidiness(n);
    universe.reserve(n + q);
    for (int i = 0; i < n; ++i) {
        cin >> mood[i] >> tidiness[i];
        universe.push_back(mood[i]);
    }
    vector<query> queries(q);
    for (auto& [index, value, type] : queries) {
        cin >> index >> value >> type;
        --index;
        universe.push_back(value);
    }

    orderstatisticmultiset<long long> all(universe), untidy(universe);
    long long total = 0, tidy_count = 0;
    for (int i = 0; i < n; ++i) {
        all.add(mood[i]);
        if (tidiness[i] == 1) ++tidy_count;
        else untidy.add(mood[i]);
        total += mood[i];
    }

    for (const auto [index, value, type] : queries) {
        all.erase(mood[index]);
        if (tidiness[index] == 1) --tidy_count;
        else untidy.erase(mood[index]);
        total += value - mood[index];
        mood[index] = value;
        tidiness[index] = type;
        all.add(value);
        if (type == 1) ++tidy_count;
        else untidy.add(value);

        if (tidy_count == 0) {
            cout << 2 * total - all.kth(0) << '\n';
        } else if (tidy_count == n) {
            cout << total << '\n';
        } else {
            long long penalty = all.sum_smallest(tidy_count);
            const long long boundary = all.kth(tidy_count - 1);
            if (untidy.kth(0) > boundary) penalty += untidy.kth(0) - boundary;
            cout << 2 * total - penalty << '\n';
        }
    }
}
