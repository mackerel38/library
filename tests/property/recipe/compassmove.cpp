#include "recipe/compassmove.hpp"

using namespace std;
using namespace poe;

int main() {
    constexpr int radius = 15;
    constexpr int width = 2 * radius + 1;
    const auto id = [](int x, int y) { return (x + radius) * width + y + radius; };
    for (unsigned int mask = 0; mask < 256; ++mask) {
        vector<int> distance(width * width, -1);
        queue<pair<int, int>> queue;
        distance[id(0, 0)] = 0;
        queue.emplace(0, 0);
        const compassmoves moves(mask);
        while (!queue.empty()) {
            const auto [x, y] = queue.front();
            queue.pop();
            for (int direction = 0; direction < 8; ++direction) {
                if (!moves.allowed(direction)) continue;
                const int nx = x + static_cast<int>(compassmoves::directions[direction].x);
                const int ny = y + static_cast<int>(compassmoves::directions[direction].y);
                if (abs(nx) > radius || abs(ny) > radius || distance[id(nx, ny)] != -1) continue;
                distance[id(nx, ny)] = distance[id(x, y)] + 1;
                queue.emplace(nx, ny);
            }
        }
        for (int x = -6; x <= 6; ++x) {
            for (int y = -6; y <= 6; ++y) {
                const auto actual = moves.distance(x, y);
                const int expected = distance[id(x, y)];
                assert((actual ? *actual : -1) == expected);
            }
        }
    }
}
