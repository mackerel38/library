#pragma once
#include <bits/stdc++.h>
#include "structure/segtree.hpp"

namespace poe {

namespace detail {

template <int Height>
using monotonegridpathmatrix = std::array<std::array<int, Height>, Height>;

inline constexpr int monotonegridpathinfinity = std::numeric_limits<int>::max() / 4;

template <int Height>
monotonegridpathmatrix<Height> monotone_grid_path_identity() {
    monotonegridpathmatrix<Height> result{};
    for (auto& row : result) row.fill(monotonegridpathinfinity);
    for (int row = 0; row < Height; ++row) result[row][row] = 0;
    return result;
}

template <int Height>
monotonegridpathmatrix<Height> monotone_grid_path_compose(
    const monotonegridpathmatrix<Height>& left,
    const monotonegridpathmatrix<Height>& right
) {
    monotonegridpathmatrix<Height> result{};
    for (auto& row : result) row.fill(monotonegridpathinfinity);
    for (int from = 0; from < Height; ++from) {
        for (int middle = 0; middle < Height; ++middle) {
            if (left[from][middle] == monotonegridpathinfinity) continue;
            for (int to = 0; to < Height; ++to) {
                result[from][to] = std::min(
                    result[from][to], left[from][middle] + right[middle][to]
                );
            }
        }
    }
    return result;
}

}

/// 固定高さgridを列方向に逆戻りしない最短路: monotonegridshortestpath<H> path(cells)。
template <int Height>
struct monotonegridshortestpath {
    static_assert(Height > 0);
    using column_type = std::array<bool, Height>;
    using matrix_type = detail::monotonegridpathmatrix<Height>;

    /// O(Height^3 * width)。trueを通行可能として列列から構築する。
    explicit monotonegridshortestpath(const std::vector<column_type>& columns)
        : columns_(columns), tree_(make_matrices(columns)) {}

    /// O(Height^3 * width)。'.'を通行可能、wallを壁として行文字列から構築する。
    explicit monotonegridshortestpath(
        const std::vector<std::string>& cells,
        char wall = '#'
    ) : monotonegridshortestpath(make_columns(cells, wall)) {}

    /// O(1)。gridの幅を返す。
    int width() const noexcept { return static_cast<int>(columns_.size()); }

    /// O(1)。(row,column)が通行可能ならtrueを返す。
    bool get(int row, int column) const {
        assert_cell(row, column);
        return columns_[column][row];
    }

    /// O(1)。path[column]はその列の通行可否を返す。
    const column_type& operator[](int column) const {
        assert(0 <= column && column < width());
        return columns_[column];
    }

    /// O(Height^3 log width)。(row,column)の通行可否をpassableへ変更する。
    void set(int row, int column, bool passable) {
        assert_cell(row, column);
        columns_[column][row] = passable;
        tree_.set(column, make_matrix(columns_[column]));
    }

    /// O(Height^3 log width)。(row,column)の通行可否を反転する。
    void toggle(int row, int column) {
        set(row, column, !get(row, column));
    }

    /// O(Height^3 log width)。列を左へ戻らない最短距離を返し、到達不能なら-1。
    int distance(int from_row, int from_column, int to_row, int to_column) const {
        assert_cell(from_row, from_column);
        assert_cell(to_row, to_column);
        assert(from_column <= to_column);
        const auto transition = tree_.prod(from_column, to_column + 1);
        const int vertical = transition[from_row][to_row];
        if (vertical == detail::monotonegridpathinfinity) return -1;
        return vertical + to_column - from_column;
    }

private:
    using tree_type = segtree<
        matrix_type,
        detail::monotone_grid_path_compose<Height>,
        detail::monotone_grid_path_identity<Height>
    >;

    static matrix_type make_matrix(const column_type& column) {
        matrix_type result{};
        for (auto& row : result) row.fill(detail::monotonegridpathinfinity);
        for (int from = 0; from < Height; ++from) {
            if (!column[from]) continue;
            result[from][from] = 0;
            for (int to = from + 1; to < Height && column[to]; ++to) {
                result[from][to] = result[to][from] = to - from;
            }
        }
        return result;
    }

    static std::vector<matrix_type> make_matrices(
        const std::vector<column_type>& columns
    ) {
        std::vector<matrix_type> result;
        result.reserve(columns.size());
        for (const auto& column : columns) result.push_back(make_matrix(column));
        return result;
    }

    static std::vector<column_type> make_columns(
        const std::vector<std::string>& cells,
        char wall
    ) {
        assert(static_cast<int>(cells.size()) == Height);
        const int width = cells.empty() ? 0 : static_cast<int>(cells[0].size());
        for (const auto& row : cells) assert(static_cast<int>(row.size()) == width);
        std::vector<column_type> columns(width);
        for (int column = 0; column < width; ++column) {
            for (int row = 0; row < Height; ++row) {
                columns[column][row] = cells[row][column] != wall;
            }
        }
        return columns;
    }

    void assert_cell(int row, int column) const {
        assert(0 <= row && row < Height);
        assert(0 <= column && column < width());
    }

    std::vector<column_type> columns_;
    tree_type tree_;
};

}
