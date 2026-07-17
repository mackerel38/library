#pragma once
#include <bits/stdc++.h>

namespace poe {

/// 静的区間queryを端点一つの更新へ分解する: Mo mo(n); mo.add(l, r); mo.run(...);
struct Mo {
    /// O(1)。長さnの列に対するquery集合を作る。
    explicit Mo(int n) : n_(n) {
        assert(n >= 0);
    }

    /// O(1)。半開区間[left, right)を登録し、answerの添字を返す。
    int add(int left, int right) {
        assert(0 <= left && left <= right && right <= n_);
        const int id = static_cast<int>(queries_.size());
        queries_.push_back({left, right, id});
        return id;
    }

    /// O((n+q)sqrt(q))回程度の端点更新。左右別の追加・削除とanswer(id)を呼ぶ。
    template <class AddLeft, class AddRight, class RemoveLeft, class RemoveRight,
              class Answer>
    void run(AddLeft add_left, AddRight add_right, RemoveLeft remove_left,
             RemoveRight remove_right, Answer answer) const {
        const int query_count = static_cast<int>(queries_.size());
        if (query_count == 0) {
            return;
        }
        const int block_size =
            std::max(1, static_cast<int>(n_ / std::sqrt(query_count)));
        std::vector<query> order = queries_;
        std::sort(order.begin(), order.end(), [&](const query& first, const query& second) {
            const int first_block = first.left / block_size;
            const int second_block = second.left / block_size;
            if (first_block != second_block) {
                return first_block < second_block;
            }
            return first_block & 1 ? first.right > second.right
                                   : first.right < second.right;
        });

        int left = 0;
        int right = 0;
        for (const query& item : order) {
            while (item.left < left) add_left(--left);
            while (right < item.right) add_right(right++);
            while (left < item.left) remove_left(left++);
            while (item.right < right) remove_right(--right);
            answer(item.id);
        }
    }

    /// O((n+q)sqrt(q))回程度の端点更新。左右共通のadd(index), remove(index)を使う。
    template <class Add, class Remove, class Answer>
    void run(Add add, Remove remove, Answer answer) const {
        run(add, add, remove, remove, answer);
    }

    /// O(1)。登録済みquery数を返す。
    int size() const noexcept {
        return static_cast<int>(queries_.size());
    }

private:
    struct query {
        int left;
        int right;
        int id;
    };

    int n_;
    std::vector<query> queries_;
};

/// offlineの二次元点queryを座標一つの更新へ分解する: Mo2d mo(x_limit, y_limit);。
struct Mo2d {
    /// O(1)。0<=x<=x_limit, 0<=y<=y_limitのquery集合を作る。
    Mo2d(int x_limit, int y_limit) : x_limit_(x_limit), y_limit_(y_limit) {
        assert(x_limit >= 0 && y_limit >= 0);
    }

    /// O(1)。点(x,y)を登録し、answerの添字を返す。
    int add(int x, int y) {
        assert(0 <= x && x <= x_limit_ && 0 <= y && y <= y_limit_);
        const int id = static_cast<int>(queries_.size());
        queries_.push_back({x, y, id});
        return id;
    }

    /// O((max(x)+q)sqrt(q))回程度の座標更新。(0,0)から各query点を巡回する。
    template<class IncreaseX, class DecreaseX, class IncreaseY, class DecreaseY, class Answer>
    void run(IncreaseX increase_x, DecreaseX decrease_x,
             IncreaseY increase_y, DecreaseY decrease_y, Answer answer) const {
        const int query_count = static_cast<int>(queries_.size());
        if (query_count == 0) return;
        const int block_size = std::max(
            1, static_cast<int>(std::max(1, x_limit_) / std::sqrt(query_count)));
        std::vector<query> order = queries_;
        std::sort(order.begin(), order.end(), [&](const query& first, const query& second) {
            const int first_block = first.x / block_size;
            const int second_block = second.x / block_size;
            if (first_block != second_block) return first_block < second_block;
            return first_block & 1 ? first.y > second.y : first.y < second.y;
        });

        int x = 0;
        int y = 0;
        for (const query& item : order) {
            while (x < item.x) { increase_x(x); ++x; }
            while (item.x < x) { decrease_x(x); --x; }
            while (y < item.y) { increase_y(y); ++y; }
            while (item.y < y) { decrease_y(y); --y; }
            answer(item.id);
        }
    }

    /// O(1)。登録済みquery数を返す。
    int size() const noexcept {
        return static_cast<int>(queries_.size());
    }

private:
    struct query {
        int x;
        int y;
        int id;
    };

    int x_limit_;
    int y_limit_;
    std::vector<query> queries_;
};

}
