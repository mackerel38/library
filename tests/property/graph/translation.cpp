#include <array>
#include <cassert>
#include <queue>
#include <random>
#include <vector>

#include "graph/translation.hpp"

namespace {

int brute(int height, int width, int occupied, int forbidden) {
    const int count = height * width;
    std::vector<int> distance(1 << count, -1);
    distance[occupied] = 0;
    std::queue<int> queue;
    queue.push(occupied);
    while (!queue.empty()) {
        const int mask = queue.front();
        queue.pop();
        if (mask == 0) return distance[mask];
        for (const auto direction : poe::grid4) {
            int next = 0;
            bool safe = true;
            for (int index = 0; index < count; ++index) {
                if (!(mask >> index & 1)) continue;
                const int row = index / width + direction.row;
                const int column = index % width + direction.column;
                if (!poe::inside_grid(row, column, height, width)) continue;
                const int destination = row * width + column;
                if (destination == forbidden) {
                    safe = false;
                    break;
                }
                next |= 1 << destination;
            }
            if (safe && distance[next] == -1) {
                distance[next] = distance[mask] + 1;
                queue.push(next);
            }
        }
    }
    return -1;
}

}

int main() {
    std::mt19937 random(427);
    for (int trial = 0; trial < 500; ++trial) {
        const int height = 2 + random() % 3;
        const int width = 2 + random() % 3;
        const int count = height * width;
        const int forbidden = random() % count;
        int mask = random() & ((1 << count) - 1);
        mask &= ~(1 << forbidden);
        std::vector<poe::gridpoint> occupied;
        for (int index = 0; index < count; ++index) {
            if (mask >> index & 1) occupied.push_back({index / width, index % width});
        }
        assert(poe::minimum_safe_translation_clear_steps(
                   height, width, occupied,
                   std::vector<poe::gridpoint>{{forbidden / width, forbidden % width}}
               ) == brute(height, width, mask, forbidden));
    }
}
