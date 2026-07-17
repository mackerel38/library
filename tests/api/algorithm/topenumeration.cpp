#include "algorithm/topenumeration.hpp"

int main() {
    const auto sums = poe::top_multiset_sums<long long>({20, 10}, 4, 3);
    assert((sums == std::vector<long long>{80, 70, 60}));
    assert((poe::top_multiset_sums<long long>({5, 5}, 2, 3)
            == std::vector<long long>{10, 10, 10}));
    assert((poe::top_multiset_sums<long long>({1, 2}, 0, 10)
            == std::vector<long long>{0}));
}
