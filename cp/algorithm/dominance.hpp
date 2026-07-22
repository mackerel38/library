#pragma once
#include <bits/stdc++.h>
#include "structure/fenwicktree.hpp"

namespace poe {

/// 三次元座標と重みを持つ点。maximum_weight_dominance_chain_3dへ渡す。
template <class Coordinate, class Weight>
struct weightedpoint3d {
    Coordinate x;
    Coordinate y;
    Coordinate z;
    Weight weight;
};

/// 最大重みdominance chainの結果。indicesは入力添字による最適chain。
template <class Weight>
struct dominancechainresult {
    Weight weight;
    std::vector<Weight> ending;
    std::vector<int> parent;
    std::vector<int> indices;
};

namespace detail {

template <class Weight>
struct dominancevalue {
    Weight weight{};
    int index = -1;
};

template <class Weight>
dominancevalue<Weight> dominance_max(
    const dominancevalue<Weight>& left,
    const dominancevalue<Weight>& right
) {
    return left.weight < right.weight ? right : left;
}

template <class Weight>
dominancevalue<Weight> dominance_identity() {
    return {};
}

}

/// O(n log^2 n)時間・O(n log n)領域。三座標が全て非減少な部分列の最大重みと復元を返す。
/// 入力順には依存せず、weightは非負であること。空chainの重みは0。
template <class Coordinate, class Weight>
dominancechainresult<Weight> maximum_weight_dominance_chain_3d(
    const std::vector<weightedpoint3d<Coordinate, Weight>>& points
) {
    using node = detail::dominancevalue<Weight>;
    const int n = static_cast<int>(points.size());
    std::vector<std::pair<Coordinate, Coordinate>> update_points;
    update_points.reserve(points.size());
    for (const auto& point : points) {
        assert(!(point.weight < Weight{}));
        update_points.emplace_back(point.y, point.z);
    }
    fenwicktree2d<Coordinate, node, detail::dominance_max<Weight>,
                  detail::dominance_identity<Weight>> data(update_points);

    std::vector<int> order(n);
    std::iota(order.begin(), order.end(), 0);
    std::ranges::sort(order, {}, [&](int index) {
        return std::tuple{points[index].x, points[index].y, points[index].z, index};
    });

    dominancechainresult<Weight> result{
        Weight{}, std::vector<Weight>(n), std::vector<int>(n, -1), {}
    };
    node best;
    for (const int index : order) {
        const auto& point = points[index];
        const node previous = data.prod(point.y, point.z);
        result.ending[index] = previous.weight + point.weight;
        result.parent[index] = previous.index;
        const node current{result.ending[index], index};
        data.apply(point.y, point.z, current);
        best = detail::dominance_max(best, current);
    }
    result.weight = best.weight;
    for (int index = best.index; index != -1; index = result.parent[index]) {
        result.indices.push_back(index);
    }
    std::ranges::reverse(result.indices);
    return result;
}

}
