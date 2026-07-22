#include "structure/intervalmap.hpp"

using namespace poe;

int main() {
    intervalmap<long long, int> data(0, 10, 0);
    const auto first = data.assign(2, 7, 1);
    assert(first.size() == 1 && first[0].left == 2 && first[0].right == 7);
    const auto second = data.assign(4, 9, 2);
    assert(second.size() == 2);
    assert(data[1] == 0 && data[3] == 1 && data[8] == 2);
    data.assign(0, 4, 2);
    assert(data.segment_count() == 2);
    data.assign(9, 10, 2);
    assert(data.segment_count() == 1);
}
