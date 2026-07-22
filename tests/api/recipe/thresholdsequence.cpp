#include <cassert>

#include "recipe/thresholdsequence.hpp"

using namespace std;
using namespace poe;

int main() {
    thresholdsequence<long long> sequence(31, 41);
    assert(sequence.empty());
    assert(sequence.apply(9) == 9);
    for (const long long value : {59LL, -26LL, 53LL, 58LL}) sequence.append(value);
    assert(sequence.size() == 4);
    assert(sequence.apply(9) == 61);
    sequence.append(79);
    assert(sequence.apply(32) == 43);
    sequence.append(38);
    assert(sequence.apply(462) == 216);
}
