#include "structure/dsu.hpp"
#include "structure/lazysegtree.hpp"
#include "structure/segtree.hpp"

long long sum_op(long long a, long long b) {
    return a + b;
}

long long sum_e() {
    return 0;
}

struct range_value {
    long long sum;
    int size;
};

range_value range_op(range_value a, range_value b) {
    return {a.sum + b.sum, a.size + b.size};
}

range_value range_e() {
    return {0, 0};
}

range_value range_mapping(long long add, range_value value) {
    return {value.sum + add * value.size, value.size};
}

long long range_composition(long long newer, long long older) {
    return newer + older;
}

long long range_id() {
    return 0;
}

int main() {
    poe::dsu uf(5);
    assert(!uf.same(0, 1));
    uf.merge(0, 1);
    uf.merge(1, 2);
    assert(uf.same(0, 2));
    assert(uf.size(1) == 3);
    assert(uf[2] == uf.leader(0));
    assert(uf.groups().size() == 3);

    poe::segtree<long long, sum_op, sum_e> seg({1, 2, 3, 4});
    assert(seg.size() == 4);
    assert(seg[2] == 3);
    assert(seg.prod(1, 4) == 9);
    seg.set(2, 10);
    assert(seg.all_prod() == 17);
    assert(seg.max_right(0, [](long long x) { return x <= 3; }) == 2);
    assert(seg.min_left(4, [](long long x) { return x <= 14; }) == 2);

    std::vector<range_value> initial = {{1, 1}, {2, 1}, {3, 1}, {4, 1}};
    poe::lazysegtree<range_value, range_op, range_e, long long, range_mapping,
                     range_composition, range_id>
        lazy(initial);
    lazy.apply(1, 4, 5);
    assert(lazy.prod(0, 4).sum == 25);
    assert(lazy[2].sum == 8);
    lazy.set(2, {20, 1});
    assert(lazy.prod(1, 4).sum == 36);
    lazy.apply(0, -1);
    assert(lazy.get(0).sum == 0);
    assert(lazy.max_right(0, [](range_value x) { return x.sum <= 8; }) == 2);
    assert(lazy.min_left(4, [](range_value x) { return x.sum <= 31; }) == 2);
}
