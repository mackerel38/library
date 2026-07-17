#pragma once
#include <bits/stdc++.h>

namespace poe {

/// 区間作用と区間積を扱う: lazysegtree<S, op, e, F, mapping, composition, id> seg(v)。
template <class S, auto op, auto e, class F, auto mapping, auto composition, auto id>
struct lazysegtree {
    /// O(n)。n要素をe()で初期化する。
    explicit lazysegtree(int n) : lazysegtree(std::vector<S>(n, e())) {
        assert(n >= 0);
    }

    /// O(n)。valuesから構築する。
    explicit lazysegtree(const std::vector<S>& values)
        : n_(static_cast<int>(values.size())) {
        while (size_ < n_) {
            size_ <<= 1;
            ++log_;
        }
        data_.assign(2 * size_, e());
        lazy_.assign(size_, id());
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
        push_path(node);
        data_[node] = value;
        for (int i = 1; i <= log_; ++i) {
            update(node >> i);
        }
    }

    /// O(log n)。a[index]を返す。
    S get(int index) {
        assert_index(index);
        int node = index + size_;
        push_path(node);
        return data_[node];
    }

    /// O(log n)。seg[index]はa[index]を返す。
    S operator[](int index) {
        return get(index);
    }

    /// O(log n)。半開区間[left, right)の積を返す。
    S prod(int left, int right) {
        assert(0 <= left && left <= right && right <= n_);
        if (left == right) {
            return e();
        }
        left += size_;
        right += size_;
        push_range(left, right);
        S result_left = e();
        S result_right = e();
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

    /// O(log n)。a[index]へ作用fを適用する。
    void apply(int index, const F& f) {
        assert_index(index);
        int node = index + size_;
        push_path(node);
        data_[node] = mapping(f, data_[node]);
        for (int i = 1; i <= log_; ++i) {
            update(node >> i);
        }
    }

    /// O(log n)。半開区間[left, right)へ作用fを適用する。
    void apply(int left, int right, const F& f) {
        assert(0 <= left && left <= right && right <= n_);
        if (left == right) {
            return;
        }
        left += size_;
        right += size_;
        push_range(left, right);
        const int original_left = left;
        const int original_right = right;
        while (left < right) {
            if (left & 1) {
                all_apply(left++, f);
            }
            if (right & 1) {
                all_apply(--right, f);
            }
            left >>= 1;
            right >>= 1;
        }
        update_range(original_left, original_right);
    }

    /// O(log n)。g(prod(left, right))がtrueとなる最大のrightを返す。g(e())はtrue。
    template <class G>
    int max_right(int left, G g) {
        assert(0 <= left && left <= n_);
        assert(g(e()));
        if (left == n_) {
            return n_;
        }
        int node = left + size_;
        push_path(node);
        S accumulated = e();
        do {
            while ((node & 1) == 0) {
                node >>= 1;
            }
            if (!g(op(accumulated, data_[node]))) {
                while (node < size_) {
                    push(node);
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
    int min_left(int right, G g) {
        assert(0 <= right && right <= n_);
        assert(g(e()));
        if (right == 0) {
            return 0;
        }
        int node = right + size_;
        push_path(node - 1);
        S accumulated = e();
        do {
            --node;
            while (node > 1 && (node & 1)) {
                node >>= 1;
            }
            if (!g(op(data_[node], accumulated))) {
                while (node < size_) {
                    push(node);
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

    /// O(1)。nodeへ作用fを適用する。
    void all_apply(int node, const F& f) {
        data_[node] = mapping(f, data_[node]);
        if (node < size_) {
            lazy_[node] = composition(f, lazy_[node]);
        }
    }

    /// O(1)。nodeの遅延作用を子へ伝える。
    void push(int node) {
        all_apply(2 * node, lazy_[node]);
        all_apply(2 * node + 1, lazy_[node]);
        lazy_[node] = id();
    }

    /// O(log n)。nodeまでの遅延作用を伝える。
    void push_path(int node) {
        for (int i = log_; i >= 1; --i) {
            push(node >> i);
        }
    }

    /// O(log n)。区間端点までの遅延作用を伝える。
    void push_range(int left, int right) {
        for (int i = log_; i >= 1; --i) {
            if (((left >> i) << i) != left) {
                push(left >> i);
            }
            if (((right >> i) << i) != right) {
                push((right - 1) >> i);
            }
        }
    }

    /// O(log n)。区間端点から根までを再計算する。
    void update_range(int left, int right) {
        for (int i = 1; i <= log_; ++i) {
            if (((left >> i) << i) != left) {
                update(left >> i);
            }
            if (((right >> i) << i) != right) {
                update((right - 1) >> i);
            }
        }
    }

    /// O(1)。添字が範囲内であることを確認する。
    void assert_index(int index) const {
        assert(0 <= index && index < n_);
    }

    int n_ = 0;
    int size_ = 1;
    int log_ = 0;
    std::vector<S> data_;
    std::vector<F> lazy_;
};

}
