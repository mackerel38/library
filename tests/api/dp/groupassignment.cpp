#include "dp/groupassignment.hpp"

#include <array>
#include <cassert>
#include <vector>

int main() {
    const std::vector<int> category = {0, 1, 0};
    const std::array<int, 2> target = {1, 1};
    const long long answer = poe::minimum_two_group_assignment<long long>(
        category,
        target,
        [](int index, int group, const auto&, const auto&) {
            return index + 10LL * group;
        },
        1LL << 60
    );
    assert(answer == 13);
}
