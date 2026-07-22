#include <bits/stdc++.h>
#include "math/rangeproductpower.hpp"

using namespace std;
using namespace poe;

int main() {
    mt19937 random(238);
    for (int trial = 0; trial < 3000; ++trial) {
        const int n = 1 + random() % 12;
        const int exponent = 2 + random() % 6;
        vector<int> values(n);
        for (int& value : values) value = 1 + random() % 100;
        vector<pair<int, int>> queries;
        for (int left = 0; left < n; ++left) {
            for (int right = left + 1; right <= n; ++right) queries.push_back({left, right});
        }
        const auto actual = range_product_power_queries(values, queries, exponent);
        primetable primes(100);
        for (int id = 0; id < static_cast<int>(queries.size()); ++id) {
            const auto [left, right] = queries[id];
            map<int, int> counts;
            for (int index = left; index < right; ++index) {
                for (const auto [prime, count] : primes.factorize(values[index])) {
                    counts[prime] += count;
                }
            }
            const bool expected = ranges::all_of(counts, [&](const auto& item) {
                return item.second % exponent == 0;
            });
            assert(actual[id] == expected);
        }
    }
}
