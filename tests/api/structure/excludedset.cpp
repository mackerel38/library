#include "structure/excludedset.hpp"

int main() {
    poe::excludedset<long long> data({16, 9, 2, 3, 1, 3});
    assert(data.size() == 5);
    assert(data.contains(9) && !data.contains(10));
    assert(data.kth_allowed(6, 10) == 17);
    assert(data.kth_allowed(1, 1) == 4);
}
