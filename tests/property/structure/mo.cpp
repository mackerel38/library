#include "structure/mo.hpp"

int main() {
    std::mt19937 random(614893);
    for (int trial = 0; trial < 300; ++trial) {
        const int n = random() % 100;
        const int q = random() % 200;
        std::vector<long long> values(n);
        for (auto& value : values) value = static_cast<int>(random() % 201) - 100;
        poe::Mo mo(n);
        std::vector<long long> expected;
        for (int query = 0; query < q; ++query) {
            int left = random() % (n + 1);
            int right = random() % (n + 1);
            if (left > right) std::swap(left, right);
            mo.add(left, right);
            expected.push_back(
                std::accumulate(values.begin() + left, values.begin() + right, 0LL));
        }
        std::vector<long long> actual(q);
        long long sum = 0;
        mo.run([&](int index) { sum += values[index]; },
               [&](int index) { sum -= values[index]; },
               [&](int id) { actual[id] = sum; });
        assert(actual == expected);
    }
}
