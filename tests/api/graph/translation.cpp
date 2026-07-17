#include <cassert>
#include <vector>

#include "graph/translation.hpp"

int main() {
    assert(poe::minimum_safe_translation_clear_steps(
        2, 2,
        std::vector<poe::gridpoint>{{0, 0}},
        std::vector<poe::gridpoint>{{1, 1}}
    ) == 1);
    assert(poe::minimum_safe_translation_clear_steps(
        3, 3,
        std::vector<poe::gridpoint>{
            {0, 0}, {0, 1}, {0, 2}, {1, 0},
            {1, 2}, {2, 0}, {2, 1}, {2, 2}
        },
        std::vector<poe::gridpoint>{{1, 1}}
    ) == -1);
}
