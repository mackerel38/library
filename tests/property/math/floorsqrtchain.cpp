#include <bits/stdc++.h>
#include "math/floorsqrtchain.hpp"

using namespace std;
using namespace poe;

int main() {
    constexpr int limit = 200000;
    floorsqrtchaincounter counter(limit);
    vector<long long> expected(limit + 1), prefix(limit + 1);
    expected[1] = 1;
    for (int x = 1; x <= limit; ++x) {
        if (x >= 2) expected[x] = prefix[floor_sqrt(x)];
        prefix[x] = prefix[x - 1] + expected[x];
        assert(counter.count(x) == expected[x]);
    }
}
