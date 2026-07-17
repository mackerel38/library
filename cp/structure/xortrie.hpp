#pragma once
#include <bits/stdc++.h>

namespace poe {

/// 整数multisetのXOR検索: xortrie<unsigned,30> trie; 各操作O(bits)。
template <std::unsigned_integral UInt = unsigned, int Bits = std::numeric_limits<UInt>::digits>
struct xortrie {
    static_assert(1 <= Bits && Bits <= std::numeric_limits<UInt>::digits);

    /// O(1)。空のtrieを作る。
    xortrie() { nodes_.push_back({}); }

    /// O(bits)。valueをcount個追加する。負数countは既存要素の削除に使える。
    void add(UInt value, int count = 1) {
        assert(count != 0 && size() + count >= 0);
        int node = 0;
        nodes_[node].count += count;
        for (int bit = Bits - 1; bit >= 0; --bit) {
            const int direction = (value >> bit) & 1;
            if (nodes_[node].child[direction] == -1) {
                assert(count > 0);
                nodes_[node].child[direction] = static_cast<int>(nodes_.size());
                nodes_.push_back({});
            }
            node = nodes_[node].child[direction];
            nodes_[node].count += count;
            assert(nodes_[node].count >= 0);
        }
    }

    /// O(bits)。valueを一つ削除する。
    void erase(UInt value) { assert(count(value) > 0); add(value, -1); }

    /// O(bits)。valueの個数を返す。
    int count(UInt value) const {
        int node = 0;
        for (int bit = Bits - 1; bit >= 0; --bit) {
            node = nodes_[node].child[(value >> bit) & 1];
            if (node == -1) return 0;
        }
        return nodes_[node].count;
    }

    /// O(bits)。value xor xがlimit未満となる格納値xの個数を返す。
    long long count_xor_less(UInt value, UInt limit) const {
        if constexpr (Bits < std::numeric_limits<UInt>::digits) {
            if (limit >= (UInt{1} << Bits)) return size();
        }
        long long answer = 0;
        int node = 0;
        for (int bit = Bits - 1; bit >= 0 && node != -1; --bit) {
            const int value_bit = (value >> bit) & 1;
            if ((limit >> bit) & 1) {
                const int same = nodes_[node].child[value_bit];
                if (same != -1) answer += nodes_[same].count;
                node = nodes_[node].child[value_bit ^ 1];
            } else {
                node = nodes_[node].child[value_bit];
            }
        }
        return answer;
    }

    /// O(bits)。value xor xがlimit以下となる格納値xの個数を返す。
    long long count_xor_at_most(UInt value, UInt limit) const {
        if constexpr (Bits < std::numeric_limits<UInt>::digits) {
            if (limit >= (UInt{1} << Bits) - 1) return size();
        } else if (limit == std::numeric_limits<UInt>::max()) {
            return size();
        }
        return count_xor_less(value, limit + 1);
    }

    /// O(bits)。格納値xに対するvalue xor xの最小値を返す。空でないことが必要。
    UInt min_xor(UInt value) const {
        assert(!empty());
        UInt answer = 0;
        int node = 0;
        for (int bit = Bits - 1; bit >= 0; --bit) {
            const int direction = (value >> bit) & 1;
            const int same = nodes_[node].child[direction];
            if (same != -1 && nodes_[same].count > 0) {
                node = same;
            } else {
                node = nodes_[node].child[direction ^ 1];
                assert(node != -1 && nodes_[node].count > 0);
                answer |= UInt{1} << bit;
            }
        }
        return answer;
    }

    /// O(1)。重複込み要素数を返す。
    int size() const noexcept { return nodes_[0].count; }

    /// O(1)。空ならtrueを返す。
    bool empty() const noexcept { return size() == 0; }

private:
    struct node {
        std::array<int, 2> child{-1, -1};
        int count = 0;
    };
    std::vector<node> nodes_;
};

namespace detail {

template<std::unsigned_integral UInt, int Bits>
struct minxorsumtrie {
    struct node {
        std::array<int, 2> child{-1, -1};
        unsigned long long full_sum = 0;
    };

    std::vector<node> nodes{{}};

    explicit minxorsumtrie(const std::vector<UInt>& values) {
        for (const UInt value : values) {
            int current = 0;
            for (int bit = Bits - 1; bit >= 0; --bit) {
                const int direction = (value >> bit) & 1;
                if (nodes[current].child[direction] == -1) {
                    nodes[current].child[direction] = static_cast<int>(nodes.size());
                    nodes.push_back({});
                }
                current = nodes[current].child[direction];
            }
        }
        build(0, Bits - 1);
    }

    unsigned long long build(int current, int bit) {
        if (bit < 0) return 0;
        const unsigned long long half = 1ULL << bit;
        const int left = nodes[current].child[0];
        const int right = nodes[current].child[1];
        if (left != -1 && right != -1) {
            nodes[current].full_sum = build(left, bit - 1) + build(right, bit - 1);
        } else {
            const int child = left != -1 ? left : right;
            nodes[current].full_sum = 2 * build(child, bit - 1) + half * half;
        }
        return nodes[current].full_sum;
    }

    unsigned long long prefix(int current, unsigned long long upper, int bit) const {
        if (upper == 0 || bit < 0) return 0;
        const unsigned long long half = 1ULL << bit;
        if (upper == 2 * half) return nodes[current].full_sum;
        const int left = nodes[current].child[0];
        const int right = nodes[current].child[1];
        if (upper <= half) {
            if (left != -1) return prefix(left, upper, bit - 1);
            return prefix(right, upper, bit - 1) + half * upper;
        }
        unsigned long long answer = left != -1
            ? nodes[left].full_sum
            : nodes[right].full_sum + half * half;
        upper -= half;
        if (right != -1) answer += prefix(right, upper, bit - 1);
        else answer += prefix(left, upper, bit - 1) + half * upper;
        return answer;
    }
};

}

/// O(n bits)前計算・O(bits)集約。x=0..upper-1についてmin_a(x xor a)の総和を返す。
template<std::unsigned_integral UInt, int Bits = std::numeric_limits<UInt>::digits>
unsigned long long sum_min_xor(const std::vector<UInt>& values, unsigned long long upper) {
    static_assert(1 <= Bits && Bits <= 32 && Bits <= std::numeric_limits<UInt>::digits);
    assert(!values.empty() && upper <= (1ULL << Bits));
    for (const UInt value : values) assert(static_cast<unsigned long long>(value) < (1ULL << Bits));
    const detail::minxorsumtrie<UInt, Bits> trie(values);
    return trie.prefix(0, upper, Bits - 1);
}

/// O(n bits)前計算・O(bits)集約。x=left..right-1についてmin_a(x xor a)の総和を返す。
template<std::unsigned_integral UInt, int Bits = std::numeric_limits<UInt>::digits>
unsigned long long sum_min_xor(
    const std::vector<UInt>& values,
    unsigned long long left,
    unsigned long long right
) {
    static_assert(1 <= Bits && Bits <= 32 && Bits <= std::numeric_limits<UInt>::digits);
    assert(!values.empty() && left <= right && right <= (1ULL << Bits));
    for (const UInt value : values) assert(static_cast<unsigned long long>(value) < (1ULL << Bits));
    const detail::minxorsumtrie<UInt, Bits> trie(values);
    return trie.prefix(0, right, Bits - 1) - trie.prefix(0, left, Bits - 1);
}

/// O(n bits)。i<jかつvalues[i] xor values[j]<=limitとなる組数を返す。
template <std::unsigned_integral UInt, int Bits = std::numeric_limits<UInt>::digits>
long long count_pairs_xor_at_most(const std::vector<UInt>& values, UInt limit) {
    xortrie<UInt, Bits> trie;
    long long answer = 0;
    for (UInt value : values) {
        answer += trie.count_xor_at_most(value, limit);
        trie.add(value);
    }
    return answer;
}

}
