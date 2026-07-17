#include "string/rotation.hpp"

int main() {
    const std::string text = "baca";
    assert(poe::minimum_rotation(text) == 3);
    assert(poe::rotated(text, poe::minimum_rotation(text)) == "abac");
    assert(poe::rotated(text, poe::maximum_rotation(text)) == "caba");
    assert(poe::minimum_rotation(std::string("aaaa")) == 0);
    assert(poe::minimum_rotation(std::string()) == 0);
    assert(poe::rotated(std::string(), 0).empty());
    const std::vector<int> values = {2, 1, 2, 1};
    assert(poe::minimum_rotation(values) == 1);
    assert(poe::rotation_distance(std::string("1010001"), std::string("1000110")) == 2);
    assert(poe::rotation_distance(std::string("0101"), std::string("0011")) == -1);
}
