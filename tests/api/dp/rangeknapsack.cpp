#include <cassert>

#include "dp/rangeknapsack.hpp"

using namespace std;
using namespace poe;

int main() {
    const vector<int> weights{3, 5, 1, 2};
    const vector<long long> values{4, 8, 2, 3};
    const vector<rangeknapsackquery> queries{{0, 4, 7}, {1, 4, 10}, {0, 2, 2}};
    assert(range_knapsack01(weights, values, queries) == vector<long long>({11, 13, 0}));
    assert(range_knapsack01(weights, values, {}).empty());
}
