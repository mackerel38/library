#include <cassert>
#include <cstdlib>

#include "dp/rectanglepartition.hpp"

using namespace poe;

int main() {
    const int answer = minimum_rectangle_partition_cost<int>(2, 3,
        [](int top, int left, int bottom, int right) {
            return (bottom - top) * (right - left) + 1;
        });
    assert(answer == 7);
    assert(minimum_rectangle_partition_cost<int>(0, 3, [](auto...) { return 1; }) == 0);
}
