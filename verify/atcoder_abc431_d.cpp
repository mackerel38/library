#include <bits/stdc++.h>
#include "dp/knapsack.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<int> weight(n);
    vector<long long> gain(n);
    long long total_weight = 0;
    long long body_happiness = 0;
    for (int index = 0; index < n; ++index) {
        long long head, body;
        cin >> weight[index] >> head >> body;
        total_weight += weight[index];
        body_happiness += body;
        gain[index] = head - body;
    }
    cout << body_happiness
            + knapsack01(weight, gain, static_cast<int>(total_weight / 2))
         << '\n';
}
