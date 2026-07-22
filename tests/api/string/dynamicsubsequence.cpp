#include <bits/stdc++.h>
#include "math/modint.hpp"
#include "string/dynamicsubsequence.hpp"

using namespace std;
using namespace poe;

int main() {
    dynamicsubsequencecount<modint998244353, 2> sequence({2, 1, 1});
    assert(sequence.size() == 3);
    assert(sequence.count() == 5);
    sequence.set(1, 3);
    assert(sequence[1] == 3);
    assert(sequence.count() == 7);
    sequence.set(2, 3);
    assert(sequence.count() == 10);
}
