#include "algorithm/quadraticselection.hpp"

using namespace std;
using namespace poe;

int main() {
    const __int128_t span = static_cast<__int128_t>(numeric_limits<unsigned long long>::max());
    const detail::quadraticselectionevent above_one{span, span - 1, 0, 1};
    const detail::quadraticselectionevent below_one{span - 1, span, 0, 2};
    assert(detail::quadraticselection_compare_event(above_one, below_one) > 0);
    assert(detail::quadraticselection_compare_event(
        {-span, span - 1, 0, 1}, {-span + 1, span, 0, 2}
    ) < 0);

    mt19937 random(2571);
    for (int iteration = 0; iteration < 5000; ++iteration) {
        const int n = 1 + random() % 11;
        const int k = random() % (n + 1);
        vector<pair<long long, long long>> points(n);
        for (auto& [x, y] : points) {
            x = static_cast<int>(random() % 13) - 6;
            y = static_cast<int>(random() % 17) - 8;
        }

        __int128_t expected = -((__int128_t)1 << 120);
        for (int subset = 0; subset < (1 << n); ++subset) {
            if (popcount(static_cast<unsigned>(subset)) != k) continue;
            __int128_t x_sum = 0, y_sum = 0;
            for (int index = 0; index < n; ++index) {
                if (subset >> index & 1) {
                    x_sum += points[index].first;
                    y_sum += points[index].second;
                }
            }
            expected = max(expected, x_sum * x_sum + y_sum);
        }
        assert(maximum_quadratic_k_selection(points, k) == expected);
    }
}
