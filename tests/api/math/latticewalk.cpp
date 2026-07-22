#include <bits/stdc++.h>
#include "math/latticewalk.hpp"

using namespace std;
using namespace poe;

int main() {
    axiswalkcounter<998244353> counter(314);
    assert(counter.count(3, 1) == 3);
    assert(counter.count(2, 1, 1) == 2);
    assert(counter.count(3, 2, 0, -1) == 3);
    assert(counter.count(1, 0, 0, 0) == 0);
    assert(counter.count(314, 15, 92, 65).val() == 106580952);
}
