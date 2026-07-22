#include "algorithm/intervalcrossing.hpp"

using namespace poe;

int main() {
    assert(count_alternating_interval_pairs<int>({{0, 3}, {1, 4}, {2, 5}}) == 3);
    assert(count_alternating_interval_pairs<int>({{0, 4}, {1, 3}, {4, 7}}) == 0);
    assert(count_alternating_interval_pairs<int>({{0, 3}, {0, 4}, {1, 4}}) == 1);
}
