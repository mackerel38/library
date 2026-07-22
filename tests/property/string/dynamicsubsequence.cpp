#include <bits/stdc++.h>
#include "math/modint.hpp"
#include "string/dynamicsubsequence.hpp"

using namespace std;
using namespace poe;

long long brute(const vector<uint64_t>& masks, int alphabet) {
    set<vector<int>> subsequences;
    vector<int> current;
    const auto dfs = [&](auto&& self, int index) -> void {
        if (index == static_cast<int>(masks.size())) {
            if (!current.empty()) subsequences.insert(current);
            return;
        }
        self(self, index + 1);
        for (int symbol = 0; symbol < alphabet; ++symbol) {
            if (!((masks[index] >> symbol) & 1U)) continue;
            current.push_back(symbol);
            self(self, index + 1);
            current.pop_back();
        }
    };
    dfs(dfs, 0);
    return static_cast<long long>(subsequences.size());
}

int main() {
    mt19937 random(246);
    for (int trial = 0; trial < 3000; ++trial) {
        const int n = random() % 9;
        vector<uint64_t> masks(n);
        for (auto& mask : masks) mask = random() % 8;
        dynamicsubsequencecount<modint998244353, 3> sequence(masks);
        assert(sequence.count().val() == brute(masks, 3));
        for (int query = 0; query < 10 && n > 0; ++query) {
            const int index = random() % n;
            masks[index] = random() % 8;
            sequence.set(index, masks[index]);
            assert(sequence.count().val() == brute(masks, 3));
        }
    }
}
