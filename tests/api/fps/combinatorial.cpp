#include <cassert>

#include "fps/combinatorial.hpp"

using namespace std;
using namespace poe;

int main() {
    const auto answer = divisible_group_counts<998244353>(3, {1, 2, 3});
    assert(answer.ordinary == 3);
    assert(answer.labeled == 5);
    const auto empty = divisible_group_counts<998244353>(0, {});
    assert(empty.ordinary == 1 && empty.labeled == 1);
    const auto impossible = divisible_group_counts<998244353>(1, {});
    assert(impossible.ordinary == 0 && impossible.labeled == 0);

    assert(labeled_color_change_counts<998244353>({2, 1})
           == fps998244353({0, 4, 2}));
    assert(labeled_color_change_counts<998244353>({3})
           == fps998244353({6, 0, 0}));
    assert(labeled_color_change_counts<998244353>({})
           == fps998244353({1}));
}
