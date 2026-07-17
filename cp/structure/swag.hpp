#pragma once
#include <bits/stdc++.h>

namespace poe {

/// queue全体のモノイド積: swag<S, op, e> queue; 各操作ならしO(1)。
template <class S, auto op, auto e>
struct swag {
    /// O(1)。空のqueueを構築する。
    swag() = default;

    /// O(1)。要素数を返す。
    int size() const noexcept {
        return static_cast<int>(input_.size() + output_.size());
    }

    /// O(1)。空ならtrueを返す。
    bool empty() const noexcept {
        return input_.empty() && output_.empty();
    }

    /// O(1)。末尾へvalueを追加する。
    void push(const S& value) {
        const S aggregate = input_.empty() ? value : op(input_.back().second, value);
        input_.emplace_back(value, aggregate);
    }

    /// ならしO(1)。先頭要素を削除する。空でないこと。
    void pop() {
        move_to_output();
        assert(!output_.empty());
        output_.pop_back();
    }

    /// ならしO(1)。先頭要素を返す。空でないこと。
    const S& front() {
        move_to_output();
        assert(!output_.empty());
        return output_.back().first;
    }

    /// O(1)。末尾要素を返す。空でないこと。
    const S& back() const {
        assert(!empty());
        return input_.empty() ? output_.front().first : input_.back().first;
    }

    /// O(1)。先頭から順にopで畳み込んだ値を返す。空ならe()。
    S prod() const {
        if (output_.empty()) {
            return input_.empty() ? e() : input_.back().second;
        }
        if (input_.empty()) {
            return output_.back().second;
        }
        return op(output_.back().second, input_.back().second);
    }

private:
    /// 合計O(n)。input_の要素を順序を保ってoutput_へ移す。
    void move_to_output() {
        if (!output_.empty()) {
            return;
        }
        while (!input_.empty()) {
            S value = std::move(input_.back().first);
            input_.pop_back();
            const S aggregate =
                output_.empty() ? value : op(value, output_.back().second);
            output_.emplace_back(std::move(value), aggregate);
        }
    }

    std::vector<std::pair<S, S>> input_;
    std::vector<std::pair<S, S>> output_;
};

}
