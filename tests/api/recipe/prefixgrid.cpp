#include "recipe/prefixgrid.hpp"

using namespace std;
using namespace poe;

int main() {
    assert(count_row_column_prefix_union<998244353>({"0?1", "?1?"}) == 6);
    assert(count_row_column_prefix_union<998244353>({"101", "010", "101"}) == 0);
    assert(count_row_column_prefix_union<998244353>({"?"}) == 2);
}
