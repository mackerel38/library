#include "recipe/connectivityfrontier.hpp"

using namespace std;
using namespace poe;

int main() {
    assert(minimum_connected_grid_completion({0b010, 0, 0b101}, 3) == 2);
    assert(minimum_connected_grid_completion({0b111, 0b111}, 3) == 0);
    assert(minimum_connected_grid_completion({0, 0b1, 0, 0b1, 0}, 1) == 1);
}
