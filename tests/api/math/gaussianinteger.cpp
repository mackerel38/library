#include <cassert>
#include "math/gaussianinteger.hpp"

using namespace poe;

int main() {
    const gaussianinteger<long long> value{1, 2};
    assert(value.norm() == 5);
    assert((value.conjugate() == gaussianinteger<long long>{1, -2}));
    assert((value * value == gaussianinteger<long long>{-3, 4}));
    assert((gaussian_power(value, 3) == gaussianinteger<long long>{-11, -2}));
    assert((gaussian_mod(gaussianinteger<long long>{-11, -2}, 5LL) ==
            gaussianinteger<long long>{4, 3}));
}
