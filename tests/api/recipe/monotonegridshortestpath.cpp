#include <cassert>
#include <string>
#include <vector>

#include "recipe/monotonegridshortestpath.hpp"

int main() {
    std::vector<std::string> cells{"...", ".#.", "..."};
    poe::monotonegridshortestpath<3> path(cells);
    assert(path.width() == 3);
    assert(!path.get(1, 1));
    assert(path[0][2]);
    assert(path.distance(0, 0, 2, 2) == 4);
    path.toggle(0, 1);
    assert(path.distance(0, 0, 2, 2) == 4);
    path.set(2, 1, false);
    assert(path.distance(0, 0, 2, 2) == -1);
}
