#pragma once
#include <bits/stdc++.h>

namespace poe {

/// 一点加算と区間和を扱う配列: fenwicktree<long long> fw(n); 構築O(n)、更新・取得O(log n)。
template <class T>
struct fenwicktree {
    /// O(n)。fenwicktree<T> fw(size): T{}で初期化されたsize要素を作る。
    explicit fenwicktree(int size) : size_(size) {
        assert(size >= 0);
        data_.resize(static_cast<std::size_t>(size_) + 1);
    }

    /// O(n)。fenwicktree<T> fw(values): valuesから構築する。
    explicit fenwicktree(const std::vector<T>& values)
        : size_(static_cast<int>(values.size())), data_(values.size() + 1) {
        for (int index = 0; index < size_; ++index) {
            const int node = index + 1;
            data_[node] += values[index];
            const int parent = node + (node & -node);
            if (parent <= size_) {
                data_[parent] += data_[node];
            }
        }
    }

    /// O(1)。fw.size(): 配列長を返す。
    int size() const noexcept {
        return size_;
    }

    /// O(log n)。fw.add(i, delta): a[i] += deltaを行う。
    void add(int index, const T& delta) {
        assert(0 <= index && index < size_);
        for (int internal_index = index + 1; internal_index <= size_;
             internal_index += internal_index & -internal_index) {
            data_[internal_index] += delta;
        }
    }

    /// O(log n)。fw.set(i, value): a[i] = valueを行う。
    void set(int index, const T& value) {
        assert(0 <= index && index < size_);
        add(index, value - (*this)[index]);
    }

    /// O(log n)。fw.sum(right): [0, right)の和を返す。
    T sum(int right) const {
        assert(0 <= right && right <= size_);
        T result{};
        for (int internal_index = right; internal_index > 0;
             internal_index -= internal_index & -internal_index) {
            result += data_[internal_index];
        }
        return result;
    }

    /// O(log n)。fw.sum(left, right): [left, right)の和を返す。
    T sum(int left, int right) const {
        assert(0 <= left && left <= right && right <= size_);
        return sum(right) - sum(left);
    }

    /// O(log n)。fw.sum(): 全要素の和を返す。
    T sum() const {
        return sum(size_);
    }

    /// O(log n)。fw[index]: a[index]を返す。
    T operator[](int index) const {
        assert(0 <= index && index < size_);
        return sum(index, index + 1);
    }

    /// O(log n)。fw.lower_bound(x): fw.sum(i + 1) >= xとなる最初のi。なければfw.size()。
    /// 全要素が非負であること。
    int lower_bound(const T& value) const {
        if (!(T{} < value)) {
            return 0;
        }
        int index = 0;
        T prefix{};
        for (int step = search_step(); step > 0; step >>= 1) {
            const int next = index + step;
            if (next <= size_) {
                T candidate = prefix;
                candidate += data_[next];
                if (candidate < value) {
                    index = next;
                    prefix = candidate;
                }
            }
        }
        return index;
    }

    /// O(log n)。fw.upper_bound(x): fw.sum(i + 1) > xとなる最初のi。なければfw.size()。
    /// 全要素が非負であること。
    int upper_bound(const T& value) const {
        if (value < T{}) {
            return 0;
        }
        int index = 0;
        T prefix{};
        for (int step = search_step(); step > 0; step >>= 1) {
            const int next = index + step;
            if (next <= size_) {
                T candidate = prefix;
                candidate += data_[next];
                if (!(value < candidate)) {
                    index = next;
                    prefix = candidate;
                }
            }
        }
        return index;
    }

private:
    /// O(log n)。max(1, size())以下で最大の2冪を返す。
    int search_step() const {
        int step = 1;
        while (step <= size_ / 2) {
            step <<= 1;
        }
        return step;
    }

    int size_;
    std::vector<T> data_;
};

/// 登録した二次元点への可換monoid更新と左下prefix集約を扱う疎Fenwick Tree。
/// fenwicktree2d<Coord,S,op,e> fw(points); 構築O(n log n)、各操作O(log^2 n)。
template <class Coordinate, class S, auto op, auto e>
struct fenwicktree2d {
    /// O(n log n)。updateを行う可能性がある座標を全て登録する。
    explicit fenwicktree2d(
        const std::vector<std::pair<Coordinate, Coordinate>>& points
    ) {
        xs_.reserve(points.size());
        for (const auto& [x, y] : points) xs_.push_back(x);
        std::ranges::sort(xs_);
        xs_.erase(std::unique(xs_.begin(), xs_.end()), xs_.end());
        ys_.resize(xs_.size() + 1);
        for (const auto& [x, y] : points) {
            const int x_index = exact_x(x) + 1;
            for (int node = x_index; node <= static_cast<int>(xs_.size());
                 node += node & -node) {
                ys_[node].push_back(y);
            }
        }
        data_.resize(xs_.size() + 1);
        for (int node = 1; node <= static_cast<int>(xs_.size()); ++node) {
            std::ranges::sort(ys_[node]);
            ys_[node].erase(std::unique(ys_[node].begin(), ys_[node].end()), ys_[node].end());
            data_[node].assign(ys_[node].size() + 1, e());
        }
    }

    /// O(log^2 n)。登録点(x,y)の値へvalueをmonoid演算で加える。
    void apply(const Coordinate& x, const Coordinate& y, const S& value) {
        const int x_index = exact_x(x) + 1;
        for (int x_node = x_index; x_node <= static_cast<int>(xs_.size());
             x_node += x_node & -x_node) {
            const auto iterator = std::lower_bound(ys_[x_node].begin(), ys_[x_node].end(), y);
            assert(iterator != ys_[x_node].end() && *iterator == y);
            const int y_index = static_cast<int>(iterator - ys_[x_node].begin()) + 1;
            for (int y_node = y_index; y_node < static_cast<int>(data_[x_node].size());
                 y_node += y_node & -y_node) {
                data_[x_node][y_node] = op(data_[x_node][y_node], value);
            }
        }
    }

    /// O(log^2 n)。登録済み更新のうち座標が(x以下,y以下)の値のmonoid積を返す。
    S prod(const Coordinate& x, const Coordinate& y) const {
        int x_node = static_cast<int>(std::upper_bound(xs_.begin(), xs_.end(), x) - xs_.begin());
        S result = e();
        for (; x_node > 0; x_node -= x_node & -x_node) {
            int y_node = static_cast<int>(
                std::upper_bound(ys_[x_node].begin(), ys_[x_node].end(), y) - ys_[x_node].begin()
            );
            for (; y_node > 0; y_node -= y_node & -y_node) {
                result = op(result, data_[x_node][y_node]);
            }
        }
        return result;
    }

    /// O(1)。登録された異なるx座標数を返す。
    int x_size() const noexcept {
        return static_cast<int>(xs_.size());
    }

private:
    int exact_x(const Coordinate& x) const {
        const auto iterator = std::lower_bound(xs_.begin(), xs_.end(), x);
        assert(iterator != xs_.end() && *iterator == x);
        return static_cast<int>(iterator - xs_.begin());
    }

    std::vector<Coordinate> xs_;
    std::vector<std::vector<Coordinate>> ys_;
    std::vector<std::vector<S>> data_;
};

}
