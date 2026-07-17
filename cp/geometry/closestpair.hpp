#pragma once
#include <bits/stdc++.h>
#include "geometry/point.hpp"

namespace poe {

/// 最近点対の結果。first,secondは元の点番号、squared_distanceは距離の二乗。
template<class T>
struct closestpairresult {
    int first = -1;
    int second = -1;
    geometrywide<T> squared_distance{};

    /// O(1)。二点以上から有効な点対が得られたか返す。
    bool exists() const noexcept {
        return first != -1;
    }

    /// O(1)。最近点対のEuclid距離を返す。有効な点対があること。
    long double distance() const {
        assert(exists());
        return std::sqrt(static_cast<long double>(squared_distance));
    }
};

namespace detail {

template<class T>
geometrywide<T> squared_point_distance(const point<T>& first, const point<T>& second) {
    const geometrywide<T> dx = geometrywide<T>{first.x} - second.x;
    const geometrywide<T> dy = geometrywide<T>{first.y} - second.y;
    return dx * dx + dy * dy;
}

template<class T>
struct closestpairsolver {
    struct indexedpoint {
        point<T> value;
        int index;
    };

    explicit closestpairsolver(const std::vector<point<T>>& points) {
        values.reserve(points.size());
        for (int index = 0; index < static_cast<int>(points.size()); ++index) {
            values.push_back({points[index], index});
        }
        std::ranges::sort(values, {}, [](const indexedpoint& current) {
            return std::tuple{current.value.x, current.value.y, current.index};
        });
        buffer.resize(values.size());
    }

    closestpairresult<T> solve() {
        return divide(0, static_cast<int>(values.size()));
    }

private:
    using result = closestpairresult<T>;

    /// O(1)。candidateが短ければbestを更新する。
    void update(result& best, const indexedpoint& first, const indexedpoint& second) const {
        const auto squared = squared_point_distance(first.value, second.value);
        if (!best.exists() || squared < best.squared_distance) {
            best = {std::min(first.index, second.index), std::max(first.index, second.index), squared};
        }
    }

    /// O(n log n)。[left,right)の最近点対を求め、範囲をy順に並べ替える。
    result divide(int left, int right) {
        if (right - left <= 3) {
            result best;
            for (int first = left; first < right; ++first) {
                for (int second = first + 1; second < right; ++second) {
                    update(best, values[first], values[second]);
                }
            }
            std::sort(values.begin() + left, values.begin() + right,
                      [](const indexedpoint& first, const indexedpoint& second) {
                          return std::tuple{first.value.y, first.value.x, first.index} <
                                 std::tuple{second.value.y, second.value.x, second.index};
                      });
            return best;
        }

        const int middle = (left + right) / 2;
        const T middle_x = values[middle].value.x;
        result best = divide(left, middle);
        const result right_best = divide(middle, right);
        if (!best.exists() || (right_best.exists() && right_best.squared_distance < best.squared_distance)) {
            best = right_best;
        }
        std::merge(values.begin() + left, values.begin() + middle,
                   values.begin() + middle, values.begin() + right,
                   buffer.begin() + left,
                   [](const indexedpoint& first, const indexedpoint& second) {
                       return std::tuple{first.value.y, first.value.x, first.index} <
                              std::tuple{second.value.y, second.value.x, second.index};
                   });
        std::copy(buffer.begin() + left, buffer.begin() + right, values.begin() + left);

        std::vector<indexedpoint> strip;
        strip.reserve(right - left);
        for (int index = left; index < right; ++index) {
            const geometrywide<T> dx = geometrywide<T>{values[index].value.x} - middle_x;
            if (best.exists() && dx * dx >= best.squared_distance) continue;
            for (int previous = static_cast<int>(strip.size()) - 1; previous >= 0; --previous) {
                const geometrywide<T> dy = geometrywide<T>{values[index].value.y} - strip[previous].value.y;
                if (best.exists() && dy * dy >= best.squared_distance) break;
                update(best, values[index], strip[previous]);
            }
            strip.push_back(values[index]);
        }
        return best;
    }

    std::vector<indexedpoint> values;
    std::vector<indexedpoint> buffer;
};

}

/// O(n log n)。二次元点列の最近点対を元の点番号と距離の二乗付きで返す。
template<class T>
closestpairresult<T> closest_pair(const std::vector<point<T>>& points) {
    return detail::closestpairsolver<T>(points).solve();
}

}
