#define PROBLEM "https://atcoder.jp/contests/abc415/tasks/abc415_g"
#include <bits/stdc++.h>
#include "math/linearecurrence.hpp"

using namespace std;
using namespace poe;

int main() {
    long long initial_bottles;
    int exchanges;
    cin >> initial_bottles >> exchanges;
    vector<pair<int, int>> offer(exchanges);
    int order = 1;
    for (auto& [required, returned] : offer) {
        cin >> required >> returned;
        order = max(order, required);
    }

    vector<long long> initial(order);
    for (int bottles = 1; bottles < order; ++bottles) {
        initial[bottles] = initial[bottles - 1];
        for (const auto [required, returned] : offer) {
            if (required <= bottles) {
                initial[bottles] = max(
                    initial[bottles],
                    initial[bottles - required + returned] + returned);
            }
        }
    }

    const long long negative_infinity = numeric_limits<long long>::lowest();
    vector<long long> coefficient(order, negative_infinity);
    coefficient[0] = 0;
    for (const auto [required, returned] : offer) {
        coefficient[required - returned - 1] =
            max(coefficient[required - returned - 1], static_cast<long long>(returned));
    }
    cout << initial_bottles +
        maxplus_recurrence(initial, coefficient, initial_bottles) << '\n';
}
