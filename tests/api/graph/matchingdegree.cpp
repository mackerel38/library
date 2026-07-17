#include <cassert>

#include "graph/matching.hpp"

using namespace std;
using namespace poe;

int main() {
    const vector<int> capacity{1, 1, 3};
    const vector<int> degree{1, 2, 3};
    const vector<long long> reward{1, 5, 10};
    assert(bipartite_degree_feasible(capacity, {3, 1}));
    assert(!bipartite_degree_feasible(capacity, {3, 2}));
    assert(max_bipartite_degree_reward(capacity, degree, reward) == 11);
    assert(max_bipartite_degree_reward<long long>({}, {0, 1}, {4, 100}) == 4);
}
