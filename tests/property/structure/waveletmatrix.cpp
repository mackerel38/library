#include "structure/waveletmatrix.hpp"

int main() {
    std::mt19937 random(812739);
    for (int trial = 0; trial < 300; ++trial) {
        const int n = random() % 80;
        std::vector<int> values(n);
        for (int& value : values) value = static_cast<int>(random() % 61) - 30;
        poe::waveletmatrix<int, long long> wm(values);
        for (int query = 0; query < 300; ++query) {
            int left = random() % (n + 1);
            int right = random() % (n + 1);
            if (left > right) std::swap(left, right);
            int lower = static_cast<int>(random() % 81) - 40;
            int upper = static_cast<int>(random() % 81) - 40;
            if (lower > upper) std::swap(lower, upper);
            int count_less = 0, count_leq = 0, count_range = 0;
            long long sum = 0, sum_less = 0, sum_leq = 0, sum_range = 0;
            std::vector<int> sorted;
            for (int index = left; index < right; ++index) {
                const int value = values[index];
                sorted.push_back(value);
                sum += value;
                if (value < upper) ++count_less, sum_less += value;
                if (value <= upper) ++count_leq, sum_leq += value;
                if (lower <= value && value < upper) ++count_range, sum_range += value;
            }
            std::sort(sorted.begin(), sorted.end());
            assert(wm.count_less(left, right, upper) == count_less);
            assert(wm.count_leq(left, right, upper) == count_leq);
            assert(wm.count(left, right, lower, upper) == count_range);
            assert(wm.sum(left, right) == sum);
            assert(wm.sum_less(left, right, upper) == sum_less);
            assert(wm.sum_leq(left, right, upper) == sum_leq);
            assert(wm.sum(left, right, lower, upper) == sum_range);
            for (int k = 0; k < static_cast<int>(sorted.size()); ++k) {
                assert(wm.kth(left, right, k) == sorted[k]);
            }
            const auto previous = wm.prev(left, right, upper);
            auto previous_iterator = std::lower_bound(sorted.begin(), sorted.end(), upper);
            if (previous_iterator == sorted.begin()) {
                assert(!previous);
            } else {
                assert(previous == *std::prev(previous_iterator));
            }
            const auto next = wm.next(left, right, lower);
            auto next_iterator = std::lower_bound(sorted.begin(), sorted.end(), lower);
            if (next_iterator == sorted.end()) {
                assert(!next);
            } else {
                assert(next == *next_iterator);
            }
        }
    }
}
