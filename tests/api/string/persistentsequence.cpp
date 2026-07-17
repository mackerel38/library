#include <cassert>
#include <vector>
#include "string/persistentsequence.hpp"
int main() {
    poe::persistentsequenceorder<int> sequences;
    assert(sequences.add(0, 2) == 1);
    assert(sequences.add(0, 1) == 2);
    assert(sequences.add(2, 2) == 3);
    assert(sequences.add(0, 1) == 4);
    assert(sequences.order() == std::vector<int>({2, 4, 3, 1}));
    assert(sequences.node(2) == sequences.node(4));

    poe::persistentconcatenation<char> concatenation({'0', '1'}, 100);
    const int binary = concatenation.add(0, 1);
    const int word = concatenation.add(binary, binary);
    assert(concatenation.length(word) == 4);
    assert(concatenation.get(word, 0) == '0');
    assert(concatenation.get(word, 3) == '1');
}
