#pragma once
#include <bits/stdc++.h>

namespace poe {

/// 全valueへのaffine変換を遅延できるmap: affinemap<int,mint> values; apply(a,b)でxをa*x+bへ変える。
template <class Key, class Value>
struct affinemap {
    /// O(1)。空のmapを作る。
    affinemap() = default;

    /// O(log n)。keyの論理valueを代入する。現在のscaleが0でないことが必要。
    void set(const Key& key, const Value& value) {
        assert(scale_ != Value{});
        const Value raw = (value - shift_) / scale_;
        auto [iterator, inserted] = values_.try_emplace(key, raw);
        if (!inserted) raw_sum_ -= iterator->second;
        iterator->second = raw;
        raw_sum_ += raw;
    }

    /// O(log n)。keyを削除し、存在したか返す。
    bool erase(const Key& key) {
        auto iterator = values_.find(key);
        if (iterator == values_.end()) return false;
        raw_sum_ -= iterator->second;
        values_.erase(iterator);
        return true;
    }

    /// O(log n)。keyが存在するか返す。
    bool contains(const Key& key) const {
        return values_.contains(key);
    }

    /// O(log n)。keyの論理valueを返す。keyが存在することが必要。
    Value get(const Key& key) const {
        return scale_ * values_.at(key) + shift_;
    }

    /// O(1)。全valueをx -> scale*x+shiftへ一括変換する。
    void apply(const Value& scale, const Value& shift) {
        scale_ *= scale;
        shift_ = scale * shift_ + shift;
    }

    /// O(1)。全valueの総和を返す。
    Value sum() const {
        return scale_ * raw_sum_ + shift_ * static_cast<long long>(values_.size());
    }

    /// O(1)。key数を返す。
    int size() const noexcept {
        return static_cast<int>(values_.size());
    }

    /// O(1)。空ならtrueを返す。
    bool empty() const noexcept {
        return values_.empty();
    }

private:
    std::map<Key, Value> values_;
    Value raw_sum_{};
    Value scale_ = Value{1};
    Value shift_{};
};

}
