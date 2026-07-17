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
    std::mt19937 random(1234567);
    for (int n = 1; n <= 40; ++n) {
        std::vector<long long> values(n);
        std::vector<range_value> nodes(n, {0, 1});
        poe::segtree<long long, sum_op, sum_e> seg(values);
        poe::lazysegtree<range_value, range_op, range_e, long long, range_mapping,
                         range_composition, range_id>
            lazy(nodes);
        for (int step = 0; step < 2000; ++step) {
            const int type = random() % 4;
            const int index = random() % n;
            int left = random() % (n + 1);
            int right = random() % (n + 1);
            if (left > right) {
                std::swap(left, right);
            }
            if (type == 0) {
                const long long value = static_cast<int>(random() % 101) - 50;
                values[index] = value;
                seg.set(index, value);
                lazy.set(index, {value, 1});
            } else if (type == 1) {
                const long long add = static_cast<int>(random() % 21) - 10;
                for (int i = left; i < right; ++i) {
                    values[i] += add;
                    seg.set(i, values[i]);
                }
                lazy.apply(left, right, add);
            } else {
                const long long expected =
                    std::accumulate(values.begin() + left, values.begin() + right, 0LL);
                assert(seg.prod(left, right) == expected);
                assert(lazy.prod(left, right).sum == expected);
                assert(seg[index] == values[index]);
                assert(lazy[index].sum == values[index]);
            }
        }
    }

    for (int n = 1; n <= 80; ++n) {
        poe::dsu uf(n);
        int expected_count = n;
        std::vector<int> component(n);
        std::iota(component.begin(), component.end(), 0);
        for (int step = 0; step < 1000; ++step) {
            const int a = random() % n;
            const int b = random() % n;
            if (random() % 2 == 0) {
                const bool was_same = component[a] == component[b];
                const int from = component[b];
                const int to = component[a];
                uf.merge(a, b);
                if (!was_same) --expected_count;
                for (int& value : component) {
                    if (value == from) {
                        value = to;
                    }
                }
            } else {
                assert(uf.same(a, b) == (component[a] == component[b]));
            }
            assert(uf.count() == expected_count);
        }
    }
}
