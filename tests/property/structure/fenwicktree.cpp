#include "structure/fenwicktree.hpp"

int main() {
    std::mt19937_64 random(0x5eed);

    for (int size = 0; size <= 64; ++size) {
        poe::fenwicktree<long long> actual(size);
        std::vector<long long> expected(size);

        for (int operation = 0; operation < 2000; ++operation) {
            if (size > 0 && random() % 3 != 0) {
                const int index = static_cast<int>(random() % size);
                const long long value = static_cast<long long>(random() % 201) - 100;
                if (random() % 2 == 0) {
                    actual.add(index, value);
                    expected[index] += value;
                } else {
                    actual.set(index, value);
                    expected[index] = value;
                }
                if (actual[index] != expected[index]) {
                    std::cerr << "point mismatch: size=" << size
                              << " operation=" << operation << " index=" << index
                              << '\n';
                    return 1;
                }
                continue;
            }

            int left = static_cast<int>(random() % (size + 1));
            int right = static_cast<int>(random() % (size + 1));
            if (left > right) {
                std::swap(left, right);
            }
            const long long expected_sum =
                std::accumulate(expected.begin() + left, expected.begin() + right, 0LL);
            if (actual.sum(left, right) != expected_sum) {
                std::cerr << "mismatch: size=" << size << " operation=" << operation
                          << " range=[" << left << ", " << right << ")\n";
                return 1;
            }
        }
    }

    for (int size = 0; size <= 64; ++size) {
        std::vector<long long> values(size);
        for (long long& value : values) value = static_cast<long long>(random() % 201) - 100;
        poe::fenwicktree<long long> actual(values);
        assert(actual.sum() == std::accumulate(values.begin(), values.end(), 0LL));
        for (int i = 0; i < size; ++i) assert(actual[i] == values[i]);
    }

    for (int size = 0; size <= 64; ++size) {
        poe::fenwicktree<int> actual(size);
        std::vector<int> expected(size);
        for (int index = 0; index < size; ++index) {
            expected[index] = static_cast<int>(random() % 6);
            actual.set(index, expected[index]);
        }
        const int total = std::accumulate(expected.begin(), expected.end(), 0);
        for (int value = -1; value <= total + 1; ++value) {
            int expected_lower = 0;
            while (expected_lower < size &&
                   std::accumulate(expected.begin(), expected.begin() + expected_lower + 1,
                                   0) < value) {
                ++expected_lower;
            }
            int expected_upper = 0;
            while (expected_upper < size &&
                   std::accumulate(expected.begin(), expected.begin() + expected_upper + 1,
                                   0) <= value) {
                ++expected_upper;
            }
            if (actual.lower_bound(value) != expected_lower ||
                actual.upper_bound(value) != expected_upper) {
                std::cerr << "bound mismatch: size=" << size << " value=" << value
                          << '\n';
                return 1;
            }
        }
    }
}
