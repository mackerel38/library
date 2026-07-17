#include "string/rollinghash.hpp"

int main() {
    poe::rollinghash<char> first(std::string("abracadabra"));
    poe::rollinghash<char> second(std::string("cadabra"));
    assert(first.size() == 11 && first[3] == 'a');
    assert(first.equal(4, 11, second, 0, 7));
    assert(first.lcp(0, 11, second, 0, 7) == 0);
    assert(first.lcp(4, 11, second, 0, 7) == 7);
    const auto combined = first.concat(first.hash(0, 4), first.hash(4, 7), 3);
    assert(combined == first.hash(0, 7));
    poe::rollinghash<int> sequence(std::vector<int>{1, 2, 1, 2});
    assert(sequence.hash(0, 2) == sequence.hash(2, 4));
}
