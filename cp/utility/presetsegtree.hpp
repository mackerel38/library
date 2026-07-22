#pragma once
#include <bits/stdc++.h>
#include "structure/fenwicktree.hpp"
#include "structure/lazysegtree.hpp"
#include "structure/segtree.hpp"

namespace poe {

namespace detail {

template <class T>
T utility_min_op(T left, T right) {
    return std::min(left, right);
}

template <class T>
T utility_min_e() {
    return std::numeric_limits<T>::max();
}

template <class T>
T utility_max_op(T left, T right) {
    return std::max(left, right);
}

template <class T>
T utility_max_e() {
    return std::numeric_limits<T>::lowest();
}

template <class T>
T utility_gcd_op(T left, T right) {
    return std::gcd(left, right);
}

template <class T>
T utility_gcd_e() {
    return T{};
}

template <class T>
struct affine_value {
    T multiply = 1;
    T add = 0;

    T operator()(const T& value) const {
        return multiply * value + add;
    }
};

template <class T>
affine_value<T> affine_composite_op(const affine_value<T>& left,
                                    const affine_value<T>& right) {
    return {right.multiply * left.multiply,
            right.multiply * left.add + right.add};
}

template <class T>
affine_value<T> affine_composite_e() {
    return {T{1}, T{0}};
}

template <class T>
struct prefix_sum_max_value {
    T sum{};
    T maximum = std::numeric_limits<T>::lowest();
};

template <class T>
prefix_sum_max_value<T> prefix_sum_max_op(
    const prefix_sum_max_value<T>& left, const prefix_sum_max_value<T>& right) {
    if (left.maximum == std::numeric_limits<T>::lowest()) return right;
    if (right.maximum == std::numeric_limits<T>::lowest()) return left;
    return {left.sum + right.sum, std::max(left.maximum, left.sum + right.maximum)};
}

template <class T>
prefix_sum_max_value<T> prefix_sum_max_e() {
    return {};
}

}

/// 一点加算・一点代入・区間和: point_add_range_sum<long long> data(values)。
template <class T>
struct point_add_range_sum {
    /// O(n)。n個の0から構築する。
    explicit point_add_range_sum(int n) : data_(n) {}

    /// O(n)。valuesから構築する。
    explicit point_add_range_sum(const std::vector<T>& values) : data_(values) {}

    /// O(log n)。a[index] += deltaを行う。
    void add(int index, const T& delta) { data_.add(index, delta); }

    /// O(log n)。a[index] = valueを行う。
    void set(int index, const T& value) { data_.set(index, value); }

    /// O(log n)。[0,right)の和を返す。
    T sum(int right) const { return data_.sum(right); }

    /// O(log n)。[left,right)の和を返す。
    T sum(int left, int right) const { return data_.sum(left, right); }

    /// O(log n)。全要素の和を返す。
    T sum() const { return data_.sum(); }

    /// O(log n)。data[index]はa[index]を返す。
    T operator[](int index) const { return data_[index]; }

    /// O(1)。要素数を返す。
    int size() const noexcept { return data_.size(); }

private:
    fenwicktree<T> data_;
};

/// 一点一次変換・一点代入・区間和: data.affine(i,multiply,add)。
template <class T>
struct point_affine_range_sum {
    /// O(n)。n個の0から構築する。
    explicit point_affine_range_sum(int n) : data_(n) {}

    /// O(n)。valuesから構築する。
    explicit point_affine_range_sum(const std::vector<T>& values) : data_(values) {}

    /// O(log n)。a[index] = multiply*a[index]+addを行う。
    void affine(int index, const T& multiply, const T& add) {
        data_.set(index, multiply * data_[index] + add);
    }

    /// O(log n)。a[index] = valueを行う。
    void set(int index, const T& value) { data_.set(index, value); }

    /// O(log n)。[0,right)の和を返す。
    T sum(int right) const { return data_.sum(right); }

    /// O(log n)。[left,right)の和を返す。
    T sum(int left, int right) const { return data_.sum(left, right); }

    /// O(log n)。全要素の和を返す。
    T sum() const { return data_.sum(); }

    /// O(log n)。data[index]はa[index]を返す。
    T operator[](int index) const { return data_[index]; }

    /// O(1)。要素数を返す。
    int size() const noexcept { return data_.size(); }

private:
    fenwicktree<T> data_;
};

/// 一点反転・一点代入・区間1数: point_flip_range_sum data(bits)。
struct point_flip_range_sum {
    /// O(n)。n個の0から構築する。
    explicit point_flip_range_sum(int n) : data_(n) {}

    /// O(n)。0/1列bitsから構築する。
    explicit point_flip_range_sum(const std::vector<int>& bits) : data_(bits) {
        for (int bit : bits) assert(bit == 0 || bit == 1);
    }

    /// O(log n)。a[index]を0と1で反転する。
    void flip(int index) { data_.set(index, 1 - data_[index]); }

    /// O(log n)。a[index]を0または1へ代入する。
    void set(int index, int value) {
        assert(value == 0 || value == 1);
        data_.set(index, value);
    }

    /// O(log n)。[0,right)にある1の個数を返す。
    int sum(int right) const { return data_.sum(right); }

    /// O(log n)。[left,right)にある1の個数を返す。
    int sum(int left, int right) const { return data_.sum(left, right); }

    /// O(log n)。全体にある1の個数を返す。
    int sum() const { return data_.sum(); }

    /// O(log n)。data[index]はa[index]を返す。
    int operator[](int index) const { return data_[index]; }

    /// O(1)。要素数を返す。
    int size() const noexcept { return data_.size(); }

private:
    fenwicktree<int> data_;
};

/// 一点代入・区間最小値: point_set_range_min<long long> data(values)。
template <class T>
struct point_set_range_min {
    /// O(n)。n個のmax()から構築する。
    explicit point_set_range_min(int n) : data_(n) {}
    /// O(n)。valuesから構築する。
    explicit point_set_range_min(const std::vector<T>& values) : data_(values) {}
    /// O(log n)。a[index] = valueを行う。
    void set(int index, const T& value) { data_.set(index, value); }
    /// O(log n)。[left,right)の最小値を返す。
    T min(int left, int right) const { return data_.prod(left, right); }
    /// O(1)。全要素の最小値を返す。
    T min() const { return data_.all_prod(); }
    /// O(1)。data[index]はa[index]を返す。
    T operator[](int index) const { return data_[index]; }
    /// O(1)。要素数を返す。
    int size() const noexcept { return data_.size(); }
private:
    segtree<T, detail::utility_min_op<T>, detail::utility_min_e<T>> data_;
};

/// 一点代入・区間最大値: point_set_range_max<long long> data(values)。
template <class T>
struct point_set_range_max {
    /// O(n)。n個のlowest()から構築する。
    explicit point_set_range_max(int n) : data_(n) {}
    /// O(n)。valuesから構築する。
    explicit point_set_range_max(const std::vector<T>& values) : data_(values) {}
    /// O(log n)。a[index] = valueを行う。
    void set(int index, const T& value) { data_.set(index, value); }
    /// O(log n)。[left,right)の最大値を返す。
    T max(int left, int right) const { return data_.prod(left, right); }
    /// O(1)。全要素の最大値を返す。
    T max() const { return data_.all_prod(); }
    /// O(1)。data[index]はa[index]を返す。
    T operator[](int index) const { return data_[index]; }
    /// O(1)。要素数を返す。
    int size() const noexcept { return data_.size(); }
private:
    segtree<T, detail::utility_max_op<T>, detail::utility_max_e<T>> data_;
};

/// 一点代入とprefix和・最大prefix和・最初の到達位置を扱う。
template <class T>
struct point_set_prefix_sum_max {
    using value_type = detail::prefix_sum_max_value<T>;

    /// O(n)。valuesから構築する。
    explicit point_set_prefix_sum_max(const std::vector<T>& values)
        : data_(make_values(values)) {}

    /// O(log n)。a[index] = valueを行う。
    void set(int index, const T& value) {
        data_.set(index, {value, value});
    }

    /// O(log n)。a[index] += deltaを行う。
    void add(int index, const T& delta) {
        set(index, (*this)[index] + delta);
    }

    /// O(1)。a[index]を返す。
    T operator[](int index) const {
        return data_[index].sum;
    }

    /// O(log n)。[0,right)の和を返す。
    T sum(int right) const {
        return data_.prod(0, right).sum;
    }

    /// O(log n)。[left,right)の和を返す。
    T sum(int left, int right) const {
        return data_.prod(left, right).sum;
    }

    /// O(1)。非空prefix和の最大値を返す。空列ではlowest()。
    T max_prefix() const {
        return data_.all_prod().maximum;
    }

    /// O(log n)。prefix和がtarget以上になる最初の要素index。なければsize()。
    int first_prefix_at_least(const T& target) const {
        if (!(std::numeric_limits<T>::lowest() < target)) return 0;
        return data_.max_right(0, [&](const value_type& value) {
            return value.maximum < target;
        });
    }

    /// O(1)。要素数を返す。
    int size() const noexcept {
        return data_.size();
    }

private:
    /// O(n)。通常列をsegment treeのleaf列へ変換する。
    static std::vector<value_type> make_values(const std::vector<T>& values) {
        std::vector<value_type> result;
        result.reserve(values.size());
        for (const T& value : values) result.push_back({value, value});
        return result;
    }

    segtree<value_type, detail::prefix_sum_max_op<T>, detail::prefix_sum_max_e<T>> data_;
};

/// 点列の一点変更と区間内最遠Manhattan距離: point_set_range_manhattan_max<long long> data(points)。
template <class T>
struct point_set_range_manhattan_max {
    using point = std::pair<T, T>;

    /// O(n)。pointsから構築する。
    explicit point_set_range_manhattan_max(const std::vector<point>& points)
        : points_(points), sum_(make_sum(points)), difference_(make_difference(points)) {}

    /// O(log n)。point[index] = (x, y)を行う。
    void set(int index, T x, T y) {
        points_[index] = {x, y};
        sum_.set(index, x + y);
        difference_.set(index, x - y);
    }

    /// O(log n)。[left,right)内の点と(x,y)とのManhattan距離の最大値を返す。
    T max(int left, int right, T x, T y) const {
        const T sum = x + y;
        const T difference = x - y;
        return std::max({sum - sum_.min(left, right), sum_.max(left, right) - sum,
                         difference - difference_.min(left, right),
                         difference_.max(left, right) - difference});
    }

    /// O(1)。point[index]を返す。
    point operator[](int index) const { return points_[index]; }

    /// O(1)。点数を返す。
    int size() const noexcept { return static_cast<int>(points_.size()); }

private:
    struct minmax {
        point_set_range_min<T> minimum;
        point_set_range_max<T> maximum;
        explicit minmax(const std::vector<T>& values) : minimum(values), maximum(values) {}
        void set(int index, const T& value) { minimum.set(index, value); maximum.set(index, value); }
        T min(int left, int right) const { return minimum.min(left, right); }
        T max(int left, int right) const { return maximum.max(left, right); }
    };
    static std::vector<T> make_sum(const std::vector<point>& points) {
        std::vector<T> values;
        for (const auto& [x, y] : points) values.push_back(x + y);
        return values;
    }
    static std::vector<T> make_difference(const std::vector<point>& points) {
        std::vector<T> values;
        for (const auto& [x, y] : points) values.push_back(x - y);
        return values;
    }
    std::vector<point> points_;
    minmax sum_;
    minmax difference_;
};

/// 一点代入・区間gcd: point_set_range_gcd<long long> data(values)。
template <class T>
struct point_set_range_gcd {
    /// O(n)。n個の0から構築する。
    explicit point_set_range_gcd(int n) : data_(n) {}
    /// O(n)。valuesから構築する。
    explicit point_set_range_gcd(const std::vector<T>& values) : data_(values) {}
    /// O(log n)。a[index] = valueを行う。
    void set(int index, const T& value) { data_.set(index, value); }
    /// O(log n)。[left,right)のgcdを返す。空区間は0。
    T gcd(int left, int right) const { return data_.prod(left, right); }
    /// O(1)。全要素のgcdを返す。
    T gcd() const { return data_.all_prod(); }
    /// O(1)。data[index]はa[index]を返す。
    T operator[](int index) const { return data_[index]; }
    /// O(1)。要素数を返す。
    int size() const noexcept { return data_.size(); }
private:
    segtree<T, detail::utility_gcd_op<T>, detail::utility_gcd_e<T>> data_;
};

/// 一点関数代入・区間関数合成: point_set_range_composite<modint> data(functions)。
template <class T>
struct point_set_range_composite {
    using affine = detail::affine_value<T>;

    /// O(n)。n個の恒等関数から構築する。
    explicit point_set_range_composite(int n) : data_(n) {}
    /// O(n)。functionsから構築する。
    explicit point_set_range_composite(const std::vector<affine>& functions)
        : data_(functions) {}
    /// O(log n)。index番目の関数をmultiply*x+addへ置き換える。
    void set(int index, const T& multiply, const T& add) {
        data_.set(index, affine{multiply, add});
    }
    /// O(log n)。[left,right)を添字順に適用した合成関数を返す。
    affine composite(int left, int right) const { return data_.prod(left, right); }
    /// O(log n)。[left,right)を添字順にvalueへ適用する。
    T apply(int left, int right, const T& value) const {
        return composite(left, right)(value);
    }
    /// O(1)。data[index]はindex番目の関数を返す。
    affine operator[](int index) const { return data_[index]; }
    /// O(1)。要素数を返す。
    int size() const noexcept { return data_.size(); }
private:
    segtree<affine, detail::affine_composite_op<T>, detail::affine_composite_e<T>> data_;
};

}

namespace poe {

namespace detail {

template <class T>
struct range_sum_node {
    T sum{};
    int length = 0;
};

template <class T>
range_sum_node<T> range_sum_op(range_sum_node<T> left, range_sum_node<T> right) {
    return {left.sum + right.sum, left.length + right.length};
}

template <class T>
range_sum_node<T> range_sum_e() {
    return {};
}

template <class T>
range_sum_node<T> range_add_sum_mapping(const T& add, range_sum_node<T> value) {
    value.sum += add * T(value.length);
    return value;
}

template <class T>
T range_add_composition(const T& added, const T& old) {
    return added + old;
}

template <class T>
T range_add_id() {
    return T{};
}

template <class T>
range_sum_node<T> range_set_sum_mapping(const std::optional<T>& set,
                                        range_sum_node<T> value) {
    if (set) value.sum = *set * T(value.length);
    return value;
}

template <class T>
std::optional<T> range_set_composition(const std::optional<T>& set,
                                       const std::optional<T>& old) {
    return set ? set : old;
}

template <class T>
std::optional<T> range_set_id() {
    return std::nullopt;
}

template <class T>
range_sum_node<T> range_affine_sum_mapping(const affine_value<T>& function,
                                           range_sum_node<T> value) {
    value.sum = function.multiply * value.sum + function.add * T(value.length);
    return value;
}

template <class T>
affine_value<T> range_affine_composition(const affine_value<T>& added,
                                         const affine_value<T>& old) {
    return {added.multiply * old.multiply,
            added.multiply * old.add + added.add};
}

template <class T>
affine_value<T> range_affine_id() {
    return {T{1}, T{0}};
}

template <class T>
T range_add_min_mapping(const T& add, const T& value) {
    return value == utility_min_e<T>() ? value : value + add;
}

template <class T>
T range_add_max_mapping(const T& add, const T& value) {
    return value == utility_max_e<T>() ? value : value + add;
}

template <class T>
T range_set_value_mapping(const std::optional<T>& set, const T& value) {
    return set ? *set : value;
}

template <class T>
struct range_gcd_node {
    T gcd{};
    T value{};
    int length = 0;
};

template <class T>
range_gcd_node<T> range_gcd_op(range_gcd_node<T> left, range_gcd_node<T> right) {
    return {std::gcd(left.gcd, right.gcd), T{}, left.length + right.length};
}

template <class T>
range_gcd_node<T> range_gcd_e() {
    return {};
}

template <class T>
range_gcd_node<T> range_set_gcd_mapping(const std::optional<T>& set,
                                        range_gcd_node<T> value) {
    if (set) {
        value.gcd = std::gcd(*set, T{});
        value.value = *set;
    }
    return value;
}

struct inversion_node {
    long long zero = 0;
    long long one = 0;
    long long inversion = 0;
};

inline inversion_node inversion_op(inversion_node left, inversion_node right) {
    return {left.zero + right.zero, left.one + right.one,
            left.inversion + right.inversion + left.one * right.zero};
}

inline inversion_node inversion_e() {
    return {};
}

inline inversion_node inversion_mapping(bool flip, inversion_node value) {
    if (flip) {
        value.inversion = value.zero * value.one - value.inversion;
        std::swap(value.zero, value.one);
    }
    return value;
}

inline bool flip_composition(bool added, bool old) {
    return added != old;
}

inline bool flip_id() {
    return false;
}

inline range_sum_node<long long> range_flip_sum_mapping(
    bool flip, range_sum_node<long long> value) {
    if (flip) value.sum = value.length - value.sum;
    return value;
}

template <class T>
std::vector<range_sum_node<T>> make_sum_nodes(const std::vector<T>& values) {
    std::vector<range_sum_node<T>> nodes;
    nodes.reserve(values.size());
    for (const T& value : values) nodes.push_back({value, 1});
    return nodes;
}

template <class T>
std::vector<range_sum_node<T>> make_zero_sum_nodes(int n) {
    assert(n >= 0);
    return std::vector<range_sum_node<T>>(n, range_sum_node<T>{T{}, 1});
}

template <class T>
struct range_sum_square_node {
    T sum{};
    T square_sum{};
    int length = 0;
};

template <class T>
range_sum_square_node<T> range_sum_square_op(range_sum_square_node<T> left,
                                             range_sum_square_node<T> right) {
    return {left.sum + right.sum, left.square_sum + right.square_sum,
            left.length + right.length};
}

template <class T>
range_sum_square_node<T> range_sum_square_e() {
    return {};
}

template <class T>
range_sum_square_node<T> range_add_sum_square_mapping(
    const T& add, range_sum_square_node<T> value) {
    value.square_sum += T{2} * add * value.sum + add * add * T{value.length};
    value.sum += add * T{value.length};
    return value;
}

template <class T>
std::vector<range_sum_square_node<T>> make_sum_square_nodes(
    const std::vector<T>& values) {
    std::vector<range_sum_square_node<T>> nodes;
    nodes.reserve(values.size());
    for (const T& value : values) nodes.push_back({value, value * value, 1});
    return nodes;
}

template <class T>
std::vector<range_sum_square_node<T>> make_zero_sum_square_nodes(int n) {
    assert(n >= 0);
    return std::vector<range_sum_square_node<T>>(
        n, range_sum_square_node<T>{T{}, T{}, 1});
}

}

/// 区間加算・区間和: range_add_range_sum<long long> data(values)。
template <class T>
struct range_add_range_sum {
    using node = detail::range_sum_node<T>;
    using tree = lazysegtree<node, detail::range_sum_op<T>, detail::range_sum_e<T>, T,
                              detail::range_add_sum_mapping<T>,
                              detail::range_add_composition<T>, detail::range_add_id<T>>;
    /// O(n)。n個の0から構築する。
    explicit range_add_range_sum(int n) : data_(detail::make_zero_sum_nodes<T>(n)) {}
    /// O(n)。valuesから構築する。
    explicit range_add_range_sum(const std::vector<T>& values)
        : data_(detail::make_sum_nodes(values)) {}
    /// O(log n)。[left,right)の各要素へdeltaを加える。
    void add(int left, int right, const T& delta) { data_.apply(left, right, delta); }
    /// O(log n)。a[index] = valueを行う。
    void set(int index, const T& value) { data_.set(index, node{value, 1}); }
    /// O(log n)。[0,right)の和を返す。
    T sum(int right) { return data_.prod(0, right).sum; }
    /// O(log n)。[left,right)の和を返す。
    T sum(int left, int right) { return data_.prod(left, right).sum; }
    /// O(1)。全要素の和を返す。
    T sum() const { return data_.all_prod().sum; }
    /// O(log n)。data[index]はa[index]を返す。
    T operator[](int index) { return data_[index].sum; }
    /// O(1)。要素数を返す。
    int size() const noexcept { return data_.size(); }
private:
    tree data_;
};

/// 区間加算・区間和・区間二乗和: range_add_range_sum_square<mint> data(values)。
template <class T>
struct range_add_range_sum_square {
    using node = detail::range_sum_square_node<T>;
    using tree = lazysegtree<node, detail::range_sum_square_op<T>,
                              detail::range_sum_square_e<T>, T,
                              detail::range_add_sum_square_mapping<T>,
                              detail::range_add_composition<T>, detail::range_add_id<T>>;

    /// O(n)。n個の0から構築する。
    explicit range_add_range_sum_square(int n)
        : data_(detail::make_zero_sum_square_nodes<T>(n)) {}

    /// O(n)。valuesから構築する。
    explicit range_add_range_sum_square(const std::vector<T>& values)
        : data_(detail::make_sum_square_nodes(values)) {}

    /// O(log n)。[left,right)の各要素へdeltaを加える。
    void add(int left, int right, const T& delta) { data_.apply(left, right, delta); }

    /// O(log n)。a[index] = valueを行う。
    void set(int index, const T& value) {
        data_.set(index, node{value, value * value, 1});
    }

    /// O(log n)。[0,right)の和を返す。
    T sum(int right) { return data_.prod(0, right).sum; }

    /// O(log n)。[left,right)の和を返す。
    T sum(int left, int right) { return data_.prod(left, right).sum; }

    /// O(1)。全要素の和を返す。
    T sum() const { return data_.all_prod().sum; }

    /// O(log n)。[0,right)の二乗和を返す。
    T square_sum(int right) { return data_.prod(0, right).square_sum; }

    /// O(log n)。[left,right)の二乗和を返す。
    T square_sum(int left, int right) { return data_.prod(left, right).square_sum; }

    /// O(1)。全要素の二乗和を返す。
    T square_sum() const { return data_.all_prod().square_sum; }

    /// O(log n)。data[index]はa[index]を返す。
    T operator[](int index) { return data_[index].sum; }

    /// O(1)。要素数を返す。
    int size() const noexcept { return data_.size(); }

private:
    tree data_;
};

/// 区間代入・区間和: range_set_range_sum<long long> data(values)。
template <class T>
struct range_set_range_sum {
    using node = detail::range_sum_node<T>;
    using action = std::optional<T>;
    using tree = lazysegtree<node, detail::range_sum_op<T>, detail::range_sum_e<T>, action,
                              detail::range_set_sum_mapping<T>,
                              detail::range_set_composition<T>, detail::range_set_id<T>>;
    /// O(n)。n個の0から構築する。
    explicit range_set_range_sum(int n) : data_(detail::make_zero_sum_nodes<T>(n)) {}
    /// O(n)。valuesから構築する。
    explicit range_set_range_sum(const std::vector<T>& values)
        : data_(detail::make_sum_nodes(values)) {}
    /// O(log n)。[left,right)の各要素をvalueへ置き換える。
    void set(int left, int right, const T& value) { data_.apply(left, right, action{value}); }
    /// O(log n)。a[index] = valueを行う。
    void set(int index, const T& value) { data_.set(index, node{value, 1}); }
    /// O(log n)。[0,right)の和を返す。
    T sum(int right) { return data_.prod(0, right).sum; }
    /// O(log n)。[left,right)の和を返す。
    T sum(int left, int right) { return data_.prod(left, right).sum; }
    /// O(1)。全要素の和を返す。
    T sum() const { return data_.all_prod().sum; }
    /// O(log n)。data[index]はa[index]を返す。
    T operator[](int index) { return data_[index].sum; }
    /// O(1)。要素数を返す。
    int size() const noexcept { return data_.size(); }
private:
    tree data_;
};

/// 区間一次変換・区間和: data.affine(left,right,multiply,add)。
template <class T>
struct range_affine_range_sum {
    using node = detail::range_sum_node<T>;
    using affine_value = detail::affine_value<T>;
    using tree = lazysegtree<node, detail::range_sum_op<T>, detail::range_sum_e<T>, affine_value,
                              detail::range_affine_sum_mapping<T>,
                              detail::range_affine_composition<T>, detail::range_affine_id<T>>;
    /// O(n)。n個の0から構築する。
    explicit range_affine_range_sum(int n) : data_(detail::make_zero_sum_nodes<T>(n)) {}
    /// O(n)。valuesから構築する。
    explicit range_affine_range_sum(const std::vector<T>& values)
        : data_(detail::make_sum_nodes(values)) {}
    /// O(log n)。[left,right)をmultiply*x+addで更新する。
    void affine(int left, int right, const T& multiply, const T& add) {
        data_.apply(left, right, affine_value{multiply, add});
    }
    /// O(log n)。a[index] = valueを行う。
    void set(int index, const T& value) { data_.set(index, node{value, 1}); }
    /// O(log n)。[0,right)の和を返す。
    T sum(int right) { return data_.prod(0, right).sum; }
    /// O(log n)。[left,right)の和を返す。
    T sum(int left, int right) { return data_.prod(left, right).sum; }
    /// O(1)。全要素の和を返す。
    T sum() const { return data_.all_prod().sum; }
    /// O(log n)。data[index]はa[index]を返す。
    T operator[](int index) { return data_[index].sum; }
    /// O(1)。要素数を返す。
    int size() const noexcept { return data_.size(); }
private:
    tree data_;
};

/// 区間加算・区間最小値: range_add_range_min<long long> data(values)。
template <class T>
struct range_add_range_min {
    using tree = lazysegtree<T, detail::utility_min_op<T>, detail::utility_min_e<T>, T,
                              detail::range_add_min_mapping<T>,
                              detail::range_add_composition<T>, detail::range_add_id<T>>;
    /// O(n)。n個の0から構築する。
    explicit range_add_range_min(int n) : data_(std::vector<T>(n)) {}
    /// O(n)。valuesから構築する。
    explicit range_add_range_min(const std::vector<T>& values) : data_(values) {}
    /// O(log n)。[left,right)の各要素へdeltaを加える。
    void add(int left, int right, const T& delta) { data_.apply(left, right, delta); }
    /// O(log n)。a[index] = valueを行う。
    void set(int index, const T& value) { data_.set(index, value); }
    /// O(log n)。[left,right)の最小値を返す。
    T min(int left, int right) { return data_.prod(left, right); }
    /// O(1)。全要素の最小値を返す。
    T min() const { return data_.all_prod(); }
    /// O(log n)。data[index]はa[index]を返す。
    T operator[](int index) { return data_[index]; }
    /// O(1)。要素数を返す。
    int size() const noexcept { return data_.size(); }
private:
    tree data_;
};

namespace detail {

template <class T>
struct range_consume_node {
    T minimum = std::numeric_limits<T>::max();
    int active_count = 0;
};

template <class T>
range_consume_node<T> range_consume_op(
    const range_consume_node<T>& left,
    const range_consume_node<T>& right
) {
    return {std::min(left.minimum, right.minimum),
            left.active_count + right.active_count};
}

template <class T>
range_consume_node<T> range_consume_e() {
    return {};
}

template <class T>
range_consume_node<T> range_consume_mapping(
    const T& add,
    range_consume_node<T> value
) {
    if (value.active_count > 0) value.minimum += add;
    return value;
}

template <class T>
std::vector<range_consume_node<T>> make_consume_nodes(
    const std::vector<T>& values
) {
    std::vector<range_consume_node<T>> result;
    result.reserve(values.size());
    for (const T& value : values) {
        assert(value >= T{});
        result.push_back({value, 1});
    }
    return result;
}

}

/// 区間消費・0下限・消費量和: data.subtract(left,right,amount)。
template <class T>
struct range_subtract_clamp_zero_sum {
    using node = detail::range_consume_node<T>;
    using tree = lazysegtree<node, detail::range_consume_op<T>,
                              detail::range_consume_e<T>, T,
                              detail::range_consume_mapping<T>,
                              detail::range_add_composition<T>,
                              detail::range_add_id<T>>;

    /// O(n)。n個の0から構築する。
    explicit range_subtract_clamp_zero_sum(int n)
        : range_subtract_clamp_zero_sum(std::vector<T>(n)) {}

    /// O(n)。非負列valuesから構築する。
    explicit range_subtract_clamp_zero_sum(const std::vector<T>& values)
        : data_(detail::make_consume_nodes(values)) {}

    /// O((z+1)log n)。[left,right)の各値からmin(value,amount)を引き、消費量和を返す。
    /// zはこの操作で初めて0になる要素数。全操作では各要素が高々一度だけzへ数えられる。
    T subtract(int left, int right, const T& amount) {
        assert(0 <= left && left <= right && right <= size());
        assert(amount >= T{});
        if (left == right || amount == T{}) return T{};

        T result = amount * T{data_.prod(left, right).active_count};
        data_.apply(left, right, -amount);
        while (data_.prod(left, right).minimum < T{}) {
            const int index = data_.max_right(
                left, [](const node& value) { return value.minimum >= T{}; });
            const T remaining = data_[index].minimum;
            result += remaining;
            data_.set(index, node{});
        }
        return result;
    }

    /// O(log n)。a[index] = valueとして再び有効化する。
    void set(int index, const T& value) {
        assert(value >= T{});
        data_.set(index, node{value, 1});
    }

    /// O(log n)。data[index]は現在値を返す。
    T operator[](int index) {
        const node value = data_[index];
        return value.active_count == 0 ? T{} : value.minimum;
    }

    /// O(log n)。indexがまだ消費対象ならtrueを返す。値0でもset直後はtrue。
    bool active(int index) {
        return data_[index].active_count > 0;
    }

    /// O(1)。要素数を返す。
    int size() const noexcept { return data_.size(); }

private:
    tree data_;
};

/// 区間加算・区間最大値: range_add_range_max<long long> data(values)。
template <class T>
struct range_add_range_max {
    using tree = lazysegtree<T, detail::utility_max_op<T>, detail::utility_max_e<T>, T,
                              detail::range_add_max_mapping<T>,
                              detail::range_add_composition<T>, detail::range_add_id<T>>;
    /// O(n)。n個の0から構築する。
    explicit range_add_range_max(int n) : data_(std::vector<T>(n)) {}
    /// O(n)。valuesから構築する。
    explicit range_add_range_max(const std::vector<T>& values) : data_(values) {}
    /// O(log n)。[left,right)の各要素へdeltaを加える。
    void add(int left, int right, const T& delta) { data_.apply(left, right, delta); }
    /// O(log n)。a[index] = valueを行う。
    void set(int index, const T& value) { data_.set(index, value); }
    /// O(log n)。[left,right)の最大値を返す。
    T max(int left, int right) { return data_.prod(left, right); }
    /// O(1)。全要素の最大値を返す。
    T max() const { return data_.all_prod(); }
    /// O(log n)。data[index]はa[index]を返す。
    T operator[](int index) { return data_[index]; }
    /// O(1)。要素数を返す。
    int size() const noexcept { return data_.size(); }
private:
    tree data_;
};

/// 区間代入・区間最小値: range_set_range_min<long long> data(values)。
template <class T>
struct range_set_range_min {
    using action = std::optional<T>;
    using tree = lazysegtree<T, detail::utility_min_op<T>, detail::utility_min_e<T>, action,
                              detail::range_set_value_mapping<T>,
                              detail::range_set_composition<T>, detail::range_set_id<T>>;
    /// O(n)。n個のmax()から構築する。
    explicit range_set_range_min(int n) : data_(n) {}
    /// O(n)。valuesから構築する。
    explicit range_set_range_min(const std::vector<T>& values) : data_(values) {}
    /// O(log n)。[left,right)の各要素をvalueへ置き換える。
    void set(int left, int right, const T& value) { data_.apply(left, right, action{value}); }
    /// O(log n)。a[index] = valueを行う。
    void set(int index, const T& value) { data_.set(index, value); }
    /// O(log n)。[left,right)の最小値を返す。
    T min(int left, int right) { return data_.prod(left, right); }
    /// O(1)。全要素の最小値を返す。
    T min() const { return data_.all_prod(); }
    /// O(log n)。data[index]はa[index]を返す。
    T operator[](int index) { return data_[index]; }
    /// O(1)。要素数を返す。
    int size() const noexcept { return data_.size(); }
private:
    tree data_;
};

/// 区間代入・区間最大値: range_set_range_max<long long> data(values)。
template <class T>
struct range_set_range_max {
    using action = std::optional<T>;
    using tree = lazysegtree<T, detail::utility_max_op<T>, detail::utility_max_e<T>, action,
                              detail::range_set_value_mapping<T>,
                              detail::range_set_composition<T>, detail::range_set_id<T>>;
    /// O(n)。n個のlowest()から構築する。
    explicit range_set_range_max(int n) : data_(n) {}
    /// O(n)。valuesから構築する。
    explicit range_set_range_max(const std::vector<T>& values) : data_(values) {}
    /// O(log n)。[left,right)の各要素をvalueへ置き換える。
    void set(int left, int right, const T& value) { data_.apply(left, right, action{value}); }
    /// O(log n)。a[index] = valueを行う。
    void set(int index, const T& value) { data_.set(index, value); }
    /// O(log n)。[left,right)の最大値を返す。
    T max(int left, int right) { return data_.prod(left, right); }
    /// O(1)。全要素の最大値を返す。
    T max() const { return data_.all_prod(); }
    /// O(log n)。data[index]はa[index]を返す。
    T operator[](int index) { return data_[index]; }
    /// O(1)。要素数を返す。
    int size() const noexcept { return data_.size(); }
private:
    tree data_;
};

/// 区間代入・区間gcd: range_set_range_gcd<long long> data(values)。
template <class T>
struct range_set_range_gcd {
    using node = detail::range_gcd_node<T>;
    using action = std::optional<T>;
    using tree = lazysegtree<node, detail::range_gcd_op<T>, detail::range_gcd_e<T>, action,
                              detail::range_set_gcd_mapping<T>,
                              detail::range_set_composition<T>, detail::range_set_id<T>>;
    /// O(n)。n個の0から構築する。
    explicit range_set_range_gcd(int n) : data_(std::vector<node>(n, node{T{}, T{}, 1})) {}
    /// O(n)。valuesから構築する。
    explicit range_set_range_gcd(const std::vector<T>& values) : data_(make_nodes(values)) {}
    /// O(log n)。[left,right)の各要素をvalueへ置き換える。
    void set(int left, int right, const T& value) { data_.apply(left, right, action{value}); }
    /// O(log n)。a[index] = valueを行う。
    void set(int index, const T& value) {
        data_.set(index, node{std::gcd(value, T{}), value, 1});
    }
    /// O(log n)。[left,right)のgcdを返す。空区間は0。
    T gcd(int left, int right) { return data_.prod(left, right).gcd; }
    /// O(1)。全要素のgcdを返す。
    T gcd() const { return data_.all_prod().gcd; }
    /// O(log n)。data[index]はa[index]を返す。
    T operator[](int index) { return data_[index].value; }
    /// O(1)。要素数を返す。
    int size() const noexcept { return data_.size(); }
private:
    static std::vector<node> make_nodes(const std::vector<T>& values) {
        std::vector<node> result;
        result.reserve(values.size());
        for (const T& value : values)
            result.push_back({std::gcd(value, T{}), value, 1});
        return result;
    }
    tree data_;
};

/// 区間加算・区間gcd: range_add_range_gcd<long long> data(values)。
template <class T>
struct range_add_range_gcd {
    /// O(n)。n個の0から構築する。
    explicit range_add_range_gcd(int n)
        : range_add_range_gcd(std::vector<T>(n)) {}
    /// O(n)。valuesから構築する。
    explicit range_add_range_gcd(const std::vector<T>& values)
        : difference_(make_difference(values)), gcd_(make_difference(values)) {}
    /// O(log n)。[left,right)の各要素へdeltaを加える。
    void add(int left, int right, const T& delta) {
        assert(0 <= left && left <= right && right <= size());
        if (left == right) return;
        change_difference(left, delta);
        if (right < size()) change_difference(right, -delta);
    }
    /// O(log n)。a[index] = valueを行う。
    void set(int index, const T& value) { add(index, index + 1, value - (*this)[index]); }
    /// O(log n)。[left,right)のgcdを返す。空区間は0。
    T gcd(int left, int right) const {
        assert(0 <= left && left <= right && right <= size());
        if (left == right) return T{};
        return std::gcd(difference_.sum(left + 1), gcd_.gcd(left + 1, right));
    }
    /// O(log n)。全要素のgcdを返す。
    T gcd() const { return gcd(0, size()); }
    /// O(log n)。data[index]はa[index]を返す。
    T operator[](int index) const { return difference_.sum(index + 1); }
    /// O(1)。要素数を返す。
    int size() const noexcept { return difference_.size(); }
private:
    static std::vector<T> make_difference(const std::vector<T>& values) {
        std::vector<T> result(values.size());
        for (int i = 0; i < static_cast<int>(values.size()); ++i)
            result[i] = values[i] - (i == 0 ? T{} : values[i - 1]);
        return result;
    }
    void change_difference(int index, const T& delta) {
        difference_.add(index, delta);
        gcd_.set(index, difference_[index]);
    }
    fenwicktree<T> difference_;
    point_set_range_gcd<T> gcd_;
};

/// 区間加算・一点取得: range_add_point_get<long long> data(values)。
template <class T>
struct range_add_point_get {
    /// O(n)。n個の0から構築する。
    explicit range_add_point_get(int n) : data_(n) {}
    /// O(n)。valuesから構築する。
    explicit range_add_point_get(const std::vector<T>& values) : data_(values) {}
    /// O(log n)。[left,right)の各要素へdeltaを加える。
    void add(int left, int right, const T& delta) { data_.add(left, right, delta); }
    /// O(log n)。a[index] = valueを行う。
    void set(int index, const T& value) { data_.set(index, value); }
    /// O(log n)。data[index]はa[index]を返す。
    T operator[](int index) { return data_[index]; }
    /// O(1)。要素数を返す。
    int size() const noexcept { return data_.size(); }
private:
    range_add_range_sum<T> data_;
};

/// 区間代入・一点取得: range_set_point_get<long long> data(values)。
template <class T>
struct range_set_point_get {
    /// O(n)。n個の0から構築する。
    explicit range_set_point_get(int n) : data_(n) {}
    /// O(n)。valuesから構築する。
    explicit range_set_point_get(const std::vector<T>& values) : data_(values) {}
    /// O(log n)。[left,right)の各要素をvalueへ置き換える。
    void set(int left, int right, const T& value) { data_.set(left, right, value); }
    /// O(log n)。a[index] = valueを行う。
    void set(int index, const T& value) { data_.set(index, value); }
    /// O(log n)。data[index]はa[index]を返す。
    T operator[](int index) { return data_[index]; }
    /// O(1)。要素数を返す。
    int size() const noexcept { return data_.size(); }
private:
    range_set_range_sum<T> data_;
};

/// 区間一次変換・一点取得: range_affine_point_get<long long> data(values)。
template <class T>
struct range_affine_point_get {
    /// O(n)。n個の0から構築する。
    explicit range_affine_point_get(int n) : data_(n) {}
    /// O(n)。valuesから構築する。
    explicit range_affine_point_get(const std::vector<T>& values) : data_(values) {}
    /// O(log n)。[left,right)をmultiply*x+addで更新する。
    void affine(int left, int right, const T& multiply, const T& add) {
        data_.affine(left, right, multiply, add);
    }
    /// O(log n)。a[index] = valueを行う。
    void set(int index, const T& value) { data_.set(index, value); }
    /// O(log n)。data[index]はa[index]を返す。
    T operator[](int index) { return data_[index]; }
    /// O(1)。要素数を返す。
    int size() const noexcept { return data_.size(); }
private:
    range_affine_range_sum<T> data_;
};

/// 区間反転・区間1数: range_flip_range_sum data(bits)。
struct range_flip_range_sum {
    using node = detail::range_sum_node<long long>;
    using tree = lazysegtree<node, detail::range_sum_op<long long>,
                              detail::range_sum_e<long long>, bool,
                              detail::range_flip_sum_mapping, detail::flip_composition,
                              detail::flip_id>;
    /// O(n)。n個の0から構築する。
    explicit range_flip_range_sum(int n) : range_flip_range_sum(std::vector<int>(n)) {}
    /// O(n)。0/1列bitsから構築する。
    explicit range_flip_range_sum(const std::vector<int>& bits)
        : data_(make_nodes(bits)) {}
    /// O(log n)。[left,right)を0と1で反転する。
    void flip(int left, int right) { data_.apply(left, right, true); }
    /// O(log n)。a[index]を0または1へ代入する。
    void set(int index, int value) {
        assert(value == 0 || value == 1);
        data_.set(index, node{value, 1});
    }
    /// O(log n)。[left,right)にある1の個数を返す。
    long long sum(int left, int right) { return data_.prod(left, right).sum; }
    /// O(1)。全体にある1の個数を返す。
    long long sum() const { return data_.all_prod().sum; }
    /// O(log n)。data[index]はa[index]を返す。
    int operator[](int index) { return static_cast<int>(data_[index].sum); }
    /// O(1)。要素数を返す。
    int size() const noexcept { return data_.size(); }
private:
    static std::vector<node> make_nodes(const std::vector<int>& bits) {
        std::vector<node> result;
        result.reserve(bits.size());
        for (int bit : bits) {
            assert(bit == 0 || bit == 1);
            result.push_back({bit, 1});
        }
        return result;
    }
    tree data_;
};

/// 区間反転・区間転倒数: range_flip_range_inversion data(bits)。
struct range_flip_range_inversion {
    using node = detail::inversion_node;
    using tree = lazysegtree<node, detail::inversion_op, detail::inversion_e, bool,
                              detail::inversion_mapping, detail::flip_composition,
                              detail::flip_id>;
    /// O(n)。n個の0から構築する。
    explicit range_flip_range_inversion(int n)
        : range_flip_range_inversion(std::vector<int>(n)) {}
    /// O(n)。0/1列bitsから構築する。
    explicit range_flip_range_inversion(const std::vector<int>& bits)
        : data_(make_nodes(bits)) {}
    /// O(log n)。[left,right)を0と1で反転する。
    void flip(int left, int right) { data_.apply(left, right, true); }
    /// O(log n)。a[index]を0または1へ代入する。
    void set(int index, int value) {
        assert(value == 0 || value == 1);
        data_.set(index, node{1 - value, value, 0});
    }
    /// O(log n)。[left,right)にある(1,0)の添字対数を返す。
    long long inversion(int left, int right) {
        return data_.prod(left, right).inversion;
    }
    /// O(log n)。[left,right)にある1の個数を返す。
    long long sum(int left, int right) { return data_.prod(left, right).one; }
    /// O(log n)。data[index]はa[index]を返す。
    int operator[](int index) { return static_cast<int>(data_[index].one); }
    /// O(1)。要素数を返す。
    int size() const noexcept { return data_.size(); }
private:
    static std::vector<node> make_nodes(const std::vector<int>& bits) {
        std::vector<node> result;
        result.reserve(bits.size());
        for (int bit : bits) {
            assert(bit == 0 || bit == 1);
            result.push_back({1 - bit, bit, 0});
        }
        return result;
    }
    tree data_;
};

namespace detail {

template <class T>
struct active_flip_max_node {
    T active_max = std::numeric_limits<T>::lowest();
    int active_count = 0;
    int inactive_count = 0;
};

template <class T>
active_flip_max_node<T> active_flip_max_op(
    active_flip_max_node<T> left,
    active_flip_max_node<T> right
) {
    return {
        std::max(left.active_max, right.active_max),
        left.active_count + right.active_count,
        left.inactive_count + right.inactive_count,
    };
}

template <class T>
active_flip_max_node<T> active_flip_max_e() {
    return {};
}

template <class T>
struct active_flip_action {
    int flip_count = 0;
    T add{};
};

template <class T>
active_flip_max_node<T> active_flip_max_mapping(
    const active_flip_action<T>& action,
    active_flip_max_node<T> value
) {
    if (action.flip_count == 0) {
        if (value.active_count > 0) value.active_max += action.add;
        return value;
    }
    if (action.flip_count & 1) std::swap(value.active_count, value.inactive_count);
    value.active_max = value.active_count > 0
        ? action.add : std::numeric_limits<T>::lowest();
    return value;
}

template <class T>
active_flip_action<T> active_flip_composition(
    const active_flip_action<T>& added,
    const active_flip_action<T>& old
) {
    return {
        added.flip_count + old.flip_count,
        (added.flip_count == 0 ? old.add : T{}) + added.add,
    };
}

template <class T>
active_flip_action<T> active_flip_id() {
    return {};
}

}

/// 区間active加算・全値clear後のactive反転・区間最大値をO(log n)で扱う。
template <class T>
struct range_active_add_range_clear_flip_max {
    using node = detail::active_flip_max_node<T>;
    using action = detail::active_flip_action<T>;
    using tree = lazysegtree<node, detail::active_flip_max_op<T>,
                              detail::active_flip_max_e<T>, action,
                              detail::active_flip_max_mapping<T>,
                              detail::active_flip_composition<T>,
                              detail::active_flip_id<T>>;

    /// O(n)。activeかつ値0のn要素から構築する。
    explicit range_active_add_range_clear_flip_max(int n)
        : data_(make_zero_nodes(n)) {}

    /// O(n)。すべてactiveなvaluesから構築する。
    explicit range_active_add_range_clear_flip_max(const std::vector<T>& values)
        : data_(make_nodes(values)) {}

    /// O(log n)。[left,right)のactive要素だけへdeltaを加える。
    void add(int left, int right, const T& delta) {
        data_.apply(left, right, action{0, delta});
    }

    /// O(log n)。[left,right)の全値を0にしてactive状態を反転する。
    void clear_flip(int left, int right) {
        data_.apply(left, right, action{1, T{}});
    }

    /// O(log n)。a[index]をvalueかつactiveへ置き換える。
    void set(int index, const T& value) {
        data_.set(index, node{value, 1, 0});
    }

    /// O(log n)。a[index]を0かつinactiveへ置き換える。
    void deactivate(int index) {
        data_.set(index, node{std::numeric_limits<T>::lowest(), 0, 1});
    }

    /// O(log n)。[left,right)の全要素の最大値を返す。区間は空でないこと。
    T max(int left, int right) {
        assert(left < right);
        return node_max(data_.prod(left, right));
    }

    /// O(1)。全要素の最大値を返す。要素数は正であること。
    T max() const {
        assert(size() > 0);
        return node_max(data_.all_prod());
    }

    /// O(log n)。index番目がactiveならtrueを返す。
    bool active(int index) {
        return data_[index].active_count == 1;
    }

    /// O(log n)。data[index]はindex番目の値を返す。
    T operator[](int index) {
        const node value = data_[index];
        return value.active_count == 1 ? value.active_max : T{};
    }

    /// O(1)。要素数を返す。
    int size() const noexcept {
        return data_.size();
    }

private:
    static std::vector<node> make_zero_nodes(int n) {
        assert(n >= 0);
        return std::vector<node>(n, node{T{}, 1, 0});
    }

    static std::vector<node> make_nodes(const std::vector<T>& values) {
        std::vector<node> result;
        result.reserve(values.size());
        for (const T& value : values) result.push_back({value, 1, 0});
        return result;
    }

    static T node_max(const node& value) {
        if (value.active_count == 0) return T{};
        if (value.inactive_count == 0) return value.active_max;
        return std::max(value.active_max, T{});
    }

    tree data_;
};

/// 非負整数列の区間切り捨て除算・区間代入・区間和: range_divide_set_range_sum<long long> data(values)。
template<class T>
struct range_divide_set_range_sum {
    static_assert(std::is_integral_v<T> && std::is_signed_v<T>);

    /// O(n)。n個の0から構築する。
    explicit range_divide_set_range_sum(int n)
        : range_divide_set_range_sum(std::vector<T>(n)) {
        assert(n >= 0);
    }

    /// O(n)。非負整数valuesから構築する。
    explicit range_divide_set_range_sum(const std::vector<T>& values)
        : n_(static_cast<int>(values.size())), nodes_(std::max(1, 4 * n_)) {
        assert(std::ranges::all_of(values, [](T value) { return value >= 0; }));
        if (n_ != 0) build(1, 0, n_, values);
    }

    /// 償却O(log n log V)。[left,right)の各a[i]をfloor(a[i]/divisor)へ更新する。divisor>=2。
    void divide(int left, int right, T divisor) {
        assert_range(left, right);
        assert(divisor >= 2);
        if (left != right) range_divide(1, 0, n_, left, right, divisor);
    }

    /// O(log n)。[left,right)の各a[i]を非負整数valueへ代入する。
    void set(int left, int right, T value) {
        assert_range(left, right);
        assert(value >= 0);
        if (left != right) range_set(1, 0, n_, left, right, value);
    }

    /// O(log n)。a[index]を非負整数valueへ代入する。
    void set(int index, T value) {
        set(index, index + 1, value);
    }

    /// O(log n)。[left,right)の和を返す。
    T sum(int left, int right) {
        assert_range(left, right);
        if (left == right) return T{};
        return range_sum(1, 0, n_, left, right);
    }

    /// O(1)。全要素の和を返す。
    T sum() const {
        return n_ == 0 ? T{} : nodes_[1].sum;
    }

    /// O(log n)。data[index]はa[index]を値で返す。
    T operator[](int index) {
        assert(0 <= index && index < n_);
        return point_get(1, 0, n_, index);
    }

    /// O(1)。要素数を返す。
    int size() const noexcept {
        return n_;
    }

private:
    struct node {
        T sum{};
        T minimum{};
        T maximum{};
        T lazy_add{};
        std::optional<T> lazy_set;
        int length = 1;
    };

    void build(int position, int left, int right, const std::vector<T>& values) {
        if (right - left == 1) {
            nodes_[position] = {values[left], values[left], values[left], T{}, std::nullopt, 1};
            return;
        }
        const int middle = (left + right) / 2;
        build(position * 2, left, middle, values);
        build(position * 2 + 1, middle, right, values);
        pull(position);
    }

    void pull(int position) {
        const node& left = nodes_[position * 2];
        const node& right = nodes_[position * 2 + 1];
        nodes_[position] = {
            left.sum + right.sum,
            std::min(left.minimum, right.minimum),
            std::max(left.maximum, right.maximum),
            T{}, std::nullopt, left.length + right.length
        };
    }

    void apply_set(int position, T value) {
        node& current = nodes_[position];
        current.sum = value * T(current.length);
        current.minimum = current.maximum = value;
        current.lazy_add = T{};
        current.lazy_set = value;
    }

    void apply_add(int position, T value) {
        node& current = nodes_[position];
        current.sum += value * T(current.length);
        current.minimum += value;
        current.maximum += value;
        if (current.lazy_set) *current.lazy_set += value;
        else current.lazy_add += value;
    }

    void push(int position) {
        node& current = nodes_[position];
        if (current.length == 1) return;
        if (current.lazy_set) {
            apply_set(position * 2, *current.lazy_set);
            apply_set(position * 2 + 1, *current.lazy_set);
            current.lazy_set.reset();
        }
        if (current.lazy_add != T{}) {
            apply_add(position * 2, current.lazy_add);
            apply_add(position * 2 + 1, current.lazy_add);
            current.lazy_add = T{};
        }
    }

    void range_divide(int position, int left, int right, int query_left, int query_right,
                      T divisor) {
        if (query_right <= left || right <= query_left) return;
        node& current = nodes_[position];
        if (query_left <= left && right <= query_right) {
            const T minimum_quotient = current.minimum / divisor;
            const T maximum_quotient = current.maximum / divisor;
            if (minimum_quotient == maximum_quotient) {
                apply_set(position, minimum_quotient);
                return;
            }
            if (current.minimum - minimum_quotient == current.maximum - maximum_quotient) {
                apply_add(position, minimum_quotient - current.minimum);
                return;
            }
        }
        push(position);
        const int middle = (left + right) / 2;
        range_divide(position * 2, left, middle, query_left, query_right, divisor);
        range_divide(position * 2 + 1, middle, right, query_left, query_right, divisor);
        pull(position);
    }

    void range_set(int position, int left, int right, int query_left, int query_right, T value) {
        if (query_right <= left || right <= query_left) return;
        if (query_left <= left && right <= query_right) {
            apply_set(position, value);
            return;
        }
        push(position);
        const int middle = (left + right) / 2;
        range_set(position * 2, left, middle, query_left, query_right, value);
        range_set(position * 2 + 1, middle, right, query_left, query_right, value);
        pull(position);
    }

    T range_sum(int position, int left, int right, int query_left, int query_right) {
        if (query_right <= left || right <= query_left) return T{};
        if (query_left <= left && right <= query_right) return nodes_[position].sum;
        push(position);
        const int middle = (left + right) / 2;
        return range_sum(position * 2, left, middle, query_left, query_right)
            + range_sum(position * 2 + 1, middle, right, query_left, query_right);
    }

    T point_get(int position, int left, int right, int index) {
        if (right - left == 1) return nodes_[position].sum;
        push(position);
        const int middle = (left + right) / 2;
        if (index < middle) return point_get(position * 2, left, middle, index);
        return point_get(position * 2 + 1, middle, right, index);
    }

    void assert_range(int left, int right) const {
        assert(0 <= left && left <= right && right <= n_);
    }

    int n_;
    std::vector<node> nodes_;
};

/// 区間chmin・chmax・加算・一点代入・区間和: Segment Tree Beatsで各操作を償却O(log n)で行う。
template<class T>
struct range_chmin_chmax_add_range_sum {
    /// O(n)。n個の0から構築する。
    explicit range_chmin_chmax_add_range_sum(int n)
        : range_chmin_chmax_add_range_sum(std::vector<T>(n)) {
        assert(n >= 0);
    }

    /// O(n)。valuesから構築する。
    explicit range_chmin_chmax_add_range_sum(const std::vector<T>& values)
        : n_(static_cast<int>(values.size())), nodes_(std::max(1, 4 * n_)) {
        if (n_ != 0) build(1, 0, n_, values);
    }

    /// 償却O(log n)。[left,right)の各要素をvalue以下へ切り詰める。
    void chmin(int left, int right, const T& value) {
        assert_range(left, right);
        range_chmin(1, 0, n_, left, right, value);
    }

    /// 償却O(log n)。[left,right)の各要素をvalue以上へ切り上げる。
    void chmax(int left, int right, const T& value) {
        assert_range(left, right);
        range_chmax(1, 0, n_, left, right, value);
    }

    /// 償却O(log n)。[left,right)の各要素へvalueを加える。
    void add(int left, int right, const T& value) {
        assert_range(left, right);
        range_add(1, 0, n_, left, right, value);
    }

    /// O(log n)。a[index] = valueを行う。
    void set(int index, const T& value) {
        assert(0 <= index && index < n_);
        point_set(1, 0, n_, index, value);
    }

    /// O(log n)。[left,right)の和を返す。
    T sum(int left, int right) {
        assert_range(left, right);
        return range_sum(1, 0, n_, left, right);
    }

    /// O(1)。全要素の和を返す。
    T sum() const {
        return n_ == 0 ? T{} : nodes_[1].sum;
    }

    /// O(log n)。data[index]はa[index]を返す。
    T operator[](int index) {
        assert(0 <= index && index < n_);
        return point_get(1, 0, n_, index);
    }

    /// O(1)。要素数を返す。
    int size() const noexcept {
        return n_;
    }

private:
    struct node {
        T sum{};
        T maximum{};
        T second_maximum = lowest();
        T minimum{};
        T second_minimum = highest();
        T lazy_add{};
        int maximum_count = 1;
        int minimum_count = 1;
        int length = 1;
    };

    static constexpr T lowest() {
        return std::numeric_limits<T>::lowest();
    }

    static constexpr T highest() {
        return std::numeric_limits<T>::max();
    }

    /// O(n)。Segment Treeを構築する。
    void build(int position, int left, int right, const std::vector<T>& values) {
        if (right - left == 1) {
            nodes_[position] = node{values[left], values[left], lowest(), values[left],
                                    highest(), T{}, 1, 1, 1};
            return;
        }
        const int middle = (left + right) / 2;
        build(position * 2, left, middle, values);
        build(position * 2 + 1, middle, right, values);
        pull(position);
    }

    /// O(1)。子の情報からpositionの情報を更新する。
    void pull(int position) {
        const node& left = nodes_[position * 2];
        const node& right = nodes_[position * 2 + 1];
        node& current = nodes_[position];
        current.sum = left.sum + right.sum;
        current.length = left.length + right.length;
        current.lazy_add = T{};

        if (left.maximum > right.maximum) {
            current.maximum = left.maximum;
            current.maximum_count = left.maximum_count;
            current.second_maximum = std::max(left.second_maximum, right.maximum);
        } else if (left.maximum < right.maximum) {
            current.maximum = right.maximum;
            current.maximum_count = right.maximum_count;
            current.second_maximum = std::max(left.maximum, right.second_maximum);
        } else {
            current.maximum = left.maximum;
            current.maximum_count = left.maximum_count + right.maximum_count;
            current.second_maximum = std::max(left.second_maximum, right.second_maximum);
        }

        if (left.minimum < right.minimum) {
            current.minimum = left.minimum;
            current.minimum_count = left.minimum_count;
            current.second_minimum = std::min(left.second_minimum, right.minimum);
        } else if (left.minimum > right.minimum) {
            current.minimum = right.minimum;
            current.minimum_count = right.minimum_count;
            current.second_minimum = std::min(left.minimum, right.second_minimum);
        } else {
            current.minimum = left.minimum;
            current.minimum_count = left.minimum_count + right.minimum_count;
            current.second_minimum = std::min(left.second_minimum, right.second_minimum);
        }
    }

    /// O(1)。position全体へ加算を適用する。
    void apply_add(int position, const T& value) {
        node& current = nodes_[position];
        current.sum += value * T(current.length);
        current.maximum += value;
        current.minimum += value;
        if (current.second_maximum != lowest()) current.second_maximum += value;
        if (current.second_minimum != highest()) current.second_minimum += value;
        current.lazy_add += value;
    }

    /// O(1)。maximumだけをvalueへ下げる。second_maximum < value < maximumであること。
    void apply_chmin(int position, const T& value) {
        node& current = nodes_[position];
        if (current.maximum <= value) return;
        current.sum += (value - current.maximum) * T(current.maximum_count);
        if (current.minimum == current.maximum) {
            current.minimum = value;
        } else if (current.second_minimum == current.maximum) {
            current.second_minimum = value;
        }
        current.maximum = value;
    }

    /// O(1)。minimumだけをvalueへ上げる。minimum < value < second_minimumであること。
    void apply_chmax(int position, const T& value) {
        node& current = nodes_[position];
        if (value <= current.minimum) return;
        current.sum += (value - current.minimum) * T(current.minimum_count);
        if (current.maximum == current.minimum) {
            current.maximum = value;
        } else if (current.second_maximum == current.minimum) {
            current.second_maximum = value;
        }
        current.minimum = value;
    }

    /// 償却O(1)。遅延情報と親の上下限を子へ伝える。
    void push(int position) {
        node& current = nodes_[position];
        if (current.length == 1) return;
        if (current.lazy_add != T{}) {
            apply_add(position * 2, current.lazy_add);
            apply_add(position * 2 + 1, current.lazy_add);
            current.lazy_add = T{};
        }
        if (nodes_[position * 2].maximum > current.maximum) {
            apply_chmin(position * 2, current.maximum);
        }
        if (nodes_[position * 2 + 1].maximum > current.maximum) {
            apply_chmin(position * 2 + 1, current.maximum);
        }
        if (nodes_[position * 2].minimum < current.minimum) {
            apply_chmax(position * 2, current.minimum);
        }
        if (nodes_[position * 2 + 1].minimum < current.minimum) {
            apply_chmax(position * 2 + 1, current.minimum);
        }
    }

    void range_chmin(int position, int left, int right, int query_left, int query_right,
                     const T& value) {
        if (query_right <= left || right <= query_left || nodes_[position].maximum <= value) return;
        if (query_left <= left && right <= query_right && nodes_[position].second_maximum < value) {
            apply_chmin(position, value);
            return;
        }
        push(position);
        const int middle = (left + right) / 2;
        range_chmin(position * 2, left, middle, query_left, query_right, value);
        range_chmin(position * 2 + 1, middle, right, query_left, query_right, value);
        pull(position);
    }

    void range_chmax(int position, int left, int right, int query_left, int query_right,
                     const T& value) {
        if (query_right <= left || right <= query_left || value <= nodes_[position].minimum) return;
        if (query_left <= left && right <= query_right && value < nodes_[position].second_minimum) {
            apply_chmax(position, value);
            return;
        }
        push(position);
        const int middle = (left + right) / 2;
        range_chmax(position * 2, left, middle, query_left, query_right, value);
        range_chmax(position * 2 + 1, middle, right, query_left, query_right, value);
        pull(position);
    }

    void range_add(int position, int left, int right, int query_left, int query_right,
                   const T& value) {
        if (query_right <= left || right <= query_left) return;
        if (query_left <= left && right <= query_right) {
            apply_add(position, value);
            return;
        }
        push(position);
        const int middle = (left + right) / 2;
        range_add(position * 2, left, middle, query_left, query_right, value);
        range_add(position * 2 + 1, middle, right, query_left, query_right, value);
        pull(position);
    }

    T range_sum(int position, int left, int right, int query_left, int query_right) {
        if (query_right <= left || right <= query_left) return T{};
        if (query_left <= left && right <= query_right) return nodes_[position].sum;
        push(position);
        const int middle = (left + right) / 2;
        return range_sum(position * 2, left, middle, query_left, query_right) +
               range_sum(position * 2 + 1, middle, right, query_left, query_right);
    }

    void point_set(int position, int left, int right, int index, const T& value) {
        if (right - left == 1) {
            nodes_[position] = node{value, value, lowest(), value, highest(), T{}, 1, 1, 1};
            return;
        }
        push(position);
        const int middle = (left + right) / 2;
        if (index < middle) point_set(position * 2, left, middle, index, value);
        else point_set(position * 2 + 1, middle, right, index, value);
        pull(position);
    }

    T point_get(int position, int left, int right, int index) {
        if (right - left == 1) return nodes_[position].sum;
        push(position);
        const int middle = (left + right) / 2;
        if (index < middle) return point_get(position * 2, left, middle, index);
        return point_get(position * 2 + 1, middle, right, index);
    }

    /// O(1)。半開区間が配列内にあることを確認する。
    void assert_range(int left, int right) const {
        assert(0 <= left && left <= right && right <= n_);
    }

    int n_;
    std::vector<node> nodes_;
};

}
