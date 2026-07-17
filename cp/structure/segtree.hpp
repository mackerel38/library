#pragma once
#include <bits/stdc++.h>

namespace poe {

/// モノイドの区間積を管理する: segtree<S, op, e> seg(values); 各操作O(log n)。
template <class S, auto op, auto e>
struct segtree {
    /// O(n)。n要素をe()で初期化する。
    explicit segtree(int n) : segtree(std::vector<S>(n, e())) {
        assert(n >= 0);
    }

    /// O(n)。valuesから構築する。
    explicit segtree(const std::vector<S>& values)
        : n_(static_cast<int>(values.size())) {
        while (size_ < n_) {
            size_ <<= 1;
            ++log_;
        }
        data_.assign(2 * size_, e());
        std::copy(values.begin(), values.end(), data_.begin() + size_);
        for (int i = size_ - 1; i >= 1; --i) {
            update(i);
        }
    }

    /// O(1)。要素数を返す。
    int size() const noexcept {
        return n_;
    }

    /// O(log n)。a[index]をvalueへ置き換える。
    void set(int index, const S& value) {
        assert_index(index);
        int node = index + size_;
        data_[node] = value;
        for (int i = 1; i <= log_; ++i) {
            update(node >> i);
        }
    }

    /// O(1)。a[index]を返す。
    S get(int index) const {
        assert_index(index);
        return data_[index + size_];
    }

    /// O(1)。seg[index]はa[index]を返す。
    S operator[](int index) const {
        return get(index);
    }

    /// O(log n)。半開区間[left, right)の積を返す。
    S prod(int left, int right) const {
        assert(0 <= left && left <= right && right <= n_);
        S result_left = e();
        S result_right = e();
        left += size_;
        right += size_;
        while (left < right) {
            if (left & 1) {
                result_left = op(result_left, data_[left++]);
            }
            if (right & 1) {
                result_right = op(data_[--right], result_right);
            }
            left >>= 1;
            right >>= 1;
        }
        return op(result_left, result_right);
    }

    /// O(1)。全要素の積を返す。
    S all_prod() const {
        return data_[1];
    }

    /// O(log n)。g(prod(left, right))がtrueとなる最大のrightを返す。g(e())はtrue。
    template <class G>
    int max_right(int left, G g) const {
        assert(0 <= left && left <= n_);
        assert(g(e()));
        if (left == n_) {
            return n_;
        }
        int node = left + size_;
        S accumulated = e();
        do {
            while ((node & 1) == 0) {
                node >>= 1;
            }
            if (!g(op(accumulated, data_[node]))) {
                while (node < size_) {
                    node <<= 1;
                    S candidate = op(accumulated, data_[node]);
                    if (g(candidate)) {
                        accumulated = candidate;
                        ++node;
                    }
                }
                return node - size_;
            }
            accumulated = op(accumulated, data_[node]);
            ++node;
        } while ((node & -node) != node);
        return n_;
    }

    /// O(log n)。g(prod(left, right))がtrueとなる最小のleftを返す。g(e())はtrue。
    template <class G>
    int min_left(int right, G g) const {
        assert(0 <= right && right <= n_);
        assert(g(e()));
        if (right == 0) {
            return 0;
        }
        int node = right + size_;
        S accumulated = e();
        do {
            --node;
            while (node > 1 && (node & 1)) {
                node >>= 1;
            }
            if (!g(op(data_[node], accumulated))) {
                while (node < size_) {
                    node = 2 * node + 1;
                    S candidate = op(data_[node], accumulated);
                    if (g(candidate)) {
                        accumulated = candidate;
                        --node;
                    }
                }
                return node + 1 - size_;
            }
            accumulated = op(data_[node], accumulated);
        } while ((node & -node) != node);
        return 0;
    }

private:
    /// O(1)。nodeを子から再計算する。
    void update(int node) {
        data_[node] = op(data_[2 * node], data_[2 * node + 1]);
    }

    /// O(1)。添字が範囲内であることを確認する。
    void assert_index(int index) const {
        assert(0 <= index && index < n_);
    }

    int n_ = 0;
    int size_ = 1;
    int log_ = 0;
    std::vector<S> data_;
};

}
