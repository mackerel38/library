#include <cassert>

#include "structure/median.hpp"

int main() {
    poe::median<long long> values;
    assert(values.empty());
    assert(values.deviation() == 0);
    for (const long long value : {5, 2, 3, 9, 1}) values.add(value);
    assert(values.size() == 5);
    assert(values.lower() == 3);
    assert(values.upper() == 3);
    assert(values.deviation() == 11);
    values.erase(3);
    assert(values.lower() == 2);
    assert(values.upper() == 5);
    assert(values.deviation() == 11);
    values.erase(1);
    values.erase(9);
    assert(values.lower() == 2);
    assert(values.upper() == 5);
}
