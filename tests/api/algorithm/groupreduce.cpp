#include <cassert>
#include <vector>

#include "algorithm/groupreduce.hpp"

int main() {
    assert(poe::reduce_adjacent_groups(std::vector<int>{1, 1, 2, 2, 2, 1}, 3) ==
           std::vector<int>({}));
    assert(poe::reduce_adjacent_groups(std::vector<int>{1, 1, 1, 1, 1}, 4) ==
           std::vector<int>({1}));
    assert(poe::reduce_adjacent_groups(std::vector<int>{1, 2, 2, 1}, 2).empty());
}
