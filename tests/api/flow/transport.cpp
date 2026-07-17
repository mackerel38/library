#include <cassert>

#include "flow/mincost.hpp"

using namespace std;
using namespace poe;

int main() {
    using edge = costtransportedge<int, long long>;
    const auto possible = mincost_transport<int, long long>(
        {2, 0, 1}, {0, 2, 0}, {edge{0, 1, 2, 3}, edge{2, 1, 1, 1}}
    );
    assert(possible.feasible() && possible.flow == 2 && possible.cost == 4);
    const auto impossible = mincost_transport<int, long long>(
        {1, 0}, {0, 2}, {edge{0, 1, 1, 5}}
    );
    assert(!impossible.feasible() && impossible.flow == 1);
}
