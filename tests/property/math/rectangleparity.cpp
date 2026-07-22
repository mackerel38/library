#include "math/rectangleparity.hpp"

using namespace std;
using namespace poe;

struct rectangleconstraint {
    int top, left, bottom, right, value;
};

bool satisfies(int width, int mask,
               const vector<rectangleconstraint>& constraints) {
    for (const auto& constraint : constraints) {
        int value = 0;
        for (int row = constraint.top; row < constraint.bottom; ++row) {
            for (int column = constraint.left; column < constraint.right; ++column) {
                value ^= mask >> (row * width + column) & 1;
            }
        }
        if (value != constraint.value) return false;
    }
    return true;
}

int main() {
    mt19937 random(276);
    const auto next = [&](int limit) {
        return static_cast<int>(random() % static_cast<unsigned int>(limit));
    };
    for (int iteration = 0; iteration < 5000; ++iteration) {
        const int height = 1 + next(3);
        const int width = 1 + next(3);
        vector<rectangleconstraint> constraints(random() % 8);
        rectangleparitysystem system(height, width);
        for (auto& constraint : constraints) {
            constraint.top = next(height);
            constraint.bottom = constraint.top + 1 + next(height - constraint.top);
            constraint.left = next(width);
            constraint.right = constraint.left + 1 + next(width - constraint.left);
            constraint.value = random() & 1;
            system.add_constraint(constraint.top, constraint.left,
                                  constraint.bottom, constraint.right,
                                  constraint.value);
        }
        bool expected = false;
        for (int mask = 0; mask < (1 << (height * width)); ++mask) {
            expected |= satisfies(width, mask, constraints);
        }
        assert(system.satisfiable() == expected);
        if (!expected) continue;
        int mask = 0;
        for (int row = 0; row < height; ++row) {
            for (int column = 0; column < width; ++column) {
                mask |= system(row, column) << (row * width + column);
            }
        }
        assert(satisfies(width, mask, constraints));
    }
}
