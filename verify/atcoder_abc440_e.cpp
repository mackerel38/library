#include <bits/stdc++.h>
#include "algorithm/topenumeration.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, count;
    long long picks;
    cin >> n >> picks >> count;
    vector<long long> values(n);
    for (auto& value : values) cin >> value;
    for (const long long sum : top_multiset_sums(values, picks, count)) {
        cout << sum << '\n';
    }
}
