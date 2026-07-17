#include "string/runlength.hpp"

int main() {
    const auto runs = poe::runlength(std::string("aaabbcaaaa"));
    const std::vector<std::pair<char, int>> expected = {
        {'a', 3}, {'b', 2}, {'c', 1}, {'a', 4}};
    assert(runs == expected);
    assert(poe::runlength_string(runs) == "aaabbcaaaa");
    const std::vector<int> values = {1, 1, 2, 3, 3};
    assert(poe::runlength_decode(poe::runlength(values)) == values);
    assert(poe::runlength(std::string()).empty());
}
