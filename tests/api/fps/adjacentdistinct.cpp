#include "fps/combinatorial.hpp"

using namespace std;
using namespace poe;

int main() {
    const auto binary = adjacent_distinct_words<998244353>(4, {1, 1});
    assert(binary.count[0] == 1);
    assert(binary.count[4] == 2);
    assert(binary.length_sum[4] == 8);

    const auto composition = adjacent_distinct_words<998244353>(4, {1, 2, 3, 4});
    assert(composition.count[4] == 4);
    assert(composition.length_sum[4] == 8);
}
