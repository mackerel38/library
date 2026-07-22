#include <bits/stdc++.h>
#include "algorithm/rangesort.hpp"

using namespace std;
using namespace poe;

int main() {
    const vector<int> values = {1, 4, 5, 2, 3};
    const vector<rangesortquery> queries = {{2, 5, true}, {0, 3, false}};
    assert(position_after_range_sorts(values, 1, queries) == 2);
    assert(position_after_range_sorts(vector<string>{"c", "a", "b"}, string{"b"},
                                      {{0, 3, true}}) == 1);
}
