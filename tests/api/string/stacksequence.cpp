#include <cassert>
#include "string/stacksequence.hpp"

int main() {
    poe::stacksequence<998244353, 10> sequence({1, 2, -1, 3, -1, -1, 4});
    auto result = sequence.all_prod();
    assert(result.length == 1 && result.value.val() == 4);
    result = sequence.prod(0, 4);
    assert(result.length == 2 && result.value.val() == 13);
    sequence.set(2, 5);
    assert(sequence[2] == 5);
    assert(sequence.prod(0, 4).value.val() == 1253);
}
