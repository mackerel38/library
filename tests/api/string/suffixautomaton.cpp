#include "string/suffixautomaton.hpp"

int main() {
    poe::suffixautomaton<char> automaton(std::string("ababa"));
    assert(automaton.length() == 5);
    assert(automaton.contains(std::string("bab")));
    assert(!automaton.contains(std::string("abb")));
    assert(automaton.count(std::string("aba")) == 2);
    assert(automaton.count(std::string("a")) == 3);
    assert(automaton.distinct_substrings() == 9);
    assert(automaton.longest_repeated_substring() == 3);
    const auto first = automaton.kth(0);
    assert(first && *first == std::vector<char>({'a'}));
    const auto common = automaton.longest_common_substring(std::string("xxbabay"));
    assert(common.length == 4 && common.begin() == 2);

    poe::suffixautomaton<int> online;
    online.add(1);
    online.add(2);
    online.add(1);
    online.build();
    assert(online.count(std::vector<int>{1}) == 2);
}
