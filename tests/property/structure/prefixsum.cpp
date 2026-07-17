#include "structure/prefixsum.hpp"

int main() {
    std::mt19937 random(712367);
    for (int trial = 0; trial < 200; ++trial) {
        const int n = 1 + random() % 30;
        poe::differencearray<int> difference(n);
        std::vector<int> brute(n);
        for (int query = 0; query < 50; ++query) {
            int left = random() % (n + 1);
            int right = random() % (n + 1);
            if (left > right) std::swap(left, right);
            const int value = static_cast<int>(random() % 21) - 10;
            difference.add(left, right, value);
            for (int i = left; i < right; ++i) brute[i] += value;
        }
        difference.build();
        assert(difference.values() == brute);
    }

    for (int trial = 0; trial < 200; ++trial) {
        const int height = 1 + random() % 12, width = 1 + random() % 12;
        std::vector initial(height, std::vector<int>(width));
        for (auto& row : initial) {
            for (int& value : row) value = static_cast<int>(random() % 21) - 10;
        }
        poe::differencearray2d<int> difference(initial);
        std::vector brute = initial;
        assert(difference.values() == initial);
        for (int query = 0; query < 80; ++query) {
            int top = random() % (height + 1), bottom = random() % (height + 1);
            int left = random() % (width + 1), right = random() % (width + 1);
            if (top > bottom) std::swap(top, bottom);
            if (left > right) std::swap(left, right);
            const int value = static_cast<int>(random() % 21) - 10;
            difference.add(top, left, bottom, right, value);
            for (int row = top; row < bottom; ++row)
                for (int column = left; column < right; ++column) brute[row][column] += value;
        }
        difference.build();
        assert(difference.values() == brute);
    }

    for (int n = 0; n <= 80; ++n) {
        std::vector<long long> values(n);
        for (auto& value : values) value = static_cast<int>(random() % 201) - 100;
        poe::prefixsum<long long> sums(values);
        for (int left = 0; left <= n; ++left) {
            for (int right = left; right <= n; ++right) {
                const long long expected =
                    std::accumulate(values.begin() + left, values.begin() + right, 0LL);
                assert(sums.sum(left, right) == expected);
            }
        }
    }

    for (int n = 1; n <= 80; ++n) {
        std::vector<long long> values(n);
        for (auto& value : values) value = static_cast<int>(random() % 201) - 100;
        poe::circularprefixsum<long long> sums(values);
        int offset = 0;
        for (int query = 0; query < 300; ++query) {
            if (random() % 3 == 0) {
                const long long shift = static_cast<int>(random() % 401) - 200;
                sums.rotate(shift);
                offset = static_cast<int>((offset + shift) % n);
                if (offset < 0) offset += n;
                assert(sums.offset() == offset);
            } else {
                int left = random() % (n + 1), right = random() % (n + 1);
                if (left > right) std::swap(left, right);
                long long expected = 0;
                for (int i = left; i < right; ++i) {
                    expected += values[(offset + i) % n];
                    assert(sums[i] == values[(offset + i) % n]);
                }
                assert(sums.sum(left, right) == expected);
            }
        }
    }

    for (int n = 0; n <= 60; ++n) {
        std::vector<long long> values(n);
        for (auto& value : values) value = static_cast<int>(random() % 201) - 100;
        poe::subarrayprefixsum<long long> sums(values);
        for (int left = 0; left <= n; ++left) {
            for (int right = left; right <= n; ++right) {
                long long expected = 0;
                for (int begin = left; begin < right; ++begin) {
                    long long current = 0;
                    for (int end = begin; end < right; ++end) {
                        current += values[end];
                        expected += current;
                    }
                }
                assert(sums.sum(left, right) == expected);
            }
        }
    }

    for (int height = 0; height <= 8; ++height) {
        for (int width = 0; width <= 8; ++width) {
            if (height == 0 && width != 0) continue;
            std::vector<std::vector<int>> grid(height, std::vector<int>(width));
            for (auto& row : grid) {
                for (int& value : row) value = static_cast<int>(random() % 21) - 10;
            }
            poe::prefixsum2d<int> sums(grid);
            for (int top = 0; top <= height; ++top) {
                for (int bottom = top; bottom <= height; ++bottom) {
                    for (int left = 0; left <= width; ++left) {
                        for (int right = left; right <= width; ++right) {
                            int expected = 0;
                            for (int row = top; row < bottom; ++row) {
                                for (int column = left; column < right; ++column) {
                                    expected += grid[row][column];
                                }
                            }
                            assert(sums.sum(top, left, bottom, right) == expected);
                        }
                    }
                }
            }
        }
    }

    for (int dimensions = 1; dimensions <= 6; ++dimensions) {
        for (int repetition = 0; repetition < 300; ++repetition) {
            std::vector<int> shape(dimensions);
            int size = 1;
            for (int& extent : shape) {
                extent = 1 + random() % 4;
                size *= extent;
            }
            std::vector<int> values(size);
            for (int& value : values) value = static_cast<int>(random() % 21) - 10;
            poe::multidimensionalprefixsum<int> sums(shape, values);
            std::vector<int> lower(dimensions), upper(dimensions);
            for (int dimension = 0; dimension < dimensions; ++dimension) {
                lower[dimension] = random() % (shape[dimension] + 1);
                upper[dimension] = random() % (shape[dimension] + 1);
                if (lower[dimension] > upper[dimension]) {
                    std::swap(lower[dimension], upper[dimension]);
                }
            }
            int expected = 0;
            for (int index = 0; index < size; ++index) {
                int remaining = index;
                bool inside = true;
                for (int dimension = dimensions - 1; dimension >= 0; --dimension) {
                    const int coordinate = remaining % shape[dimension];
                    remaining /= shape[dimension];
                    inside &= lower[dimension] <= coordinate && coordinate < upper[dimension];
                }
                if (inside) expected += values[index];
            }
            assert(sums.sum(lower, upper) == expected);
        }
    }
}
