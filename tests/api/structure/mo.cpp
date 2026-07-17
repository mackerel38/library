#include "structure/mo.hpp"

int main() {
    const std::vector<int> values = {3, 1, 4, 1, 5};
    poe::Mo mo(values.size());
    const int first = mo.add(1, 4);
    const int second = mo.add(0, 5);
    const int empty = mo.add(2, 2);
    std::vector<int> answer(mo.size());
    int sum = 0;
    mo.run([&](int index) { sum += values[index]; },
           [&](int index) { sum -= values[index]; },
           [&](int id) { answer[id] = sum; });
    assert(answer[first] == 6);
    assert(answer[second] == 14);
    assert(answer[empty] == 0);
}
