#include <algorithm>
#include <cassert>
#include <optional>
#include <random>
#include <vector>

#include "dp/digit.hpp"
#include "dp/monotone.hpp"
#include "dp/slopetrick.hpp"

int main() {
    std::mt19937 random(20260715);
    for (int upper = 0; upper <= 1000; ++upper) {
        const auto result = poe::digitdp<long long>(
            std::to_string(upper), 0,
            [](int remainder, int digit, int) { return (remainder + digit) % 7; },
            poe::leadingzero::use
        );
        std::vector<long long> brute(7);
        for (int value = 0; value <= upper; ++value) {
            int sum = 0;
            for (int current = value; current > 0; current /= 10) sum += current % 10;
            ++brute[sum % 7];
        }
        for (int remainder = 0; remainder < 7; ++remainder) {
            assert(result.sum([&](int state, bool) { return state == remainder; }) == brute[remainder]);
        }
    }
    for (int trial = 0; trial < 300; ++trial) {
        const int rows = 1 + random() % 30;
        const int columns = 1 + random() % 30;
        std::vector<long long> row_offset(rows), column_offset(columns);
        for (auto& value : row_offset) value = random() % 20;
        for (auto& value : column_offset) value = random() % 20;
        auto value = [&](int row, int column) {
            const long long difference = 2LL * row - column;
            return difference * difference + row_offset[row] + column_offset[column];
        };
        const auto answer = poe::monotone_minima<long long>(rows, columns, value);
        for (int row = 0; row < rows; ++row) {
            std::pair<long long, int> brute{value(row, 0), 0};
            for (int column = 1; column < columns; ++column) {
                brute = std::min(brute, std::pair{value(row, column), column});
            }
            assert(answer[row] == brute);
        }
    }
    constexpr int low = -200;
    constexpr int high = 200;
    constexpr long long inf = 1LL << 50;
    for (int trial = 0; trial < 200; ++trial) {
        poe::slopetrick<long long> function;
        std::vector<long long> brute(high - low + 1);
        for (int operation = 0; operation < 30; ++operation) {
            const int type = random() % 4;
            if (type == 0) {
                const int a = static_cast<int>(random() % 41) - 20;
                function.add_left(a);
                for (int x = low; x <= high; ++x) brute[x - low] += std::max(0, a - x);
            } else if (type == 1) {
                const int a = static_cast<int>(random() % 41) - 20;
                function.add_right(a);
                for (int x = low; x <= high; ++x) brute[x - low] += std::max(0, x - a);
            } else if (type == 2) {
                const int a = static_cast<int>(random() % 41) - 20;
                function.add_abs(a);
                for (int x = low; x <= high; ++x) brute[x - low] += std::abs(x - a);
            } else {
                const int a = random() % 3;
                const int b = a + random() % 3;
                function.shift(a, b);
                std::vector<long long> next(brute.size(), inf);
                for (int x = low; x <= high; ++x) {
                    for (int y = x - b; y <= x - a; ++y) {
                        if (low <= y && y <= high) next[x - low] = std::min(next[x - low], brute[y - low]);
                    }
                }
                brute = std::move(next);
            }
            for (int x = -30; x <= 30; ++x) assert(function[x] == brute[x - low]);
        }
    }
}
