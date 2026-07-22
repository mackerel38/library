#include "string/trie.hpp"

int main() {
    poe::trie<char> dictionary;
    const int apple = dictionary.add(std::string("apple"));
    dictionary.add(std::string("app"));
    dictionary.add(std::string("apple"));
    dictionary.add(std::string("ape"));
    assert(dictionary.word_count() == 4);
    assert(dictionary.count(std::string("apple")) == 2);
    assert(dictionary.count(std::string("ap")) == 0);
    assert(dictionary.prefix_count(std::string("ap")) == 4);
    assert(dictionary.prefix_count(std::string("app")) == 3);
    const int app_node = dictionary.find(std::string("app"));
    assert(app_node != -1 && dictionary[app_node].depth == 3);
    assert(dictionary.terminal(apple) == dictionary.find(std::string("apple")));

    poe::ahocorasick<char> automaton('a', 'z');
    const int he = automaton.add(std::string("he"));
    const int she = automaton.add(std::string("she"));
    const int his = automaton.add(std::string("his"));
    const int hers = automaton.add(std::string("hers"));
    automaton.build();
    const std::string text = "ahishers";
    const auto count = automaton.count(text);
    assert(count == std::vector<long long>({1, 1, 1, 1}));
    const auto positions = automaton.find_all(text);
    assert(positions[he] == std::vector<int>{4});
    assert(positions[she] == std::vector<int>{3});
    assert(positions[his] == std::vector<int>{1});
    assert(positions[hers] == std::vector<int>{4});
    const auto matches = automaton.match(text);
    assert(matches.size() == 4);
    int state = 0;
    for (char symbol : std::string("she")) state = automaton.next(state, symbol);
    const unsigned mask = automaton.pattern_mask<unsigned>(state);
    assert((mask >> he & 1U) && (mask >> she & 1U));
    assert(automaton.shortest_match_length(state) == 2);
    assert(!automaton.shortest_match_length(0));
}
