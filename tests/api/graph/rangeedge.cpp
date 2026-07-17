#include <cassert>
#include <vector>

#include "graph/rangeedge.hpp"

int main() {
    using transition = poe::rangedistancetransition<long long>;
    const std::vector<long long> position{0, 20, 50, 90, 110, 150};
    const std::vector<transition> transitions{
        {0, 2, 4, 6, 100},
        {0, 1, 1, 3, 10000},
        {5, 6, 0, 2, 30},
    };
    const auto distance =
        poe::range_distance_shortest_paths(position, transitions, 0);
    assert(distance[0] == 0 && distance[1] == 410 && distance[2] == 10050);
    assert(distance[3] > 1'000'000);
    assert(distance[4] == 210 && distance[5] == 250);
}
