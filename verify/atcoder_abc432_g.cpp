#include <bits/stdc++.h>
#include "fps/transform.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    vector<int> first(n), second(m);
    int maximum = 0;
    for (int& value : first) {
        cin >> value;
        maximum = max(maximum, value);
    }
    for (int& value : second) {
        cin >> value;
        maximum = max(maximum, value);
    }
    fps998244353 frequency(maximum + 1);
    for (const int value : second) ++frequency[value];
    const auto transformed = binomial_transform<998244353>(frequency, modint998244353{1});
    modint998244353 answer;
    for (const int value : first) answer += transformed[value];
    cout << answer << '\n';
}
