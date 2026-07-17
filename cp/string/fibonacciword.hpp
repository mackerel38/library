#pragma once
#include <bits/stdc++.h>

namespace poe {

/// 巨大Fibonacci wordの文字数・添字: fibonacciword word(first,second,max_length)。構築O(256 log max_length)。
struct fibonacciword {
    /// O(|first|+|second|+256 log max_length)。必要なprefix長まで前計算する。
    fibonacciword(std::string first, std::string second,
                  unsigned long long max_length = std::numeric_limits<unsigned long long>::max())
        : bases_{std::move(first), std::move(second)}, cap_(max_length) {
        assert(!bases_[0].empty() && !bases_[1].empty() && max_length >= 1);
        length_.push_back(std::min<unsigned long long>(bases_[0].size(), cap_));
        length_.push_back(std::min<unsigned long long>(bases_[1].size(), cap_));
        count_.resize(2);
        for (int base = 0; base < 2; ++base)
            for (unsigned long long i = 0; i < length_[base]; ++i)
                ++count_[base][static_cast<unsigned char>(bases_[base][i])];
        while (length_.back() < cap_) {
            length_.push_back(std::min(cap_, saturated_add(length_.back(), length_[length_.size() - 2])));
            std::array<unsigned long long, 256> total{};
            for (int c = 0; c < 256; ++c)
                total[c] = saturated_add(count_.back()[c], count_[count_.size() - 2][c]);
            count_.push_back(total);
        }
    }

    /// O(log max_length)。十分大きいS_iの先頭right文字にcharacterが現れる回数を返す。
    unsigned long long count(char character, unsigned long long right) const {
        assert(right <= cap_);
        unsigned long long answer = 0;
        int level = static_cast<int>(length_.size()) - 1;
        const unsigned char target = static_cast<unsigned char>(character);
        while (level >= 2) {
            if (right <= length_[level - 1]) {
                --level;
            } else {
                answer += count_[level - 1][target];
                right -= length_[level - 1];
                level -= 2;
            }
        }
        const auto prefix = bases_[level].substr(0, static_cast<std::size_t>(right));
        answer += std::ranges::count(prefix, character);
        return answer;
    }

    /// O(log max_length)。半開区間[left,right)のcharacter出現回数を返す。
    unsigned long long count(char character, unsigned long long left,
                             unsigned long long right) const {
        assert(left <= right);
        return count(character, right) - count(character, left);
    }

    /// O(log max_length)。十分大きいS_iの0-indexed position文字目を返す。
    char operator[](unsigned long long position) const {
        assert(position < cap_);
        int level = static_cast<int>(length_.size()) - 1;
        while (level >= 2) {
            if (position < length_[level - 1]) {
                --level;
            } else {
                position -= length_[level - 1];
                level -= 2;
            }
        }
        return bases_[level][position];
    }

private:
    unsigned long long saturated_add(unsigned long long first,
                                     unsigned long long second) const {
        return first >= cap_ - std::min(cap_, second) ? cap_ : first + second;
    }
    std::array<std::string, 2> bases_;
    unsigned long long cap_;
    std::vector<unsigned long long> length_;
    std::vector<std::array<unsigned long long, 256>> count_;
};

}
