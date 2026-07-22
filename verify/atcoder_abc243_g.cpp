#include <bits/stdc++.h>
#include "math/floorsqrtchain.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    vector<unsigned long long> x(t);
    for (auto& value : x) cin >> value;
    floorsqrtchaincounter counter(*max_element(x.begin(), x.end()));
    for (const auto value : x) cout << counter(value) << '\n';
}
