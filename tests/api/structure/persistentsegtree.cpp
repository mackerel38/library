#include "structure/persistentsegtree.hpp"

long long add(long long left, long long right) { return left + right; }
long long zero() { return 0; }
std::string concatenate(std::string left, std::string right) { return left + right; }
std::string empty_string() { return {}; }

int main() {
    poe::persistentsegtree<long long, add, zero> sum({1, 2, 3, 4});
    const auto first = sum.initial();
    const auto second = sum.set(first, 1, 10);
    const auto third = sum.apply(second, 2, 5);
    assert(sum.prod(first, 0, 4) == 10);
    assert(sum.prod(second, 0, 3) == 14);
    assert(sum.get(third, 2) == 8 && sum.all_prod(third) == 23);

    poe::persistentsegtree<std::string, concatenate, empty_string> text({"a", "b", "c"});
    const auto changed = text.set(text.initial(), 1, "X");
    assert(text.prod(text.initial(), 0, 3) == "abc");
    assert(text.prod(changed, 0, 3) == "aXc");
}
