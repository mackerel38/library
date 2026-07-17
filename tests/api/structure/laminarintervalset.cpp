#include <cassert>
#include "structure/laminarintervalset.hpp"

int main() {
    poe::laminarintervalset data(10);
    assert(data.add(1, 8));
    assert(data.add(3, 5));
    assert(!data.add(2, 4));
    assert(data.add(0, 9));
    assert(data.size() == 3);
    assert((data.intervals() == std::vector<std::pair<int, int>>{
        {1, 8}, {3, 5}, {0, 9}}));
}
