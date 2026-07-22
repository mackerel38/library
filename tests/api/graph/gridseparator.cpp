#include <bits/stdc++.h>
#include "graph/gridseparator.hpp"

using namespace std;
using namespace poe;

int main() {
    vector<vector<bool>> removable = {
        {false, true, true},
        {false, true, true},
        {true, true, false},
        {true, true, false},
    };
    const auto result = minimum_grid_vertex_separator(
        removable, pair{0, 0}, pair{3, 2});
    assert(result.possible());
    assert(result.size == 3);
    assert(result.count.val() == 6);

    vector<vector<bool>> impossible = {
        {true, false},
        {true, false},
        {true, false},
    };
    assert(!minimum_grid_vertex_separator(
        impossible, pair{2, 1}, pair{0, 1}).possible());
}
