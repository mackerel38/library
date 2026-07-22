#include <bits/stdc++.h>
#include "graph/poset.hpp"

using namespace std;
using namespace poe;

int main() {
    const vector<int> values = {1, 2, 3, 6};
    const auto result = maximum_poset_antichain(4, [&](int left, int right) {
        return values[left] != values[right] && values[right] % values[left] == 0;
    });
    assert(result.size() == 2);
    assert(result.chains.size() == 2);
    for (int i = 0; i < result.size(); ++i) {
        for (int j = i + 1; j < result.size(); ++j) {
            assert(values[result.antichain[i]] % values[result.antichain[j]] != 0);
            assert(values[result.antichain[j]] % values[result.antichain[i]] != 0);
        }
    }
}
