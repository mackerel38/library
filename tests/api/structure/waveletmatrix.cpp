#include "structure/waveletmatrix.hpp"

int main() {
    poe::waveletmatrix<int, long long> wm({5, -2, 5, 1, 8, -2});
    assert(wm.size() == 6 && wm[1] == -2);
    assert(wm.kth(0, 6, 0) == -2);
    assert(wm.kth(1, 5, 2) == 5);
    assert(wm.count_less(0, 6, 5) == 3);
    assert(wm.count_leq(0, 6, 5) == 5);
    assert(wm.count(0, 6, 5) == 2);
    assert(wm.count(0, 6, -2, 6) == 5);
    assert(wm.sum(0, 6) == 15);
    assert(wm.sum_less(0, 6, 5) == -3);
    assert(wm.sum_leq(0, 6, 5) == 7);
    assert(wm.sum(0, 6, -2, 6) == 7);
    assert(wm.prev(0, 6, 5) == 1);
    assert(wm.next(0, 6, 2) == 5);
    assert(!wm.prev(0, 6, -2));
    assert(!wm.next(0, 6, 9));
    assert(wm.sum(3, 3) == 0 && wm.count_less(3, 3, 100) == 0);
}
