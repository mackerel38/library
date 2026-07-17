#include "structure/dsu.hpp"

int main() {
    poe::paritydsu parity(4);
    assert(parity.merge(0, 1, 1));
    assert(parity.merge(1, 2, 1));
    assert(parity.minimum_color_size() == 1);
    assert(parity.merge(2, 3, 1));
    assert(parity.minimum_color_size() == 2);
    assert(!parity.merge(0, 2, 1));
    assert(!parity.consistent() && parity.minimum_color_size() == -1);

    poe::weighted_dsu<long long> weighted(5);
    assert(weighted.merge(0, 1, 4));
    assert(weighted.merge(1, 2, -7));
    assert(weighted.same(0, 2));
    assert(weighted.diff(0, 2) == -3);
    assert(weighted.diff(2, 0) == 3);
    assert(weighted.merge(0, 2, -3));
    assert(!weighted.merge(0, 2, 10));
    assert(weighted.size(1) == 3 && weighted.count() == 3);

    poe::rollbackdsu rollback(5);
    const int initial = rollback.snapshot();
    assert(rollback.merge(0, 1));
    const int after_first = rollback.snapshot();
    assert(rollback.merge(1, 2));
    assert(!rollback.merge(0, 2));
    assert(rollback.size(0) == 3 && rollback.count() == 3);
    rollback.undo();
    assert(rollback.same(0, 2));
    rollback.rollback(after_first);
    assert(rollback.same(0, 1) && !rollback.same(0, 2));
    rollback.rollback(initial);
    assert(rollback.count() == 5);
}
