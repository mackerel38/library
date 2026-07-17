#include <cassert>

#include "string/fibonacciword.hpp"

int main() {
    poe::fibonacciword word("a", "b", 100);
    assert(word[0] == 'b' && word[1] == 'a' && word[2] == 'b');
    assert(word.count('a', 1, 7) == 3);
    assert(word.count('b', 0, 3) == 2);
}
