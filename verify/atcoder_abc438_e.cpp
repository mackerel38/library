#include <bits/stdc++.h>
#include "structure/doubling.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    cin >> n >> q;
    vector<int> next(n);
    for (int& vertex : next) {
        cin >> vertex;
        --vertex;
    }
    vector<long long> weight(n);
    iota(weight.begin(), weight.end(), 1LL);
    weighteddoubling table(next, weight, 1'000'000'000ULL);
    while (q--) {
        unsigned long long step;
        int bucket;
        cin >> step >> bucket;
        cout << table.jump(bucket - 1, step).aggregate << '\n';
    }
}
