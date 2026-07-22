#include <bits/stdc++.h>
#include "math/floorsqrtchain.hpp"

using namespace std;
using namespace poe;

int main() {
    floorsqrtchaincounter counter(1000000000000000000ULL);
    assert(counter.count(1) == 1);
    assert(counter(16) == 5);
    assert(counter(123456789012ULL) == 4555793983LL);
    assert(counter(1000000000000000000ULL) == 23561347048791096LL);
}
