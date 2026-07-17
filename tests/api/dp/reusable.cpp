#include <cassert>
#include <optional>
#include <vector>

#include "dp/aliens.hpp"
#include "dp/digit.hpp"
#include "dp/monotone.hpp"
#include "dp/slopetrick.hpp"

int main() {
    {
        const auto result = poe::digitdp<long long>(
            "30", 0,
            [](int remainder, int digit, int) { return (remainder + digit) % 4; },
            poe::leadingzero::use
        );
        assert(result.sum([](int state, bool) { return state == 0; }) == 7);
    }
    {
        poe::slopetrick<long long> function;
        function.add_abs(3);
        function.add_left(5);
        assert(function.min() == 2);
        assert(function[3] == 2);
        assert(function[5] == 2);
        const auto [left, right] = function.argmin();
        assert(left == 3 && right == 5);
    }
    {
        const auto minimum = poe::monotone_minima<long long>(5, 6, [](int row, int column) {
            const long long difference = row - column;
            return difference * difference;
        });
        for (int row = 0; row < 5; ++row) assert(minimum[row] == std::pair(0LL, row));
    }
    {
        auto result = poe::aliens_max<long long>(-20, 20, 3, [](long long penalty) {
            std::pair<long long, int> best{std::numeric_limits<long long>::lowest(), 0};
            for (int count = 0; count <= 6; ++count) {
                const long long value = 9 - 1LL * (count - 3) * (count - 3);
                best = std::max(best, std::pair{value - penalty * count, -count});
            }
            return std::pair{best.first, -best.second};
        });
        assert(result.value == 9);
    }
}
