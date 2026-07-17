#include <cassert>
#include <queue>
#include <vector>

#include "recipe/connectivityfrontier.hpp"

long long brute(int height, int width) {
    const int cells = height * width;
    long long answer = 0;
    for (int mask = 0; mask < (1 << cells); ++mask) {
        if (!(mask & 1) || !(mask >> (cells - 1) & 1)) continue;
        std::queue<int> queue;
        std::vector<bool> seen(cells);
        queue.push(0);
        seen[0] = true;
        while (!queue.empty()) {
            const int cell = queue.front();
            queue.pop();
            const int row = cell % height;
            const int column = cell / height;
            for (const auto& [dr, dc] : {std::pair{-1, 0}, std::pair{1, 0}, std::pair{0, -1}, std::pair{0, 1}}) {
                const int next_row = row + dr;
                const int next_column = column + dc;
                if (next_row < 0 || next_row >= height || next_column < 0 || next_column >= width) continue;
                const int next = next_column * height + next_row;
                if (!(mask >> next & 1) || seen[next]) continue;
                seen[next] = true;
                queue.push(next);
            }
        }
        answer += seen[cells - 1];
    }
    return answer;
}

int main() {
    for (int height = 1; height <= 4; ++height) {
        for (int width = 1; width <= 4; ++width) {
            poe::connectivityfrontierdp<long long> dp(height, 0);
            for (int column = 1; column < width; ++column) dp.advance_all();
            assert(dp.sum_marked(height - 1) == brute(height, width));
        }
    }
}
