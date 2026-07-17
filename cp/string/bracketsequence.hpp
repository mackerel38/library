#pragma once
#include <bits/stdc++.h>

namespace poe {

/// 末尾push/popできる括弧列: bracketsequence brackets; brackets.push('('); brackets.good()。
struct bracketsequence {
    /// O(1)。空の括弧列を作る。
    bracketsequence() : balance_{0}, minimum_{0} {}

    /// O(n)。'('と')'からなるtextを格納する。
    explicit bracketsequence(std::string_view text) : bracketsequence() {
        reserve(static_cast<int>(text.size()));
        for (const char bracket : text) push(bracket);
    }

    /// 償却O(1)。末尾へbracketを追加する。
    void push(char bracket) {
        assert(bracket == '(' || bracket == ')');
        text_.push_back(bracket);
        balance_.push_back(balance_.back() + (bracket == '(' ? 1 : -1));
        minimum_.push_back(std::min(minimum_.back(), balance_.back()));
    }

    /// O(1)。空でない列の末尾を削除する。
    void pop() {
        assert(!empty());
        text_.pop_back();
        balance_.pop_back();
        minimum_.pop_back();
    }

    /// O(1)。末尾の括弧を返す。
    char back() const {
        assert(!empty());
        return text_.back();
    }

    /// O(1)。index番目の括弧を返す。
    char operator[](int index) const {
        assert(0 <= index && index < size());
        return text_[index];
    }

    /// O(1)。現在の開き括弧数-閉じ括弧数を返す。
    int balance() const noexcept { return balance_.back(); }

    /// O(1)。prefix balanceの最小値を返す。空prefixを含む。
    int minimum_balance() const noexcept { return minimum_.back(); }

    /// O(1)。現在の列が良い括弧列ならtrueを返す。
    bool good() const noexcept { return balance() == 0 && minimum_balance() == 0; }

    /// O(1)。文字数を返す。
    int size() const noexcept { return static_cast<int>(text_.size()); }

    /// O(1)。空ならtrueを返す。
    bool empty() const noexcept { return text_.empty(); }

    /// O(n)。保持領域をcount文字分予約する。
    void reserve(int count) {
        assert(count >= 0);
        text_.reserve(count);
        balance_.reserve(count + 1);
        minimum_.reserve(count + 1);
    }

    /// O(1)。現在の文字列を返す。
    const std::string& string() const noexcept { return text_; }

private:
    std::string text_;
    std::vector<int> balance_;
    std::vector<int> minimum_;
};

}
