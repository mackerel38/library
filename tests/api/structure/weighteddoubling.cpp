#include <cassert>
#include <string>
#include <vector>

#include "structure/doubling.hpp"

int main() {
    poe::weighteddoubling<long long> sum({1, 2, 0}, {10, 20, 30}, 20);
    auto [vertex, aggregate] = sum.jump(0, 5);
    assert(vertex == 2 && aggregate == 80);

    poe::weighteddoubling<std::string> concatenate(
        {1, 2, 0}, {"a", "b", "c"}, 10, std::plus<std::string>{}, std::string{});
    auto result = concatenate.jump(1, 5);
    assert(result.vertex == 0 && result.aggregate == "bcabc");
}
