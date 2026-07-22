#include "graph/integerconstraints.hpp"

struct constraint {
    int left;
    int right;
    int lower;
    int upper;
};

bool brute(const std::vector<std::pair<int, int>>& bounds,
           const std::vector<constraint>& constraints) {
    std::vector<int> values(bounds.size());
    auto search = [&](auto&& self, int index) -> bool {
        if (index == static_cast<int>(bounds.size())) {
            for (const auto [left, right, lower, upper] : constraints) {
                const int sum = values[left] + values[right];
                if (sum < lower || upper < sum) return false;
            }
            return true;
        }
        for (values[index] = bounds[index].first;
             values[index] <= bounds[index].second; ++values[index]) {
            if (self(self, index + 1)) return true;
        }
        return false;
    };
    return search(search, 0);
}

int main() {
    std::mt19937 random(277);
    const auto next = [&](int limit) {
        return static_cast<int>(random() % static_cast<unsigned int>(limit));
    };
    for (int iteration = 0; iteration < 10000; ++iteration) {
        const int n = 1 + next(5);
        std::vector<std::pair<int, int>> bounds(n);
        for (auto& [lower, upper] : bounds) {
            lower = next(5) - 2;
            upper = lower + next(4);
        }
        std::vector<constraint> constraints(random() % 8);
        poe::boundedsumsat sat(bounds);
        for (auto& [left, right, lower, upper] : constraints) {
            left = next(n);
            right = next(n);
            lower = next(13) - 6;
            upper = lower + next(7);
            sat.add_sum_constraint(left, right, lower, upper);
        }
        const bool expected = brute(bounds, constraints);
        assert(sat.satisfiable() == expected);
        if (expected) {
            const auto& answer = sat.answer();
            for (int index = 0; index < n; ++index) {
                assert(bounds[index].first <= answer[index] &&
                       answer[index] <= bounds[index].second);
            }
            for (const auto [left, right, lower, upper] : constraints) {
                assert(lower <= answer[left] + answer[right] &&
                       answer[left] + answer[right] <= upper);
            }
        }
    }
}
