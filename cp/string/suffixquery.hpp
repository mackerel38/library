#pragma once
#include <bits/stdc++.h>
#include "string/lcp.hpp"
#include "string/suffixarray.hpp"

namespace poe {

/// suffix間LCP・部分文字列比較・pattern検索: suffixquery query(text); 構築O(n log n)。
struct suffixquery {
    /// O(n log n)。textのsuffix array、LCP、RMQを構築する。
    explicit suffixquery(std::string text)
        : text_(std::move(text)), suffix_array_(poe::suffix_array(text_)),
          rank_(text_.size()) {
        for (int i = 0; i < size(); ++i) rank_[suffix_array_[i]] = i;
        const auto adjacent = lcp_array(text_, suffix_array_);
        if (!adjacent.empty()) {
            log_.resize(adjacent.size() + 1);
            for (int i = 2; i < static_cast<int>(log_.size()); ++i)
                log_[i] = log_[i / 2] + 1;
            table_.push_back(adjacent);
            for (int level = 1; (1 << level) <= static_cast<int>(adjacent.size()); ++level) {
                const int width = 1 << level;
                std::vector<int> row(adjacent.size() - width + 1);
                for (int i = 0; i + width <= static_cast<int>(adjacent.size()); ++i)
                    row[i] = std::min(table_[level - 1][i],
                                      table_[level - 1][i + width / 2]);
                table_.push_back(std::move(row));
            }
        }
    }

    /// O(1)。suffix text[first..]とtext[second..]のLCP長を返す。
    int lcp(int first, int second) const {
        assert(0 <= first && first < size());
        assert(0 <= second && second < size());
        if (first == second) return size() - first;
        int left = rank_[first];
        int right = rank_[second];
        if (left > right) std::swap(left, right);
        const int level = log_[right - left];
        return std::min(table_[level][left],
                        table_[level][right - (1 << level)]);
    }

    /// O(1)。二つの部分文字列を辞書順比較し、左が小/同じ/大なら-1/0/1を返す。
    int compare(int first_left, int first_right,
                int second_left, int second_right) const {
        assert(0 <= first_left && first_left <= first_right && first_right <= size());
        assert(0 <= second_left && second_left <= second_right && second_right <= size());
        const int first_length = first_right - first_left;
        const int second_length = second_right - second_left;
        if (first_length == 0 || second_length == 0)
            return (first_length > 0) - (second_length > 0);
        const int common = std::min({lcp(first_left, second_left), first_length, second_length});
        if (common == std::min(first_length, second_length))
            return (first_length > second_length) - (first_length < second_length);
        const unsigned char first = static_cast<unsigned char>(text_[first_left + common]);
        const unsigned char second = static_cast<unsigned char>(text_[second_left + common]);
        return (first > second) - (first < second);
    }

    /// O(1)。左の部分文字列が右より辞書順で小さいならtrueを返す。
    bool less(int first_left, int first_right,
              int second_left, int second_right) const {
        return compare(first_left, first_right, second_left, second_right) < 0;
    }

    /// O(|pattern| log n)。patternで始まるsuffixのSA上半開区間を返す。
    std::pair<int, int> equal_range(const std::string& pattern) const {
        auto compare_suffix = [&](int position, bool upper) {
            int index = 0;
            while (position + index < size() && index < static_cast<int>(pattern.size()) &&
                   text_[position + index] == pattern[index]) ++index;
            if (index == static_cast<int>(pattern.size())) return upper;
            if (position + index == size()) return true;
            return static_cast<unsigned char>(text_[position + index]) <
                   static_cast<unsigned char>(pattern[index]);
        };
        int lower = 0;
        int upper = size();
        while (lower < upper) {
            const int middle = (lower + upper) / 2;
            if (compare_suffix(suffix_array_[middle], false)) lower = middle + 1;
            else upper = middle;
        }
        const int first = lower;
        upper = size();
        while (lower < upper) {
            const int middle = (lower + upper) / 2;
            if (compare_suffix(suffix_array_[middle], true)) lower = middle + 1;
            else upper = middle;
        }
        return {first, lower};
    }

    /// O(|pattern| log n)。textがpatternを含むならtrueを返す。
    bool contains(const std::string& pattern) const {
        if (pattern.empty()) return true;
        const auto [left, right] = equal_range(pattern);
        return left < right;
    }

    /// O(1)。suffix arrayを返す。
    const std::vector<int>& suffix_array() const noexcept { return suffix_array_; }
    /// O(1)。suffix開始位置ごとの順位を返す。
    const std::vector<int>& rank() const noexcept { return rank_; }
    /// O(1)。文字列長を返す。
    int size() const noexcept { return static_cast<int>(text_.size()); }

private:
    std::string text_;
    std::vector<int> suffix_array_;
    std::vector<int> rank_;
    std::vector<int> log_;
    std::vector<std::vector<int>> table_;
};

}
