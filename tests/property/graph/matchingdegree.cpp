#include <cassert>
#include <random>

#include "graph/matching.hpp"

using namespace std;
using namespace poe;

bool brute_assign(const vector<int>& capacity, const vector<int>& degree) {
    vector<int> used(capacity.size());
    auto solve = [&](auto&& self, int right) -> bool {
        if (right == static_cast<int>(degree.size())) return true;
        const int left_size = static_cast<int>(capacity.size());
        for (int mask = 0; mask < (1 << left_size); ++mask) {
            if (popcount(static_cast<unsigned>(mask)) != degree[right]) continue;
            bool valid = true;
            for (int left = 0; left < left_size; ++left) {
                if ((mask >> left) & 1) valid &= ++used[left] <= capacity[left];
            }
            if (valid && self(self, right + 1)) return true;
            for (int left = 0; left < left_size; ++left) {
                if ((mask >> left) & 1) --used[left];
            }
        }
        return false;
    };
    return solve(solve, 0);
}

int main() {
    mt19937 random(424);
    for (int test = 0; test < 1000; ++test) {
        const int left_size = random() % 5;
        const int right_size = random() % 7;
        vector<int> capacity(left_size), degree(right_size);
        vector<long long> reward(right_size);
        for (int& value : capacity) value = random() % (right_size + 1);
        for (int right = 0; right < right_size; ++right) {
            degree[right] = random() % (left_size + 1);
            reward[right] = random() % 20;
        }

        assert(bipartite_degree_feasible(capacity, degree) == brute_assign(capacity, degree));
        long long expected = 0;
        for (int mask = 0; mask < (1 << right_size); ++mask) {
            vector<int> selected_degree;
            long long value = 0;
            for (int right = 0; right < right_size; ++right) {
                if ((mask >> right) & 1) {
                    selected_degree.push_back(degree[right]);
                    value += reward[right];
                }
            }
            if (brute_assign(capacity, selected_degree)) expected = max(expected, value);
        }
        assert(max_bipartite_degree_reward(capacity, degree, reward) == expected);
    }
}
