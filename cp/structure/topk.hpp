#pragma once
#include <bits/stdc++.h>

namespace poe {

/// キーが相異なる値上位K件を保持する固定長monoid。merge O(K^2)。
template <class Value, class Key, std::size_t K>
struct topkdistinct {
    using entry = std::pair<Value, Key>;
    std::array<std::optional<entry>, K> data{};

    /// O(K^2)。同じkeyは大きいvalueだけ残して候補を追加する。
    void add(Value value, Key key) {
        std::vector<entry> candidates;
        candidates.reserve(K + 1);
        for (const auto& current : data)
            if (current) candidates.push_back(*current);
        candidates.push_back({value, key});
        assign(candidates);
    }

    /// O(1)。有効な候補数を返す。
    int size() const noexcept {
        return static_cast<int>(std::ranges::count_if(data, [](const auto& x) {
            return x.has_value();
        }));
    }

    /// O(1)。value降順のindex番目を返す。
    const entry& operator[](int index) const {
        assert(0 <= index && index < size());
        return *data[index];
    }

private:
    void assign(std::vector<entry>& candidates) {
        std::ranges::sort(candidates, std::greater<entry>());
        data.fill(std::nullopt);
        std::size_t count = 0;
        for (const auto& candidate : candidates) {
            bool used = false;
            for (std::size_t i = 0; i < count; ++i)
                if (data[i]->second == candidate.second) used = true;
            if (!used) data[count++] = candidate;
            if (count == K) break;
        }
    }

    template <class V, class Q, std::size_t L>
    friend topkdistinct<V, Q, L> merge_topkdistinct(topkdistinct<V, Q, L>,
                                                    const topkdistinct<V, Q, L>&);
};

/// O(K^2)。二集合からキーが相異なる値上位K件を返す。
template <class Value, class Key, std::size_t K>
topkdistinct<Value, Key, K> merge_topkdistinct(
    topkdistinct<Value, Key, K> first,
    const topkdistinct<Value, Key, K>& second
) {
    std::vector<typename topkdistinct<Value, Key, K>::entry> candidates;
    candidates.reserve(2 * K);
    for (const auto& current : first.data)
        if (current) candidates.push_back(*current);
    for (const auto& current : second.data)
        if (current) candidates.push_back(*current);
    first.assign(candidates);
    return first;
}

/// O(1)。topkdistinct monoidの単位元を返す。
template <class Value, class Key, std::size_t K>
topkdistinct<Value, Key, K> empty_topkdistinct() {
    return {};
}

/// O(K^2)。候補一件からtopkdistinctを作る。
template <class Value, class Key, std::size_t K>
topkdistinct<Value, Key, K> make_topkdistinct(Value value, Key key) {
    topkdistinct<Value, Key, K> result;
    result.add(value, key);
    return result;
}

}
