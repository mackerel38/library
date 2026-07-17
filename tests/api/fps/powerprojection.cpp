#include "fps/powerprojection.hpp"

int main() {
    using fps = poe::fps998244353;
    const auto result = poe::power_projection(fps{0, 1, 1}, fps{1, 2, 3});
    assert(result.size() == 3);
    assert(result[0] == 3);
    assert(result[1] == 3);
    assert(result[2] == 1);
}
