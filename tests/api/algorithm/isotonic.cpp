#include <cassert>
#include <vector>

#include "algorithm/isotonic.hpp"

int main() {
    {
        const auto result = poe::right_transfer_isotonic({0, 0, -2});
        assert(result.values == std::vector<long long>({-1, -1, 0}));
        assert(result.operations == 3);
    }
    {
        const auto result = poe::right_transfer_isotonic({4, 5, 1, 2});
        assert(result.values == std::vector<long long>({3, 3, 3, 3}));
        assert(result.operations == 5);
    }
    {
        const auto result = poe::right_transfer_isotonic({-3, -1, -1, 8});
        assert(result.values == std::vector<long long>({-3, -1, -1, 8}));
        assert(result.operations == 0);
    }
}
