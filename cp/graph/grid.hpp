#pragma once
#include <bits/stdc++.h>

namespace poe {

/// grid上の(row,column)座標。
struct gridpoint {
    int row;
    int column;
    auto operator<=>(const gridpoint&) const = default;
};

/// gridcomponentsresultは各マスの成分番号・成分サイズ・外周接触を持つ。
struct gridcomponentsresult {
    std::vector<std::vector<int>> component;
    std::vector<int> size;
    std::vector<bool> touches_border;
    /// O(1)。成分数を返す。
    int count() const noexcept { return static_cast<int>(size.size()); }
};

/// 各layer内のgrid成分と、同じ座標で接する隣接layer成分を持つ。
struct layeredgridcomponentsresult {
    std::vector<gridcomponentsresult> layer;
    std::vector<std::vector<std::vector<int>>> up;
    std::vector<std::vector<std::vector<int>>> down;
    /// O(1)。layer数を返す。
    int count() const noexcept { return static_cast<int>(layer.size()); }
};

/// 上下左右の4近傍。
inline constexpr std::array<gridpoint, 4> grid4{{{-1, 0}, {0, 1}, {1, 0}, {0, -1}}};

/// 周囲の8近傍。
inline constexpr std::array<gridpoint, 8> grid8{{
    {-1, -1}, {-1, 0}, {-1, 1}, {0, -1},
    {0, 1}, {1, -1}, {1, 0}, {1, 1}
}};

/// O(1)。(row,column)がheight x widthのgrid内ならtrue。
inline bool inside_grid(int row, int column, int height, int width) noexcept {
    return 0 <= row && row < height && 0 <= column && column < width;
}

/// O(k alpha(k))期待時間。(0,0)から(h-1,w-1)へ疎なblockedを避ける4近傍pathがあればtrue。
inline bool sparse_grid_path_exists(
    int height, int width, const std::vector<gridpoint>& blocked) {
    assert(height > 0 && width > 0);
    const int lower_left = static_cast<int>(blocked.size());
    const int upper_right = lower_left + 1;
    std::vector<int> parent_or_size(lower_left + 2, -1);
    auto leader = [&](auto&& self, int vertex) -> int {
        if (parent_or_size[vertex] < 0) return vertex;
        return parent_or_size[vertex] = self(self, parent_or_size[vertex]);
    };
    auto merge = [&](int first, int second) {
        first = leader(leader, first);
        second = leader(leader, second);
        if (first == second) return;
        if (-parent_or_size[first] < -parent_or_size[second]) {
            std::swap(first, second);
        }
        parent_or_size[first] += parent_or_size[second];
        parent_or_size[second] = first;
    };
    std::unordered_map<long long, int> index;
    index.reserve(2 * blocked.size() + 1);
    auto key = [width](int row, int column) {
        return static_cast<long long>(row) * width + column;
    };
    for (int id = 0; id < static_cast<int>(blocked.size()); ++id) {
        const auto [row, column] = blocked[id];
        assert(inside_grid(row, column, height, width));
        assert(!(row == 0 && column == 0));
        assert(!(row + 1 == height && column + 1 == width));
        assert(index.emplace(key(row, column), id).second);
        if (column == 0 || row + 1 == height) merge(id, lower_left);
        if (row == 0 || column + 1 == width) merge(id, upper_right);
    }
    for (int id = 0; id < static_cast<int>(blocked.size()); ++id) {
        for (const auto direction : grid8) {
            const int row = blocked[id].row + direction.row;
            const int column = blocked[id].column + direction.column;
            if (!inside_grid(row, column, height, width)) continue;
            const auto iterator = index.find(key(row, column));
            if (iterator != index.end()) merge(id, iterator->second);
        }
    }
    return leader(leader, lower_left) != leader(leader, upper_right);
}

/// O(hwK)。passableなマスのdirections連結成分と外周接触を列挙する。
template<class Passable, std::size_t K>
gridcomponentsresult grid_components(
    int height, int width, Passable passable,
    const std::array<gridpoint, K>& directions
) {
    assert(height >= 0 && width >= 0);
    gridcomponentsresult result;
    result.component.assign(height, std::vector<int>(width, -1));
    for (int start_row = 0; start_row < height; ++start_row) {
        for (int start_column = 0; start_column < width; ++start_column) {
            if (!passable(start_row, start_column) ||
                result.component[start_row][start_column] != -1) continue;
            const int id = result.count();
            result.size.push_back(0);
            result.touches_border.push_back(false);
            std::vector<gridpoint> stack{{start_row, start_column}};
            result.component[start_row][start_column] = id;
            while (!stack.empty()) {
                const auto current = stack.back();
                stack.pop_back();
                ++result.size[id];
                result.touches_border[id] = result.touches_border[id] ||
                    current.row == 0 || current.row + 1 == height ||
                    current.column == 0 || current.column + 1 == width;
                for (const auto direction : directions) {
                    const int row = current.row + direction.row;
                    const int column = current.column + direction.column;
                    if (!inside_grid(row, column, height, width) ||
                        result.component[row][column] != -1 || !passable(row, column)) continue;
                    result.component[row][column] = id;
                    stack.push_back({row, column});
                }
            }
        }
    }
    return result;
}

/// O(hw)。passableなマスの上下左右連結成分と外周接触を列挙する。
template<class Passable>
gridcomponentsresult grid_components(int height, int width, Passable passable) {
    return grid_components(height, width, std::move(passable), grid4);
}

/// O(lhw log(hw))。各layerを4近傍成分へ圧縮し、同じ座標で接する上下成分を結ぶ。
template <class Passable>
layeredgridcomponentsresult layered_grid_components(
    int layer_count,
    int height,
    int width,
    Passable passable
) {
    assert(layer_count >= 0 && height >= 0 && width >= 0);
    layeredgridcomponentsresult result;
    result.layer.reserve(layer_count);
    for (int layer = 0; layer < layer_count; ++layer) {
        result.layer.push_back(grid_components(height, width, [&](int row, int column) {
            return passable(layer, row, column);
        }));
    }
    result.up.resize(layer_count);
    result.down.resize(layer_count);
    for (int layer = 0; layer < layer_count; ++layer) {
        result.up[layer].resize(result.layer[layer].count());
        result.down[layer].resize(result.layer[layer].count());
    }
    for (int layer = 0; layer + 1 < layer_count; ++layer) {
        std::vector<std::pair<int, int>> edges;
        edges.reserve(static_cast<std::size_t>(height) * width);
        for (int row = 0; row < height; ++row) {
            for (int column = 0; column < width; ++column) {
                const int lower = result.layer[layer].component[row][column];
                const int upper = result.layer[layer + 1].component[row][column];
                if (lower != -1 && upper != -1) edges.emplace_back(lower, upper);
            }
        }
        std::ranges::sort(edges);
        edges.erase(std::unique(edges.begin(), edges.end()), edges.end());
        for (const auto [lower, upper] : edges) {
            result.up[layer][lower].push_back(upper);
            result.down[layer + 1][upper].push_back(lower);
        }
    }
    return result;
}

/// O(hwK)。複数始点からdirectionsへ進むgrid BFS距離。通れない・到達不能なマスは-1。
template<class Passable, std::size_t K>
std::vector<std::vector<int>> gridbfs(
    int height,
    int width,
    const std::vector<gridpoint>& sources,
    Passable passable,
    const std::array<gridpoint, K>& directions
) {
    assert(height >= 0 && width >= 0);
    std::vector distance(height, std::vector<int>(width, -1));
    std::queue<gridpoint> queue;
    for (const auto source : sources) {
        assert(inside_grid(source.row, source.column, height, width));
        if (!passable(source.row, source.column) || distance[source.row][source.column] != -1) {
            continue;
        }
        distance[source.row][source.column] = 0;
        queue.push(source);
    }
    while (!queue.empty()) {
        const auto current = queue.front();
        queue.pop();
        for (const auto direction : directions) {
            const int row = current.row + direction.row;
            const int column = current.column + direction.column;
            if (!inside_grid(row, column, height, width) || distance[row][column] != -1 ||
                !passable(row, column)) continue;
            distance[row][column] = distance[current.row][current.column] + 1;
            queue.push({row, column});
        }
    }
    return distance;
}

/// O(hw)。複数始点から上下左右へ進むgrid BFS距離。通れない・到達不能なマスは-1。
template<class Passable>
std::vector<std::vector<int>> gridbfs(
    int height,
    int width,
    const std::vector<gridpoint>& sources,
    Passable passable
) {
    return gridbfs(height, width, sources, std::move(passable), grid4);
}

}
