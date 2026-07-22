#pragma once
#include <bits/stdc++.h>
#include "math/convolution.hpp"

namespace poe {

/// relaxedconvolution conv(n); conv.append(a[i],b[i])で積のi次係数を返す。
/// 全n回でO(M(n) log n)、各append時点ではそれ以前の係数だけを使う。
template<int mod>
struct relaxedconvolution {
    using mint = staticmodint<mod>;

    /// O(n)。係数をsize個追加できる空のonline畳み込みを作る。
    explicit relaxedconvolution(int size)
        : size_(size), index_(0), left_(size), right_(size), product_(size) {
        assert(size >= 0 && (mod - 1) % std::bit_ceil(
            static_cast<unsigned>(std::max(1, size))
        ) == 0);
    }

    /// 全呼び出しでO(M(n) log n)。次の二係数を追加して積の同じ次数の係数を返す。
    mint append(mint left_value, mint right_value) {
        assert(index_ < size_);
        const int current = index_;
        left_[current] = left_value;
        right_[current] = right_value;
        product_[current] += left_[current] * right_[0];
        if (current > 0) product_[current] += right_[current] * left_[0];

        ++index_;
        if (index_ >= size_) return product_[current];
        for (int block = 1, level = 0; block <= index_; block <<= 1, ++level) {
            if (index_ % (2 * block) != block) continue;
            std::vector<mint> transformed_left(2 * block);
            std::vector<mint> transformed_right(2 * block);
            if (index_ == block) {
                std::copy_n(left_.begin(), block, transformed_left.begin());
                std::copy_n(right_.begin(), block, transformed_right.begin());
                detail::ntt(transformed_left, false);
                detail::ntt(transformed_right, false);
                for (int i = 0; i < 2 * block; ++i) {
                    transformed_left[i] *= transformed_right[i];
                }
            } else {
                prepare_prefix(level, block);
                std::copy_n(
                    left_.begin() + index_ - block, block, transformed_left.begin()
                );
                std::copy_n(
                    right_.begin() + index_ - block, block, transformed_right.begin()
                );
                detail::ntt(transformed_left, false);
                detail::ntt(transformed_right, false);
                for (int i = 0; i < 2 * block; ++i) {
                    transformed_left[i] =
                        transformed_left[i] * prefix_right_[level][i]
                        + transformed_right[i] * prefix_left_[level][i];
                }
            }
            detail::ntt(transformed_left, true);
            const int end = std::min(index_ + block, size_);
            for (int degree = index_; degree < end; ++degree) {
                product_[degree] += transformed_left[block + degree - index_];
            }
        }
        return product_[current];
    }

private:
    void prepare_prefix(int level, int block) {
        if (static_cast<int>(prefix_left_.size()) <= level) {
            prefix_left_.resize(level + 1);
            prefix_right_.resize(level + 1);
        }
        if (!prefix_left_[level].empty()) return;
        prefix_left_[level].assign(left_.begin(), left_.begin() + 2 * block);
        prefix_right_[level].assign(right_.begin(), right_.begin() + 2 * block);
        detail::ntt(prefix_left_[level], false);
        detail::ntt(prefix_right_[level], false);
    }

    int size_;
    int index_;
    std::vector<mint> left_;
    std::vector<mint> right_;
    std::vector<mint> product_;
    std::vector<std::vector<mint>> prefix_left_;
    std::vector<std::vector<mint>> prefix_right_;
};

/// O(M(n) log n)。online_convolution(source,kernel,decide): 左から確定する畳み込みDPを返す。
/// raw[i]=source[i]+sum_{j<i} committed[j]*kernel[i-j]を受け、decide(i,raw[i])が確定値を返す。
template<int mod, class Decide>
std::vector<staticmodint<mod>> online_convolution(
    std::vector<staticmodint<mod>> source,
    const std::vector<staticmodint<mod>>& kernel,
    Decide decide
) {
    using mint = staticmodint<mod>;
    const int size = static_cast<int>(source.size());
    assert(static_cast<int>(kernel.size()) >= size);
    std::vector<mint> committed(size);
    auto solve = [&](auto&& self, int left, int right) -> void {
        if (right - left == 1) {
            committed[left] = decide(left, source[left]);
            return;
        }
        const int middle = (left + right) / 2;
        self(self, left, middle);
        std::vector<mint> fixed(committed.begin() + left, committed.begin() + middle);
        std::vector<mint> transition(kernel.begin() + 1, kernel.begin() + (right - left));
        const auto product = convolution(std::move(fixed), std::move(transition));
        for (int index = middle; index < right; ++index) {
            const int product_index = index - left - 1;
            if (product_index < static_cast<int>(product.size())) source[index] += product[product_index];
        }
        self(self, middle, right);
    };
    if (size > 0) solve(solve, 0, size);
    return committed;
}

/// O(M(n) log n)。convolution_recurrence(source,kernel): f=source+f*kernel、kernel[0]=0を解く。
template<int mod>
std::vector<staticmodint<mod>> convolution_recurrence(
    std::vector<staticmodint<mod>> source,
    const std::vector<staticmodint<mod>>& kernel
) {
    return online_convolution<mod>(std::move(source), kernel, [](int, auto value) { return value; });
}

/// O(M(n) log n)。a[i]=[x^i]base(x)prod_{start<=j<i}(1+a[j]x)で定まる係数列を返す。
template<int mod>
std::vector<staticmodint<mod>> online_factor_product_coefficients(
    std::vector<staticmodint<mod>> base,
    int start
) {
    using mint = staticmodint<mod>;
    const int size = static_cast<int>(base.size());
    assert(0 <= start && start <= size);
    if (start == size) return base;

    std::vector<mint> result = base;
    auto solve = [&](auto&& self, int left, int right, std::vector<mint> current)
        -> std::vector<mint> {
        assert(static_cast<int>(current.size()) == right - left);
        if (right - left == 1) {
            result[left] = current[0];
            return {mint{1}, result[left]};
        }
        const int middle = (left + right) / 2;
        std::vector<mint> left_current(
            current.begin(), current.begin() + (middle - left));
        auto left_product = self(self, left, middle, std::move(left_current));

        const auto updated = convolution(current, left_product);
        std::vector<mint> right_current(right - middle);
        for (int degree = middle; degree < right; ++degree) {
            right_current[degree - middle] = updated[degree - left];
        }
        auto right_product = self(
            self, middle, right, std::move(right_current));
        auto product = convolution(std::move(left_product), std::move(right_product));
        product.resize(right - left + 1);
        return product;
    };

    std::vector<mint> current(base.begin() + start, base.end());
    solve(solve, start, size, std::move(current));
    return result;
}

}
