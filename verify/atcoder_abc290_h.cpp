#include <bits/stdc++.h>
#include "dp/groupassignment.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    vector<long long> dog(n), cat(m);
    for (auto& value : dog) cin >> value;
    for (auto& value : cat) cin >> value;
    ranges::sort(dog);
    ranges::sort(cat);

    long long answer = 0;
    if (n % 2 == 1) {
        if (m % 2 == 1) answer += dog.back();
        answer += reduce(cat.begin(), cat.end(), 0LL);
        dog.pop_back();
        --n;
    }
    if (m % 2 == 1) {
        answer += reduce(dog.begin(), dog.end(), 0LL);
        cat.pop_back();
        --m;
    }

    vector<pair<long long, int>> items;
    for (long long value : dog) items.emplace_back(value, 0);
    for (long long value : cat) items.emplace_back(value, 1);
    ranges::sort(items);
    vector<int> category;
    category.reserve(items.size());
    for (const auto& [value, kind] : items) category.push_back(kind);

    answer += minimum_two_group_assignment<long long>(
        category,
        array{n / 2, m / 2},
        [&](int index, int group, const auto& first, const auto& second) {
            const int kind = items[index].second;
            const int opposite_total = kind == 0 ? m : n;
            const auto& count = group == 0 ? first : second;
            return items[index].first * (opposite_total - 2LL * count[1 - kind]);
        },
        1LL << 62
    );
    cout << answer << '\n';
}
