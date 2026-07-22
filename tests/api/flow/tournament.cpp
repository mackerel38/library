#include <bits/stdc++.h>
#include "flow/tournament.hpp"

using namespace std;
using namespace poe;

int main() {
    const vector<pair<int, int>> decided = {{1, 0}, {1, 2}};
    assert((possible_unique_roundrobin_winners(4, decided) == vector<int>{1, 3}));
    const auto completion = complete_roundrobin_for_unique_winner(4, decided, 3);
    assert(completion.possible);
    assert(completion.results.size() == 6);

    const vector<pair<int, int>> cycle = {{0, 1}, {1, 2}, {2, 0}};
    assert(possible_unique_roundrobin_winners(3, cycle).empty());
}
