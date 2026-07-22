#pragma once
#include <bits/stdc++.h>
#include "math/gaussian.hpp"

namespace poe {

/// 0/1行列へ長方形XOR制約を追加する: rectangleparitysystem system(h,w)。
struct rectangleparitysystem {
    /// O(1)。height行width列の未知0/1行列を作る。
    rectangleparitysystem(int height, int width)
        : height_(height), width_(width) {
        assert(height >= 0 && width >= 0);
    }

    /// O(1)。半開長方形[top,bottom)x[left,right)のXORをvalueにする。
    void add_constraint(
        int top, int left, int bottom, int right, int value
    ) {
        assert(0 <= top && top <= bottom && bottom <= height_);
        assert(0 <= left && left <= right && right <= width_);
        assert(value == 0 || value == 1);
        constraints_.push_back({top, left, bottom, right, value});
        has_answer_ = false;
    }

    /// O(q^2 min(q,hw)/64+hw)。充足可能ならtrueを返し、0/1行列を復元する。
    bool satisfiable() {
        has_answer_ = false;
        std::map<std::pair<int, int>, int> ids;
        const auto id = [&](int row, int column) {
            const auto point = std::pair{row, column};
            const auto [iterator, inserted] =
                ids.emplace(point, static_cast<int>(ids.size()));
            static_cast<void>(inserted);
            return iterator->second;
        };
        std::vector<std::vector<int>> equations;
        std::vector<int> right;
        equations.reserve(constraints_.size());
        right.reserve(constraints_.size());
        for (const auto& constraint : constraints_) {
            equations.push_back({
                id(constraint.top, constraint.left),
                id(constraint.top, constraint.right),
                id(constraint.bottom, constraint.left),
                id(constraint.bottom, constraint.right),
            });
            right.push_back(constraint.value);
        }
        const auto solution = solve_sparse_binary_linear_one(
            static_cast<int>(ids.size()), equations, right);
        if (!solution) return false;

        std::vector<std::vector<unsigned char>> prefix(
            static_cast<std::size_t>(height_) + 1,
            std::vector<unsigned char>(static_cast<std::size_t>(width_) + 1));
        for (const auto& [point, index] : ids) {
            prefix[point.first][point.second] =
                static_cast<unsigned char>((*solution)[index]);
        }
        answer_.assign(height_, std::vector<int>(width_));
        for (int row = 0; row < height_; ++row) {
            for (int column = 0; column < width_; ++column) {
                answer_[row][column] = prefix[row][column] ^
                    prefix[row + 1][column] ^ prefix[row][column + 1] ^
                    prefix[row + 1][column + 1];
            }
        }
        has_answer_ = true;
        return true;
    }

    /// O(1)。直前のsatisfiable()で得た0/1行列を返す。
    const std::vector<std::vector<int>>& answer() const {
        assert(has_answer_);
        return answer_;
    }

    /// O(1)。直前に得た(row,column)の値を返す。
    int operator()(int row, int column) const {
        assert(has_answer_ && 0 <= row && row < height_);
        assert(0 <= column && column < width_);
        return answer_[row][column];
    }

private:
    struct constraint {
        int top;
        int left;
        int bottom;
        int right;
        int value;
    };

    int height_;
    int width_;
    std::vector<constraint> constraints_;
    std::vector<std::vector<int>> answer_;
    bool has_answer_ = false;
};

}
