#include "tree/subarrayunion.hpp"

int main() {
    std::vector<int> parent{-1, 0, 0, 1};
    std::vector<int> vertices{3, 2};
    std::vector<long long> weight{1, 2, 4, 8};
    assert(poe::sum_subarray_root_path_unions(parent, vertices, weight) == 31);
}
