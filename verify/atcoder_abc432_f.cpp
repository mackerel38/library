#include <bits/stdc++.h>
#include "dp/subset.hpp"

using namespace std;
using namespace poe;

struct operation {
    int from;
    int to;
    long long count;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<long long> candies(n);
    for (long long& value : candies) cin >> value;
    const long long total = reduce(candies.begin(), candies.end(), 0LL);
    if (total % n != 0) {
        cout << -1 << '\n';
        return 0;
    }
    const long long average = total / n;
    vector<long long> deviation(n);
    for (int index = 0; index < n; ++index) deviation[index] = candies[index] - average;
    const auto partition = max_zero_sum_partition(deviation);

    vector<operation> answer;
    for (const uint64_t group : partition.groups) {
        vector<int> indices;
        for (int index = 0; index < n; ++index) if (group >> index & 1) indices.push_back(index);
        ranges::sort(indices, [&](int left, int right) { return candies[left] > candies[right]; });
        for (int position = 0; position + 1 < static_cast<int>(indices.size()); ++position) {
            const int from = indices[position];
            const int to = indices[position + 1];
            const long long count = candies[from] - average;
            assert(count > 0);
            answer.push_back({from, to, count});
            candies[from] -= count;
            candies[to] += count;
        }
    }
    assert(answer.size() == static_cast<size_t>(n - partition.groups.size()));
    assert(ranges::all_of(candies, [&](long long value) { return value == average; }));
    cout << answer.size() << '\n';
    for (const auto [from, to, count] : answer) {
        cout << from + 1 << ' ' << to + 1 << ' ' << count << '\n';
    }
}
