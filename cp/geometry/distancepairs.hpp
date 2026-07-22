#pragma once
#include <bits/stdc++.h>
#include "geometry/point.hpp"

namespace poe {

namespace detail {

struct bucketpairhash {
    std::size_t operator()(const std::pair<long long, long long>& value) const noexcept {
        const auto mix = [](unsigned long long x) {
            x += 0x9e3779b97f4a7c15ULL;
            x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
            x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
            return x ^ (x >> 31);
        };
        return mix(value.first) ^ std::rotl(mix(value.second), 32);
    }
};

template<std::signed_integral T>
long long floor_bucket(T value, T width) {
    static_assert(sizeof(T) <= sizeof(long long));
    const long long converted = static_cast<long long>(value);
    const long long divisor = static_cast<long long>(width);
    long long quotient = converted / divisor;
    if (converted % divisor < 0) --quotient;
    return quotient;
}

}

/// 期待O(n+z)時間・O(n+z)領域。Euclid距離がradius以下の全点番号対を辞書順で返す。
template<std::signed_integral T>
std::vector<std::pair<int, int>> pairs_within_distance(
    const std::vector<point<T>>& points,
    T radius
) {
    assert(radius > 0);
    std::unordered_map<
        std::pair<long long, long long>,
        std::vector<int>,
        detail::bucketpairhash
    > buckets;
    buckets.reserve(points.size() * 2);
    std::vector<std::pair<int, int>> result;
    const __int128 radius_square = static_cast<__int128>(radius) * radius;
    for (int index = 0; index < static_cast<int>(points.size()); ++index) {
        const long long bucket_x = detail::floor_bucket(points[index].x, radius);
        const long long bucket_y = detail::floor_bucket(points[index].y, radius);
        for (long long dx = -1; dx <= 1; ++dx) {
            for (long long dy = -1; dy <= 1; ++dy) {
                const auto iterator = buckets.find({bucket_x + dx, bucket_y + dy});
                if (iterator == buckets.end()) continue;
                for (const int previous : iterator->second) {
                    const __int128 x = static_cast<__int128>(points[index].x)
                                     - points[previous].x;
                    const __int128 y = static_cast<__int128>(points[index].y)
                                     - points[previous].y;
                    if (x * x + y * y <= radius_square) {
                        result.emplace_back(previous, index);
                    }
                }
            }
        }
        buckets[{bucket_x, bucket_y}].push_back(index);
    }
    std::ranges::sort(result);
    return result;
}

}
