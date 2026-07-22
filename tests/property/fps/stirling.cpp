#include "fps/stirling.hpp"

int main() {
    using mint = poe::staticmodint<998244353>;
    std::vector<mint> first{1};
    std::vector<mint> second{1};
    for (int n = 0; n <= 500; ++n) {
        assert(poe::signed_stirling_first_row<998244353>(n) == first);
        assert(poe::stirling_second_row<998244353>(n) == second);
        std::vector<mint> next_first(static_cast<std::size_t>(n) + 2);
        std::vector<mint> next_second(static_cast<std::size_t>(n) + 2);
        for (int k = 0; k <= n + 1; ++k) {
            if (k > 0) {
                next_first[k] += first[k - 1];
                next_second[k] += second[k - 1];
            }
            if (k <= n) {
                next_first[k] -= first[k] * n;
                next_second[k] += second[k] * k;
            }
        }
        first.swap(next_first);
        second.swap(next_second);
    }
}
