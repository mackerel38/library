#include <cassert>
#include <string>
#include "algorithm/shortestword.hpp"

using namespace poe;

int main() {
    const auto word = shortest_lexicographic_word<char>(
        4,
        0,
        [](int state) { return state == 3; },
        [](int state, auto relax) {
            if (state == 0) {
                relax(1, 'a');
                relax(2, 'b');
            } else if (state == 1) {
                relax(3, 'b');
            } else if (state == 2) {
                relax(3, 'a');
            }
        }
    );
    assert(word && std::string(word->begin(), word->end()) == "ab");
}
