#include <algorithm>
#include <cassert>
#include <functional>
#include <limits>
#include <random>
#include <vector>

#include "dp/rectanglepartition.hpp"
#include "structure/prefixsum.hpp"

using namespace poe;

int main() {
    std::mt19937 random(233);
    for (int trial = 0; trial < 500; ++trial) {
        const int size = random() % 4 + 1;
        const int cells = size * size;
        int target = 0;
        std::vector<std::vector<int>> marked(size, std::vector<int>(size));
        for (int cell = 0; cell < cells; ++cell) {
            if (random() % 2) target |= 1 << cell;
            marked[cell / size][cell % size] = target >> cell & 1;
        }
        const prefixsum2d<int> sum(marked);
        const int actual = minimum_rectangle_partition_cost<int>(size, size,
            [&](int top, int left, int bottom, int right) {
                return sum.sum(top, left, bottom, right) == 0
                    ? 0 : std::max(bottom - top, right - left);
            });

        std::vector<int> memo(1 << cells, -1);
        std::function<int(int)> solve = [&](int remaining) -> int {
            if (remaining == 0) return 0;
            int& answer = memo[remaining];
            if (answer != -1) return answer;
            answer = std::numeric_limits<int>::max();
            const int cell = std::countr_zero(static_cast<unsigned>(remaining));
            const int row = cell / size;
            const int column = cell % size;
            for (int side = 1; side <= size; ++side) {
                for (int top = std::max(0, row - side + 1); top <= row && top + side <= size; ++top) {
                    for (int left = std::max(0, column - side + 1);
                         left <= column && left + side <= size; ++left) {
                        int covered = 0;
                        for (int y = top; y < top + side; ++y) {
                            for (int x = left; x < left + side; ++x) covered |= 1 << (y * size + x);
                        }
                        answer = std::min(answer, side + solve(remaining & ~covered));
                    }
                }
            }
            return answer;
        };
        assert(actual == solve(target));
    }
}
