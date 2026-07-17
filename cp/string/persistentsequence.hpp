#pragma once
#include <bits/stdc++.h>

namespace poe {

/// 既存列への一要素追加で作る列群を辞書順化する: persistentsequenceorder<int> sequences; O(log sigma)/追加。
template <class Symbol>
struct persistentsequenceorder {
    /// O(1)。空列version 0だけを持つ。
    persistentsequenceorder() : nodes_(1), version_node_(1, 0) {}

    /// O(log sigma)。versionの末尾にsymbolを加えた新version番号を返す。
    int add(int version, const Symbol& symbol) {
        assert(0 <= version && version < size());
        const int parent = version_node_[version];
        auto [iterator, inserted] = nodes_[parent].next.try_emplace(symbol, nodes_.size());
        if (inserted) nodes_.push_back({});
        const int node = iterator->second;
        const int id = size();
        version_node_.push_back(node);
        nodes_[node].versions.push_back(id);
        return id;
    }

    /// O(total versions + trie nodes)。空列を除くversion番号を列の辞書順・同値なら番号順で返す。
    std::vector<int> order() const {
        std::vector<int> result;
        result.reserve(size() - 1);
        std::vector<std::pair<int, bool>> stack{{0, false}};
        while (!stack.empty()) {
            const auto [node, visited] = stack.back();
            stack.pop_back();
            if (!visited) {
                for (int version : nodes_[node].versions) result.push_back(version);
                for (auto iterator = nodes_[node].next.rbegin(); iterator != nodes_[node].next.rend(); ++iterator) {
                    stack.emplace_back(iterator->second, false);
                }
            }
        }
        return result;
    }

    /// O(1)。空列を含むversion数を返す。
    int size() const noexcept { return static_cast<int>(version_node_.size()); }

    /// O(1)。versionに対応する共有trie頂点を返す。
    int node(int version) const {
        assert(0 <= version && version < size());
        return version_node_[version];
    }

private:
    struct node_type {
        std::map<Symbol, int> next;
        std::vector<int> versions;
    };
    std::vector<node_type> nodes_;
    std::vector<int> version_node_;
};

/// 共有列の連結DAG: persistentconcatenation<char> seq({'0','1'}, limit); add後も旧versionを保持する。
template<class Symbol>
struct persistentconcatenation {
    /// O(initial)。各要素を長さ1の初期versionとして作る。参照する最大長limitを指定する。
    explicit persistentconcatenation(
        const std::vector<Symbol>& initial,
        unsigned long long limit = std::numeric_limits<unsigned long long>::max()
    ) : limit_(limit) {
        assert(!initial.empty() && limit >= 1);
        nodes_.reserve(initial.size());
        for (const auto& symbol : initial) nodes_.push_back(node_type{1, -1, 0, symbol});
    }

    /// O(log V)。leftとrightを連結し先頭limit要素だけを表す新version番号を返す。
    int add(int left, int right) {
        assert_version(left);
        assert_version(right);
        const auto left_length = nodes_[left].length;
        const auto right_length = nodes_[right].length;
        const auto length = saturated_add(left_length, right_length);
        const auto included_left = left_length;
        const auto included_right = length - included_left;
        const int heavy = included_left >= included_right ? left : right;
        const int light = included_left >= included_right ? right : left;
        const auto heavy_offset = included_left >= included_right ? 0ULL : included_left;
        const auto light_offset = included_left >= included_right ? included_left : 0ULL;

        const int version = size();
        nodes_.push_back(node_type{length, light, light_offset, std::nullopt});
        const int levels = std::bit_width(static_cast<unsigned>(size()));
        while (static_cast<int>(jump_node_.size()) < levels) {
            jump_node_.push_back(std::vector<int>(size(), -1));
            jump_offset_.push_back(std::vector<unsigned long long>(size()));
        }
        for (int level = 0; level < levels; ++level) {
            jump_node_[level].resize(size(), -1);
            jump_offset_[level].resize(size());
        }
        jump_node_[0][version] = heavy;
        jump_offset_[0][version] = heavy_offset;
        for (int level = 1; level < levels; ++level) {
            const int middle = jump_node_[level - 1][version];
            if (middle == -1 || jump_node_[level - 1][middle] == -1) break;
            jump_node_[level][version] = jump_node_[level - 1][middle];
            jump_offset_[level][version] = saturated_add(
                jump_offset_[level - 1][version], jump_offset_[level - 1][middle]
            );
        }
        return version;
    }

    /// O(log V log limit)。versionの0-indexed位置indexにある要素を返す。
    const Symbol& get(int version, unsigned long long index) const {
        assert_version(version);
        assert(index < nodes_[version].length);
        int current = version;
        while (!nodes_[current].symbol) {
            for (int level = static_cast<int>(jump_node_.size()) - 1; level >= 0; --level) {
                const int next = jump_node_[level][current];
                if (next == -1) continue;
                const auto offset = jump_offset_[level][current];
                if (offset <= index && index - offset < nodes_[next].length) {
                    index -= offset;
                    current = next;
                }
            }
            if (nodes_[current].symbol) break;
            const auto offset = nodes_[current].light_offset;
            assert(offset <= index && index - offset < nodes_[nodes_[current].light].length);
            index -= offset;
            current = nodes_[current].light;
        }
        return *nodes_[current].symbol;
    }

    /// O(1)。versionの上限切り詰め後の長さを返す。
    unsigned long long length(int version) const {
        assert_version(version);
        return nodes_[version].length;
    }

    /// O(1)。現在のversion数を返す。
    int size() const noexcept { return static_cast<int>(nodes_.size()); }

private:
    struct node_type {
        unsigned long long length;
        int light;
        unsigned long long light_offset;
        std::optional<Symbol> symbol;
    };

    unsigned long long saturated_add(unsigned long long left, unsigned long long right) const {
        return left >= limit_ - std::min(right, limit_) ? limit_ : left + right;
    }

    void assert_version(int version) const {
        assert(0 <= version && version < size());
    }

    unsigned long long limit_;
    std::vector<node_type> nodes_;
    std::vector<std::vector<int>> jump_node_;
    std::vector<std::vector<unsigned long long>> jump_offset_;
};

}
