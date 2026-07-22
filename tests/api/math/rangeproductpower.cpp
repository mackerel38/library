#include <bits/stdc++.h>
#include "math/rangeproductpower.hpp"

using namespace std;
using namespace poe;

int main() {
    const vector<int> values = {7, 49, 30, 1, 15, 8, 6, 10};
    const vector<pair<int, int>> queries = {{0, 2}, {1, 3}, {3, 4}, {4, 8}, {2, 8}};
    assert(range_product_power_queries(values, queries, 3)
           == vector<bool>({true, false, true, false, true}));
    assert(range_product_power_queries(vector<int>{2, 8, 4}, {{0, 2}, {0, 3}}, 2)
           == vector<bool>({true, true}));
}
