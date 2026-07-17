#include <cassert>
#include <random>

#include "dp/rangeknapsack.hpp"

using namespace std;
using namespace poe;

int main() {
    mt19937 random(426);
    for (int test = 0; test < 250; ++test) {
        const int size = random() % 8 + 1;
        vector<int> weights(size);
        vector<long long> values(size);
        for (int i = 0; i < size; ++i) {
            weights[i] = random() % 7;
            values[i] = static_cast<int>(random() % 20) - 4;
        }
        vector<rangeknapsackquery> queries;
        vector<long long> expected;
        for (int left = 0; left < size; ++left) {
            for (int right = left + 1; right <= size; ++right) {
                for (int capacity = 0; capacity <= 12; ++capacity) {
                    queries.push_back({left, right, capacity});
                    long long best = 0;
                    const int length = right - left;
                    for (int mask = 0; mask < (1 << length); ++mask) {
                        int weight = 0;
                        long long value = 0;
                        for (int i = 0; i < length; ++i) {
                            if ((mask >> i) & 1) {
                                weight += weights[left + i];
                                value += values[left + i];
                            }
                        }
                        if (weight <= capacity) best = max(best, value);
                    }
                    expected.push_back(best);
                }
            }
        }
        assert(range_knapsack01(weights, values, queries) == expected);
    }
}
