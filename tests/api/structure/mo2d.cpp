#include <cassert>
#include <vector>
#include "structure/mo.hpp"

using namespace std;
using namespace poe;

int main() {
    Mo2d mo(20, 20);
    mo.add(3, 7);
    mo.add(10, 2);
    mo.add(0, 0);
    vector<int> answers(3);
    int x = 0, y = 0;
    mo.run(
        [&](int old) { assert(old == x); ++x; },
        [&](int old) { assert(old == x); --x; },
        [&](int old) { assert(old == y); ++y; },
        [&](int old) { assert(old == y); --y; },
        [&](int id) { answers[id] = x * x + 3 * y; }
    );
    assert(answers == vector<int>({30, 106, 0}));
}
