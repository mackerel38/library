#include <cassert>
#include <vector>

#include "graph/travelingsalesman.hpp"

int main() {
    const long long inf = 1LL << 60;
    std::vector<std::vector<long long>> cost{
        {inf, 1, 5, 3},
        {2, inf, 4, 1},
        {1, 2, inf, 1},
        {2, 4, 1, inf}
    };
    const auto result = poe::travelingsalesman(cost, inf);
    assert(result.reachable);
    assert(result.cost == 4);
    assert(result.path.front() == 0 && result.path.back() == 0);
    assert(result.path.size() == 5);

    cost[3][2] = inf;
    cost[1][2] = inf;
    cost[0][2] = inf;
    assert(!poe::travelingsalesman(cost, inf).reachable);
}
