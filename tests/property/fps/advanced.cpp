#include <cassert>
#include <random>

#include "fps/online.hpp"
#include "fps/rational.hpp"

int main() {
    using mint = poe::modint998244353;
    std::mt19937 random(20260717);
    for (int test = 0; test < 100; ++test) {
        const int size = 1 + random() % 100;
        std::vector<mint> source(size), kernel(size), brute(size);
        std::vector<bool> blocked(size);
        for (auto& value : source) value = random() % 1000;
        for (int i = 1; i < size; ++i) kernel[i] = random() % 1000;
        for (int i = 0; i < size; ++i) blocked[i] = random() % 7 == 0;
        for (int i = 0; i < size; ++i) {
            brute[i] += source[i];
            for (int j = 0; j < i; ++j) brute[i] += brute[j] * kernel[i - j];
            if (blocked[i]) brute[i] = 0;
        }
        const auto fast = poe::online_convolution<998244353>(source, kernel,
            [&](int index, mint value) { return blocked[index] ? mint{} : value; });
        assert(fast == brute);

        poe::relaxedconvolution<998244353> relaxed(size);
        std::vector<mint> left(size), right(size);
        for (int i = 0; i < size; ++i) {
            left[i] = random() % 1000;
            right[i] = random() % 1000;
            mint expected{};
            for (int j = 0; j <= i; ++j) expected += left[j] * right[i - j];
            assert(relaxed.append(left[i], right[i]) == expected);
        }

        const int count = 1 + random() % 30;
        std::vector<mint> values(count);
        for (auto& value : values) value = random() % 1000;
        const auto sums = poe::power_sums<998244353>(values, 40);
        for (int exponent = 0; exponent < 40; ++exponent) {
            mint expected{};
            for (mint value : values) expected += value.pow(exponent);
            assert(sums[exponent] == expected);
        }

        const int variable_count = random() % 6;
        std::vector<poe::weightedsumvariable> variables(variable_count);
        for (auto& [weight, limit] : variables) {
            weight = random() % 5 + 1;
            limit = random() % 3 == 0 ? -1 : static_cast<long long>(random() % 5);
        }
        for (int target = 0; target < 35; ++target) {
            mint expected = 0;
            auto enumerate = [&](auto&& self, int index, int sum) -> void {
                if (index == variable_count) {
                    expected += sum == target;
                    return;
                }
                const auto [weight, limit] = variables[index];
                const long long upper = limit < 0 ? target / weight : limit;
                for (long long value = 0; value <= upper && sum + value * weight <= target; ++value) {
                    self(self, index + 1, sum + static_cast<int>(value * weight));
                }
            };
            enumerate(enumerate, 0, 0);
            assert(poe::count_weighted_sum<998244353>(target, variables) == expected);
        }
    }
}
