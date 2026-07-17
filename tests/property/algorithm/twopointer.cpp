#include <cassert>
#include <random>
#include <vector>

#include "algorithm/twopointer.hpp"

int main() {
    std::mt19937 random(20260715);
    for (int trial = 0; trial < 500; ++trial) {
        const int n = random() % 9;
        std::vector<int> values(n);
        for (int& value : values) value = random() % 5;
        std::vector<long long> brute(n * (n - 1) / 2 + 1);
        for (int left = 0; left < n; ++left) {
            long long inversions = 0;
            for (int right = left; right < n; ++right) {
                for (int i = left; i < right; ++i) inversions += values[i] > values[right];
                ++brute[inversions];
            }
        }
        for (int target = 0; target < static_cast<int>(brute.size()); ++target) {
            assert(poe::count_subarrays_inversions(values, target) == brute[target]);
            assert(poe::count_subarrays_inversions_at_most(values, target) ==
                   std::accumulate(brute.begin(), brute.begin() + target + 1, 0LL));
        }
    }
    for (int trial = 0; trial < 500; ++trial) {
        const int height = 1 + random() % 6;
        const int width = 1 + random() % 6;
        std::vector matrix(height, std::vector<int>(width));
        for (auto& row : matrix) {
            for (int& value : row) value = random() % 4;
        }
        for (int target = 0; target <= 12; ++target) {
            long long brute = 0;
            for (int top = 0; top < height; ++top) {
                for (int bottom = top + 1; bottom <= height; ++bottom) {
                    for (int left = 0; left < width; ++left) {
                        for (int right = left + 1; right <= width; ++right) {
                            int sum = 0;
                            for (int row = top; row < bottom; ++row) {
                                for (int column = left; column < right; ++column) {
                                    sum += matrix[row][column];
                                }
                            }
                            brute += sum == target;
                        }
                    }
                }
            }
            assert(poe::count_subrectangles_sum_nonnegative(matrix, target) == brute);
        }
    }
    for (int trial = 0; trial < 1000; ++trial) {
        const int size = random() % 20;
        std::vector<int> values(size);
        for (int& value : values) value = random() % 4;
        const int minimum_first = 1 + random() % 8;
        const int maximum_second = 1 + random() % 8;
        long long expected = 0;
        for (int left = 0; left < size; ++left) {
            int first = 0, second = 0;
            for (int right = left; right < size; ++right) {
                first += values[right] <= 1;
                second += values[right] >= 2;
                expected += first >= minimum_first && second < maximum_second;
            }
        }
        assert(poe::count_subarrays_frequency_constraints(
            values, minimum_first, maximum_second,
            [](int value) { return value <= 1; }, [](int value) { return value >= 2; }
        ) == expected);
    }
    for (int trial = 0; trial < 1000; ++trial) {
        const int size = 1 + random() % 20;
        std::vector<int> values(size);
        int total = 0;
        for (int& value : values) {
            value = random() % 6;
            total += value;
        }
        if (total == 0) continue;
        const int target = 1 + random() % total;
        const auto result = poe::circular_threshold_windows(values, target);
        for (int left = 0; left < size; ++left) {
            int sum = 0;
            int length = 0;
            while (sum < target) sum += values[(left + length++) % size];
            assert(result[left].length == length && result[left].sum == sum);
        }
    }
}
