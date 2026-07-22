#include <bits/stdc++.h>
#include "string/increasingsubstrings.hpp"

using namespace std;
using namespace poe;

int main() {
    assert(maximum_increasing_disjoint_substrings("") == 0);
    assert(maximum_increasing_disjoint_substrings("0") == 1);
    assert(maximum_increasing_disjoint_substrings("0101010") == 3);
    assert(maximum_increasing_disjoint_substrings(
        "000011001110101001011110001001") == 9);
    assert(maximum_increasing_disjoint_substrings("aaaa") == 2);
}
