#include "dp/intervalmaximum.hpp"

using namespace std;
using namespace poe;

int main() {
    mt19937 random(262);
    for (int iteration = 0; iteration < 5000; ++iteration) {
        const int n = 1 + static_cast<int>(random() % 7);
        const int maximum = static_cast<int>(random() % 4);
        const int q = static_cast<int>(random() % 9);
        vector<intervalmaximumconstraint> constraints;
        for (int query = 0; query < q; ++query) {
            int left = static_cast<int>(random() % n);
            int right = static_cast<int>(random() % n);
            if (right < left) swap(left, right);
            constraints.push_back({left, right + 1,
                                   static_cast<int>(random() % (maximum + 1))});
        }

        modint998244353 expected = 0;
        vector<int> values(n);
        const auto enumerate = [&](auto&& self, int index) -> void {
            if (index == n) {
                for (const auto& constraint : constraints) {
                    if (*max_element(values.begin() + constraint.left,
                                     values.begin() + constraint.right) != constraint.maximum) {
                        return;
                    }
                }
                ++expected;
                return;
            }
            for (values[index] = 0; values[index] <= maximum; ++values[index]) {
                self(self, index + 1);
            }
        };
        enumerate(enumerate, 0);
        assert(count_interval_maximum_sequences<998244353>(n, maximum, constraints) == expected);
    }
}
