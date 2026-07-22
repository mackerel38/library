#pragma once
#include <bits/stdc++.h>

namespace poe {

/// prefix木上の二多重集合matching: add_left/add_right後、minimum_movesで親移動の最小回数を求める。
template<int Alphabet>
struct prefixmultisetmatcher {
    static_assert(Alphabet >= 1);

    /// O(1)。空列だけを表す根を作る。
    prefixmultisetmatcher() : nodes_(1) {}

    /// O(length)。left側へwordをcount個追加する。各symbolは[0,Alphabet)。
    template<std::ranges::input_range Range>
    void add_left(const Range& word, long long count = 1) {
        add(word, count, true);
    }

    /// O(length)。right側へwordをcount個追加する。各symbolは[0,Alphabet)。
    template<std::ranges::input_range Range>
    void add_right(const Range& word, long long count = 1) {
        add(word, count, false);
    }

    /// O(nodes)。各辺を親へ越えられる条件の下で全要素を対応付ける最小移動回数を返し、不可能ならnullopt。
    template<class LeftMovable, class RightMovable>
    std::optional<long long> minimum_moves(
        LeftMovable left_movable,
        RightMovable right_movable
    ) const {
        std::vector<long long> left(nodes_.size()), right(nodes_.size());
        for (int node = 0; node < static_cast<int>(nodes_.size()); ++node) {
            left[node] = nodes_[node].left;
            right[node] = nodes_[node].right;
        }

        long long moves = 0;
        for (int node = static_cast<int>(nodes_.size()) - 1; node >= 1; --node) {
            const long long matched = std::min(left[node], right[node]);
            left[node] -= matched;
            right[node] -= matched;
            if (left[node] != 0 && !left_movable(nodes_[node].symbol)) return std::nullopt;
            if (right[node] != 0 && !right_movable(nodes_[node].symbol)) return std::nullopt;
            moves += left[node] + right[node];
            left[nodes_[node].parent] += left[node];
            right[nodes_[node].parent] += right[node];
        }
        const long long matched = std::min(left[0], right[0]);
        left[0] -= matched;
        right[0] -= matched;
        if (left[0] != 0 || right[0] != 0) return std::nullopt;
        return moves;
    }

    /// O(1)。prefix木の頂点数を返す。
    int size() const noexcept {
        return static_cast<int>(nodes_.size());
    }

private:
    struct node {
        std::array<int, Alphabet> child;
        int parent = -1;
        int symbol = -1;
        long long left = 0;
        long long right = 0;

        node() {
            child.fill(-1);
        }
    };

    template<std::ranges::input_range Range>
    void add(const Range& word, long long count, bool is_left) {
        assert(count > 0);
        int node = 0;
        for (const auto& raw_symbol : word) {
            const int symbol = static_cast<int>(raw_symbol);
            assert(0 <= symbol && symbol < Alphabet);
            if (nodes_[node].child[symbol] == -1) {
                const int child = static_cast<int>(nodes_.size());
                nodes_[node].child[symbol] = child;
                nodes_.push_back({});
                nodes_.back().parent = node;
                nodes_.back().symbol = symbol;
            }
            node = nodes_[node].child[symbol];
        }
        (is_left ? nodes_[node].left : nodes_[node].right) += count;
    }

    std::vector<node> nodes_;
};

/// O(n bits)時間・領域。sourceの一要素を2倍またはfloor(x/2)にしてtarget多重集合へ変える最小回数を返す。
template<std::unsigned_integral UInt>
std::optional<long long> minimum_double_halve_multiset_operations(
    const std::vector<UInt>& source,
    const std::vector<UInt>& target
) {
    if (source.size() != target.size()) return std::nullopt;
    constexpr int bits = std::numeric_limits<UInt>::digits;
    prefixmultisetmatcher<2> matcher;

    auto add_value = [&](UInt value, bool left) {
        std::array<int, bits> digits{};
        int begin = bits;
        while (value != 0) {
            digits[--begin] = static_cast<int>(value & 1);
            value >>= 1;
        }
        const std::span<const int> word(digits.begin() + begin, digits.end());
        if (left) matcher.add_left(word);
        else matcher.add_right(word);
    };
    for (UInt value : source) add_value(value, true);
    for (UInt value : target) add_value(value, false);
    return matcher.minimum_moves(
        [](int) { return true; },
        [](int last_bit) { return last_bit == 0; }
    );
}

}
