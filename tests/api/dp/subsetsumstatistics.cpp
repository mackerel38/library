#include <cassert>
#include <vector>
#include "dp/subsetsum.hpp"

using namespace std;
using namespace poe;

int main() {
    auto result = subset_sum_less_by_size({1, 2, 4}, 5);
    assert(result.count == vector<unsigned long long>({1, 3, 1, 0}));
    assert(result.sum[0] == 0);
    assert(result.sum[1] == 7);
    assert(result.sum[2] == 3);
}
