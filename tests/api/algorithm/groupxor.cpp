#include <cassert>
#include <vector>

#include "algorithm/groupxor.hpp"

int main() {
    const std::vector<std::vector<unsigned long long>> groups{{2, 3}, {4, 6}};
    assert((poe::kth_largest_group_xor<unsigned long long, 3>(groups, 3) == 5));
}
