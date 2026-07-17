#pragma once
#include <bits/stdc++.h>

namespace poe {

/// 区間加算をまとめて反映する差分配列: differencearray<long long> diff(n); add後にbuildする。
template<class T>
struct differencearray {
    /// O(n)。長さnの0列を作る。
    explicit differencearray(int n)
        : difference_(static_cast<std::size_t>(std::max(0, n) + 1)),
          values_(static_cast<std::size_t>(std::max(0, n))) {
        assert(n >= 0);
    }

    /// O(n)。valuesを初期値とする差分配列を作る。
    explicit differencearray(const std::vector<T>& values)
        : difference_(values.size() + 1), values_(values) {
        for (int i = 0; i < size(); ++i) {
            difference_[i] += values[i];
            difference_[i + 1] -= values[i];
        }
        built_ = true;
    }

    /// O(1)。要素数を返す。
    int size() const noexcept { return static_cast<int>(values_.size()); }

    /// O(1)。半開区間[left,right)へvalueを加える。取得前にbuildを呼ぶ。
    void add(int left, int right, const T& value) {
        assert(0 <= left && left <= right && right <= size());
        difference_[left] += value;
        difference_[right] -= value;
        built_ = false;
    }

    /// O(n)。追加をすべて反映し、operator[]で取得できるようにする。
    void build() {
        T current{};
        for (int i = 0; i < size(); ++i) {
            current += difference_[i];
            values_[i] = current;
        }
        built_ = true;
    }

    /// O(1)。build後のindex番目を返す。
    const T& operator[](int index) const {
        assert(built_ && 0 <= index && index < size());
        return values_[index];
    }

    /// O(1)。build後の列を返す。
    const std::vector<T>& values() const {
        assert(built_);
        return values_;
    }

private:
    std::vector<T> difference_;
    std::vector<T> values_;
    bool built_ = false;
};

/// 長方形加算をまとめて反映する2次元差分配列: differencearray2d<int> diff(h,w); add後にbuildする。
template <class T>
struct differencearray2d {
    /// O(hw)。height行width列の0表を作る。
    differencearray2d(int height, int width)
        : height_(height), width_(width),
          difference_(std::max(0, height) + 1, std::vector<T>(std::max(0, width) + 1)),
          values_(std::max(0, height), std::vector<T>(std::max(0, width))) {
        assert(height >= 0 && width >= 0);
    }

    /// O(1)。行数を返す。
    int height() const noexcept { return height_; }

    /// O(1)。列数を返す。
    int width() const noexcept { return width_; }

    /// O(1)。半開長方形[top,bottom)x[left,right)へvalueを加える。取得前にbuildを呼ぶ。
    void add(int top, int left, int bottom, int right, const T& value) {
        assert(0 <= top && top <= bottom && bottom <= height_);
        assert(0 <= left && left <= right && right <= width_);
        difference_[top][left] += value;
        difference_[top][right] -= value;
        difference_[bottom][left] -= value;
        difference_[bottom][right] += value;
        built_ = false;
    }

    /// O(hw)。追加をすべて反映し、operator()で取得できるようにする。
    void build() {
        for (int row = 0; row < height_; ++row) {
            for (int column = 0; column < width_; ++column) {
                values_[row][column] = difference_[row][column];
                if (row > 0) values_[row][column] += values_[row - 1][column];
                if (column > 0) values_[row][column] += values_[row][column - 1];
                if (row > 0 && column > 0) values_[row][column] -= values_[row - 1][column - 1];
            }
        }
        built_ = true;
    }

    /// O(1)。build後の(row,column)を返す。
    const T& operator()(int row, int column) const {
        assert(built_ && 0 <= row && row < height_ && 0 <= column && column < width_);
        return values_[row][column];
    }

    /// O(1)。build後の表を返す。
    const std::vector<std::vector<T>>& values() const {
        assert(built_);
        return values_;
    }

private:
    int height_;
    int width_;
    std::vector<std::vector<T>> difference_;
    std::vector<std::vector<T>> values_;
    bool built_ = false;
};

/// 静的な列の区間和: prefixsum<long long> ps(values); 構築O(n)、取得O(1)。
template <class T>
struct prefixsum {
    /// O(n)。valuesから累積和を構築する。
    explicit prefixsum(const std::vector<T>& values)
        : values_(values), sums_(values.size() + 1) {
        for (int index = 0; index < size(); ++index) {
            sums_[index + 1] = sums_[index] + values_[index];
        }
    }

    /// O(1)。要素数を返す。
    int size() const noexcept {
        return static_cast<int>(values_.size());
    }

    /// O(1)。ps[index]で元の列の要素を返す。
    const T& operator[](int index) const {
        assert(0 <= index && index < size());
        return values_[index];
    }

    /// O(1)。ps.sum(right)で[0, right)の和を返す。
    T sum(int right) const {
        assert(0 <= right && right <= size());
        return sums_[right];
    }

    /// O(1)。ps.sum(left, right)で[left, right)の和を返す。
    T sum(int left, int right) const {
        assert(0 <= left && left <= right && right <= size());
        return sums_[right] - sums_[left];
    }

    /// O(1)。ps.sum()で全要素の和を返す。
    T sum() const {
        return sums_.back();
    }

private:
    std::vector<T> values_;
    std::vector<T> sums_;
};

/// 全部分配列和の静的区間query: subarrayprefixsum<long long> sums(values); sum(l,r)は[l,r)内。
template <class T>
struct subarrayprefixsum {
    /// O(n)。valuesと添字一次・二次momentの累積和を構築する。
    explicit subarrayprefixsum(const std::vector<T>& values)
        : values_(values) {
        for (auto& sum : sums_) sum.resize(values.size() + 1);
        for (int index = 0; index < size(); ++index) {
            const long long position = index;
            sums_[0][index + 1] = sums_[0][index] + values[index];
            sums_[1][index + 1] = sums_[1][index] + values[index] * position;
            sums_[2][index + 1] = sums_[2][index] + values[index] * position * position;
        }
    }

    /// O(1)。要素数を返す。
    int size() const noexcept {
        return static_cast<int>(values_.size());
    }

    /// O(1)。元の列のindex番目を返す。
    const T& operator[](int index) const {
        assert(0 <= index && index < size());
        return values_[index];
    }

    /// O(1)。values[left:right]の全非空部分配列の要素和を合計して返す。
    T sum(int left, int right) const {
        assert(0 <= left && left <= right && right <= size());
        const T value = range_sum(0, left, right);
        const T first = range_sum(1, left, right);
        const T second = range_sum(2, left, right);
        return value * (1LL - left) * right +
               first * (left + right - 1LL) - second;
    }

    /// O(1)。列全体の全非空部分配列の要素和を合計して返す。
    T sum() const {
        return sum(0, size());
    }

private:
    T range_sum(int degree, int left, int right) const {
        return sums_[degree][right] - sums_[degree][left];
    }

    std::vector<T> values_;
    std::array<std::vector<T>, 3> sums_;
};

/// 静的円環列の左回転と区間和: circularprefixsum<long long> data(values)。
template <class T>
struct circularprefixsum {
    /// O(n)。非空列valuesから構築する。
    explicit circularprefixsum(const std::vector<T>& values)
        : data_(doubled(values)), size_(static_cast<int>(values.size())) {
        assert(!values.empty());
    }

    /// O(1)。論理列をleft個だけ左回転する。負なら右回転する。
    void rotate(long long left) {
        left %= size_;
        if (left < 0) left += size_;
        offset_ = (offset_ + static_cast<int>(left)) % size_;
    }

    /// O(1)。現在の論理列の[0,right)の和を返す。
    T sum(int right) const {
        return sum(0, right);
    }

    /// O(1)。現在の論理列の[left,right)の和を返す。
    T sum(int left, int right) const {
        assert(0 <= left && left <= right && right <= size_);
        return data_.sum(offset_ + left, offset_ + right);
    }

    /// O(1)。全要素の和を返す。
    T sum() const {
        return data_.sum(0, size_);
    }

    /// O(1)。data[index]は現在の論理列の要素を返す。
    const T& operator[](int index) const {
        assert(0 <= index && index < size_);
        return data_[offset_ + index];
    }

    /// O(1)。要素数を返す。
    int size() const noexcept {
        return size_;
    }

    /// O(1)。元の列からの左回転量を返す。
    int offset() const noexcept {
        return offset_;
    }

private:
    static std::vector<T> doubled(const std::vector<T>& values) {
        std::vector<T> result;
        result.reserve(2 * values.size());
        result.insert(result.end(), values.begin(), values.end());
        result.insert(result.end(), values.begin(), values.end());
        return result;
    }

    prefixsum<T> data_;
    int size_;
    int offset_ = 0;
};

/// 静的な長方形領域の和: prefixsum2d<long long> ps(grid); 構築O(hw)、取得O(1)。
template <class T>
struct prefixsum2d {
    /// O(hw)。長方形のgridから2次元累積和を構築する。
    explicit prefixsum2d(const std::vector<std::vector<T>>& grid)
        : values_(grid), height_(static_cast<int>(grid.size())),
          width_(grid.empty() ? 0 : static_cast<int>(grid.front().size())),
          sums_(height_ + 1, std::vector<T>(width_ + 1)) {
        for (const auto& row : grid) {
            assert(static_cast<int>(row.size()) == width_);
        }
        for (int row = 0; row < height_; ++row) {
            for (int column = 0; column < width_; ++column) {
                sums_[row + 1][column + 1] =
                    sums_[row][column + 1] + sums_[row + 1][column] -
                    sums_[row][column] + values_[row][column];
            }
        }
    }

    /// O(1)。行数を返す。
    int height() const noexcept {
        return height_;
    }

    /// O(1)。列数を返す。
    int width() const noexcept {
        return width_;
    }

    /// O(1)。ps(row, column)で元の表の要素を返す。
    const T& operator()(int row, int column) const {
        assert(0 <= row && row < height_ && 0 <= column && column < width_);
        return values_[row][column];
    }

    /// O(1)。ps.sum(bottom, right)で[0,bottom)x[0,right)の和を返す。
    T sum(int bottom, int right) const {
        return sum(0, 0, bottom, right);
    }

    /// O(1)。ps.sum(top, left, bottom, right)で半開長方形の和を返す。
    T sum(int top, int left, int bottom, int right) const {
        assert(0 <= top && top <= bottom && bottom <= height_);
        assert(0 <= left && left <= right && right <= width_);
        return sums_[bottom][right] - sums_[top][right] - sums_[bottom][left] +
               sums_[top][left];
    }

    /// O(1)。ps.sum()で全要素の和を返す。
    T sum() const {
        return sums_[height_][width_];
    }

private:
    std::vector<std::vector<T>> values_;
    int height_;
    int width_;
    std::vector<std::vector<T>> sums_;
};

/// 静的な直方体領域の和: multidimensionalprefixsum<long long> ps(shape, values); 構築O(dN)。
template<class T>
struct multidimensionalprefixsum {
    /// O(dN)。各次元の長さshapeとrow-major順のvaluesから多次元累積和を構築する。
    multidimensionalprefixsum(std::vector<int> shape, std::vector<T> values)
        : shape_(std::move(shape)), values_(std::move(values)), strides_(shape_.size()) {
        assert(!shape_.empty() && shape_.size() < 63);
        long long total = 1;
        for (int extent : shape_) {
            assert(extent >= 0);
            total *= extent;
            assert(total <= std::numeric_limits<int>::max());
        }
        assert(static_cast<int>(values_.size()) == total);
        int stride = 1;
        for (int dimension = dimensions() - 1; dimension >= 0; --dimension) {
            strides_[dimension] = stride;
            stride *= shape_[dimension];
        }
        sums_ = values_;
        for (int dimension = 0; dimension < dimensions(); ++dimension) {
            const int current_stride = strides_[dimension];
            for (int index = 0; index < size(); ++index) {
                if (shape_[dimension] == 0 ||
                    index / current_stride % shape_[dimension] == 0) continue;
                sums_[index] += sums_[index - current_stride];
            }
        }
    }

    /// O(1)。次元数を返す。
    int dimensions() const noexcept { return static_cast<int>(shape_.size()); }

    /// O(1)。全要素数を返す。
    int size() const noexcept { return static_cast<int>(values_.size()); }

    /// O(1)。各次元の長さを返す。
    const std::vector<int>& shape() const noexcept { return shape_; }

    /// O(1)。ps[index]でrow-major順index番目の元要素を返す。
    const T& operator[](int index) const {
        assert(0 <= index && index < size());
        return values_[index];
    }

    /// O(d)。ps(coordinates)で指定座標の元要素を返す。
    const T& operator()(const std::vector<int>& coordinates) const {
        return values_[flat_index(coordinates)];
    }

    /// O(d)。各次元の[0,upper[d])からなる直方体領域の和を返す。
    T sum(const std::vector<int>& upper) const {
        assert_bounds(upper);
        if (std::ranges::find(upper, 0) != upper.end()) return T{};
        int index = 0;
        for (int dimension = 0; dimension < dimensions(); ++dimension) {
            index += (upper[dimension] - 1) * strides_[dimension];
        }
        return sums_[index];
    }

    /// O(d 2^d)。各次元の[lower[d],upper[d])からなる直方体領域の和を返す。
    T sum(const std::vector<int>& lower, const std::vector<int>& upper) const {
        assert_bounds(lower);
        assert_bounds(upper);
        for (int dimension = 0; dimension < dimensions(); ++dimension) {
            assert(lower[dimension] <= upper[dimension]);
        }
        T result{};
        std::vector<int> bound(dimensions());
        const std::uint64_t count = std::uint64_t{1} << dimensions();
        for (std::uint64_t mask = 0; mask < count; ++mask) {
            for (int dimension = 0; dimension < dimensions(); ++dimension) {
                bound[dimension] = mask >> dimension & 1U
                    ? lower[dimension] : upper[dimension];
            }
            const T value = sum(bound);
            if (std::popcount(mask) & 1) result -= value;
            else result += value;
        }
        return result;
    }

    /// O(1)。全要素の和を返す。
    T sum() const { return sums_.empty() ? T{} : sums_.back(); }

private:
    /// O(d)。座標をrow-major順の添字へ変換する。
    int flat_index(const std::vector<int>& coordinates) const {
        assert(static_cast<int>(coordinates.size()) == dimensions());
        int result = 0;
        for (int dimension = 0; dimension < dimensions(); ++dimension) {
            assert(0 <= coordinates[dimension] && coordinates[dimension] < shape_[dimension]);
            result += coordinates[dimension] * strides_[dimension];
        }
        return result;
    }

    /// O(d)。各次元の半開区間端が範囲内であることを確認する。
    void assert_bounds(const std::vector<int>& bounds) const {
        assert(static_cast<int>(bounds.size()) == dimensions());
        for (int dimension = 0; dimension < dimensions(); ++dimension) {
            assert(0 <= bounds[dimension] && bounds[dimension] <= shape_[dimension]);
        }
    }

    std::vector<int> shape_;
    std::vector<T> values_;
    std::vector<int> strides_;
    std::vector<T> sums_;
};

}
