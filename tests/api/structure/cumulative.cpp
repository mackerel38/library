#include <algorithm>
#include <cassert>
#include <vector>
#include "structure/cumulative.hpp"

using namespace std;
using namespace poe;

int main() {
    vector<int> values = {1, 4, 2, 3, 0, 5};
    auto prefix = multidimensional_cumulative(
        {2, 3}, values, [](int a, int b) { return max(a, b); });
    assert(prefix == vector<int>({1, 4, 4, 3, 4, 5}));

    auto suffix = multidimensional_cumulative(
        {2, 3}, values, [](int a, int b) { return max(a, b); },
        cumulativedirection::suffix);
    assert(suffix == vector<int>({5, 5, 5, 5, 5, 5}));
}
