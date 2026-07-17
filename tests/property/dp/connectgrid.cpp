#include "recipe/connectivityfrontier.hpp"

using namespace std;
using namespace poe;

int brute(const vector<uint64_t>& required, int width) {
    const int rows = required.size();
    const int cells = rows * width;
    int required_bits = 0;
    for (int row = 0; row < rows; ++row) {
        for (int column = 0; column < width; ++column) {
            if (required[row] >> column & 1ULL) required_bits |= 1 << (row * width + column);
        }
    }
    int answer = cells;
    for (int mask = required_bits; mask < (1 << cells); mask = (mask + 1) | required_bits) {
        const int start = countr_zero(static_cast<unsigned>(mask));
        int seen = 1 << start;
        vector<int> stack{start};
        while (!stack.empty()) {
            const int cell = stack.back();
            stack.pop_back();
            const int row = cell / width;
            const int column = cell % width;
            for (const auto [dr, dc] : array<pair<int, int>, 4>{{{-1, 0}, {1, 0}, {0, -1}, {0, 1}}}) {
                const int nr = row + dr;
                const int nc = column + dc;
                if (nr < 0 || nr >= rows || nc < 0 || nc >= width) continue;
                const int next = nr * width + nc;
                if (!(mask >> next & 1) || (seen >> next & 1)) continue;
                seen |= 1 << next;
                stack.push_back(next);
            }
        }
        if (seen == mask) answer = min(answer, popcount(static_cast<unsigned>(mask ^ required_bits)));
        if (mask == (1 << cells) - 1) break;
    }
    return answer;
}

int main() {
    mt19937 random(20260716);
    for (int rows = 1; rows <= 4; ++rows) {
        for (int width = 1; width <= 4 && rows * width <= 16; ++width) {
            for (int trial = 0; trial < 300; ++trial) {
                vector<uint64_t> required(rows);
                do {
                    for (uint64_t& mask : required) mask = random() % (1 << width);
                } while (ranges::all_of(required, [](uint64_t mask) { return mask == 0; }));
                assert(minimum_connected_grid_completion(required, width) == brute(required, width));
            }
        }
    }
}
