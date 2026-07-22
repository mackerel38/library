#include "math/rectangleparity.hpp"

int main() {
    poe::rectangleparitysystem system(3, 4);
    system.add_constraint(0, 0, 2, 3, 1);
    system.add_constraint(1, 1, 3, 4, 0);
    assert(system.satisfiable());
    const auto& matrix = system.answer();
    int first = 0;
    int second = 0;
    for (int row = 0; row < 2; ++row) {
        for (int column = 0; column < 3; ++column) first ^= matrix[row][column];
    }
    for (int row = 1; row < 3; ++row) {
        for (int column = 1; column < 4; ++column) second ^= system(row, column);
    }
    assert(first == 1 && second == 0);
}
