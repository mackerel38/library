#include "math/modint.hpp"
#include "utility/presetsegtree.hpp"

long long sum_of(const std::vector<long long>& values, int left, int right) {
    return std::accumulate(values.begin() + left, values.begin() + right, 0LL);
}

long long gcd_of(const std::vector<long long>& values, int left, int right) {
    long long result = 0;
    for (int i = left; i < right; ++i) result = std::gcd(result, values[i]);
    return result;
}

long long inversion_of(const std::vector<int>& bits, int left, int right) {
    long long result = 0;
    for (int i = left; i < right; ++i)
        for (int j = i + 1; j < right; ++j)
            result += bits[i] == 1 && bits[j] == 0;
    return result;
}

int main() {
    std::mt19937 random(20260714);
    for (int n = 1; n <= 24; ++n) {
        std::vector<long long> initial(n);
        for (long long& value : initial) value = static_cast<int>(random() % 31) - 15;

        {
            auto values = initial;
            poe::point_add_range_sum<long long> sum(values);
            poe::point_affine_range_sum<long long> affine(values);
            poe::point_set_range_min<long long> min(values);
            poe::point_set_range_max<long long> max(values);
            poe::point_set_range_gcd<long long> gcd(values);
            poe::point_set_prefix_sum_max<long long> prefix(values);
            for (int step = 0; step < 800; ++step) {
                const int index = random() % n;
                const int operation = random() % 3;
                if (operation == 0) {
                    const long long value = static_cast<int>(random() % 41) - 20;
                    values[index] = value;
                    sum.set(index, value);
                    affine.set(index, value);
                    min.set(index, value);
                    max.set(index, value);
                    gcd.set(index, value);
                    prefix.set(index, value);
                } else if (operation == 1) {
                    const long long multiply = static_cast<int>(random() % 3) - 1;
                    const long long add = static_cast<int>(random() % 11) - 5;
                    values[index] = multiply * values[index] + add;
                    affine.affine(index, multiply, add);
                    sum.set(index, values[index]);
                    min.set(index, values[index]);
                    max.set(index, values[index]);
                    gcd.set(index, values[index]);
                    prefix.set(index, values[index]);
                } else {
                    int left = random() % (n + 1), right = random() % (n + 1);
                    if (left > right) std::swap(left, right);
                    assert(sum.sum(left, right) == sum_of(values, left, right));
                    assert(affine.sum(left, right) == sum_of(values, left, right));
                    if (left < right) {
                        assert(min.min(left, right) ==
                               *std::min_element(values.begin() + left, values.begin() + right));
                        assert(max.max(left, right) ==
                               *std::max_element(values.begin() + left, values.begin() + right));
                    }
                    assert(gcd.gcd(left, right) == gcd_of(values, left, right));
                    assert(sum[index] == values[index] && affine[index] == values[index]);
                    assert(prefix.sum(right) == sum_of(values, 0, right));
                    long long current = 0;
                    long long maximum = std::numeric_limits<long long>::lowest();
                    int first_nonnegative = n;
                    for (int i = 0; i < n; ++i) {
                        current += values[i];
                        maximum = std::max(maximum, current);
                        if (first_nonnegative == n && current >= 0) first_nonnegative = i;
                    }
                    assert(prefix.max_prefix() == maximum);
                    assert(prefix.first_prefix_at_least(0) == first_nonnegative);
                }
            }
        }

        {
            auto values = initial;
            poe::range_add_range_sum<long long> sum(values);
            poe::range_add_range_sum_square<long long> square(values);
            poe::range_add_range_min<long long> min(values);
            poe::range_add_range_max<long long> max(values);
            poe::range_add_range_gcd<long long> gcd(values);
            poe::range_add_point_get<long long> get(values);
            for (int step = 0; step < 800; ++step) {
                int left = random() % (n + 1), right = random() % (n + 1);
                if (left > right) std::swap(left, right);
                if (random() % 2 == 0) {
                    const long long delta = static_cast<int>(random() % 15) - 7;
                    for (int i = left; i < right; ++i) values[i] += delta;
                    sum.add(left, right, delta);
                    square.add(left, right, delta);
                    min.add(left, right, delta);
                    max.add(left, right, delta);
                    gcd.add(left, right, delta);
                    get.add(left, right, delta);
                } else {
                    assert(sum.sum(left, right) == sum_of(values, left, right));
                    assert(square.sum(left, right) == sum_of(values, left, right));
                    long long expected_square = 0;
                    for (int i = left; i < right; ++i) expected_square += values[i] * values[i];
                    assert(square.square_sum(left, right) == expected_square);
                    assert(gcd.gcd(left, right) == gcd_of(values, left, right));
                    if (left < right) {
                        assert(min.min(left, right) ==
                               *std::min_element(values.begin() + left, values.begin() + right));
                        assert(max.max(left, right) ==
                               *std::max_element(values.begin() + left, values.begin() + right));
                    }
                    const int index = random() % n;
                    assert(sum[index] == values[index] && min[index] == values[index]);
                    assert(square[index] == values[index]);
                    assert(max[index] == values[index] && gcd[index] == values[index]);
                    assert(get[index] == values[index]);
                }
            }
        }

        {
            auto values = initial;
            poe::range_set_range_sum<long long> sum(values);
            poe::range_set_range_min<long long> min(values);
            poe::range_set_range_max<long long> max(values);
            poe::range_set_range_gcd<long long> gcd(values);
            poe::range_set_point_get<long long> get(values);
            for (int step = 0; step < 800; ++step) {
                int left = random() % (n + 1), right = random() % (n + 1);
                if (left > right) std::swap(left, right);
                if (random() % 2 == 0) {
                    const long long value = static_cast<int>(random() % 31) - 15;
                    for (int i = left; i < right; ++i) values[i] = value;
                    sum.set(left, right, value);
                    min.set(left, right, value);
                    max.set(left, right, value);
                    gcd.set(left, right, value);
                    get.set(left, right, value);
                } else {
                    assert(sum.sum(left, right) == sum_of(values, left, right));
                    assert(gcd.gcd(left, right) == gcd_of(values, left, right));
                    if (left < right) {
                        assert(min.min(left, right) ==
                               *std::min_element(values.begin() + left, values.begin() + right));
                        assert(max.max(left, right) ==
                               *std::max_element(values.begin() + left, values.begin() + right));
                    }
                    const int index = random() % n;
                    assert(sum[index] == values[index] && min[index] == values[index]);
                    assert(max[index] == values[index] && gcd[index] == values[index]);
                    assert(get[index] == values[index]);
                }
            }
        }

        {
            auto values = initial;
            poe::range_affine_range_sum<long long> sum(values);
            poe::range_affine_point_get<long long> get(values);
            for (int step = 0; step < 500; ++step) {
                int left = random() % (n + 1), right = random() % (n + 1);
                if (left > right) std::swap(left, right);
                const long long multiply = static_cast<int>(random() % 3) - 1;
                const long long add = static_cast<int>(random() % 11) - 5;
                for (int i = left; i < right; ++i) values[i] = multiply * values[i] + add;
                sum.affine(left, right, multiply, add);
                get.affine(left, right, multiply, add);
                int query_left = random() % (n + 1), query_right = random() % (n + 1);
                if (query_left > query_right) std::swap(query_left, query_right);
                assert(sum.sum(query_left, query_right) ==
                       sum_of(values, query_left, query_right));
                const int index = random() % n;
                assert(sum[index] == values[index] && get[index] == values[index]);
            }
        }

        {
            std::vector<int> bits(n);
            for (int& bit : bits) bit = random() & 1;
            poe::range_flip_range_sum sum(bits);
            poe::range_flip_range_inversion inversion(bits);
            for (int step = 0; step < 500; ++step) {
                int left = random() % (n + 1), right = random() % (n + 1);
                if (left > right) std::swap(left, right);
                if (random() % 2 == 0) {
                    for (int i = left; i < right; ++i) bits[i] ^= 1;
                    sum.flip(left, right);
                    inversion.flip(left, right);
                } else {
                    assert(sum.sum(left, right) ==
                           std::accumulate(bits.begin() + left, bits.begin() + right, 0LL));
                    assert(inversion.inversion(left, right) == inversion_of(bits, left, right));
                    const int index = random() % n;
                    assert(sum[index] == bits[index] && inversion[index] == bits[index]);
                }
            }
        }

        {
            std::vector<long long> values(n);
            std::vector<int> active(n, 1);
            poe::range_active_add_range_clear_flip_max<long long> data(n);
            for (int step = 0; step < 700; ++step) {
                int left = random() % n, right = 1 + random() % n;
                if (left >= right) std::swap(left, right), ++right;
                const int operation = random() % 4;
                if (operation == 0) {
                    const long long delta = static_cast<int>(random() % 21) - 10;
                    for (int i = left; i < right; ++i) if (active[i]) values[i] += delta;
                    data.add(left, right, delta);
                } else if (operation == 1) {
                    for (int i = left; i < right; ++i) values[i] = 0, active[i] ^= 1;
                    data.clear_flip(left, right);
                } else if (operation == 2) {
                    const int index = random() % n;
                    const long long value = static_cast<int>(random() % 31) - 15;
                    values[index] = value;
                    active[index] = 1;
                    data.set(index, value);
                } else {
                    long long expected = std::numeric_limits<long long>::lowest();
                    for (int i = left; i < right; ++i) expected = std::max(expected, values[i]);
                    assert(data.max(left, right) == expected);
                    const int index = random() % n;
                    assert(data[index] == values[index]);
                    assert(data.active(index) == static_cast<bool>(active[index]));
                }
            }
        }

        {
            using mint = poe::modint998244353;
            using data_type = poe::point_set_range_composite<mint>;
            std::vector<data_type::affine> functions(n, {1, 0});
            data_type data(functions);
            for (int step = 0; step < 500; ++step) {
                const int index = random() % n;
                const mint multiply = random() % 10;
                const mint add = random() % 10;
                functions[index] = {multiply, add};
                data.set(index, multiply, add);
                int left = random() % (n + 1), right = random() % (n + 1);
                if (left > right) std::swap(left, right);
                mint expected = 7;
                for (int i = left; i < right; ++i) expected = functions[i](expected);
                assert(data.apply(left, right, mint{7}) == expected);
            }
        }

        {
            std::vector<long long> values(n);
            for (auto& value : values) value = random() % 1001;
            poe::range_divide_set_range_sum<long long> data(values);
            for (int step = 0; step < 800; ++step) {
                int left = random() % (n + 1), right = random() % (n + 1);
                if (left > right) std::swap(left, right);
                const int operation = random() % 4;
                if (operation == 0) {
                    const long long divisor = 2 + random() % 20;
                    for (int i = left; i < right; ++i) values[i] /= divisor;
                    data.divide(left, right, divisor);
                } else if (operation == 1) {
                    const long long value = random() % 1001;
                    for (int i = left; i < right; ++i) values[i] = value;
                    data.set(left, right, value);
                } else if (operation == 2) {
                    assert(data.sum(left, right) == sum_of(values, left, right));
                } else {
                    const int index = random() % n;
                    assert(data[index] == values[index]);
                }
                assert(data.sum() == sum_of(values, 0, n));
            }
        }

        {
            auto values = initial;
            poe::range_chmin_chmax_add_range_sum<long long> data(values);
            for (int step = 0; step < 1200; ++step) {
                int left = random() % (n + 1), right = random() % (n + 1);
                if (left > right) std::swap(left, right);
                const long long value = static_cast<int>(random() % 61) - 30;
                const int operation = random() % 6;
                if (operation == 0) {
                    for (int i = left; i < right; ++i) values[i] = std::min(values[i], value);
                    data.chmin(left, right, value);
                } else if (operation == 1) {
                    for (int i = left; i < right; ++i) values[i] = std::max(values[i], value);
                    data.chmax(left, right, value);
                } else if (operation == 2) {
                    for (int i = left; i < right; ++i) values[i] += value;
                    data.add(left, right, value);
                } else if (operation == 3) {
                    const int index = random() % n;
                    values[index] = value;
                    data.set(index, value);
                } else if (operation == 4) {
                    assert(data.sum(left, right) == sum_of(values, left, right));
                } else {
                    const int index = random() % n;
                    assert(data[index] == values[index]);
                }
                assert(data.sum() == sum_of(values, 0, n));
            }
        }

        {
            std::vector<long long> values(n);
            for (auto& value : values) value = random() % 31;
            poe::range_subtract_clamp_zero_sum<long long> data(values);
            for (int step = 0; step < 500; ++step) {
                int left = random() % (n + 1), right = random() % (n + 1);
                if (left > right) std::swap(left, right);
                const long long amount = random() % 21;
                long long expected = 0;
                for (int i = left; i < right; ++i) {
                    expected += std::min(values[i], amount);
                    values[i] = std::max(0LL, values[i] - amount);
                }
                assert(data.subtract(left, right, amount) == expected);
                for (int i = 0; i < n; ++i) assert(data[i] == values[i]);
                if (random() % 5 == 0) {
                    const int index = random() % n;
                    values[index] = random() % 31;
                    data.set(index, values[index]);
                }
            }
        }
    }
}
