#pragma once
#include <bits/stdc++.h>
#include "string/lcp.hpp"
#include "string/suffixarray.hpp"

namespace poe {

/// 一つの部分文字列の出現位置。strings[string_index][left, right)を表す。
struct substringoccurrence {
    int string_index;
    int left;
    int right;
};

/// 構築O(L+K)、追加領域O(L+K)。K個の文字列の全部分文字列を辞書順で順位検索する。
struct substringorder {
private:
    struct node {
        int depth = 0;
        int parent = -1;
        int terminal_count = 0;
        long long leaf_count = 0;
        substringoccurrence representative{-1, -1, -1};
        std::vector<int> children;
    };

    struct block {
        long long end;
        long long copies;
        int parent_depth;
        substringoccurrence representative;
    };

    std::vector<block> blocks_;
    long long size_ = 0;

public:
    /// O(L+K)。stringsの非空な全部分文字列の出現を辞書順に並べる索引を構築する。
    explicit substringorder(const std::vector<std::string>& strings) {
        const int string_count = static_cast<int>(strings.size());
        std::size_t sequence_size = strings.size();
        for (const auto& text : strings) {
            sequence_size += text.size();
        }

        std::vector<int> sequence;
        std::vector<int> string_index;
        std::vector<int> local_index;
        sequence.reserve(sequence_size);
        string_index.reserve(sequence_size);
        local_index.reserve(sequence_size);
        for (int index = 0; index < string_count; ++index) {
            for (int position = 0; position < static_cast<int>(strings[index].size());
                 ++position) {
                sequence.push_back(string_count +
                                   static_cast<unsigned char>(strings[index][position]));
                string_index.push_back(index);
                local_index.push_back(position);
            }
            sequence.push_back(index);
            string_index.push_back(-1);
            local_index.push_back(-1);
        }
        if (sequence.empty()) {
            return;
        }

        const auto suffix = poe::suffix_array(sequence, string_count + 255);
        const auto adjacent_lcp = poe::lcp_array(sequence, suffix);
        int first_real = 0;
        while (first_real < static_cast<int>(suffix.size()) &&
               string_index[suffix[first_real]] < 0) {
            ++first_real;
        }
        for (int rank = first_real; rank < static_cast<int>(suffix.size()); ++rank) {
            assert(string_index[suffix[rank]] >= 0);
        }

        std::vector<node> nodes(1);
        std::vector<int> stack{0};
        for (int rank = first_real; rank < static_cast<int>(suffix.size()); ++rank) {
            const int global_position = suffix[rank];
            const int source = string_index[global_position];
            const int left = local_index[global_position];
            const int length = static_cast<int>(strings[source].size()) - left;
            const int common = rank == first_real ? 0 : adjacent_lcp[rank - 1];

            while (nodes[stack.back()].depth > common) {
                stack.pop_back();
            }
            if (nodes[stack.back()].depth < common) {
                const int parent = stack.back();
                assert(!nodes[parent].children.empty());
                const int adopted = nodes[parent].children.back();
                nodes[parent].children.pop_back();
                const int internal = static_cast<int>(nodes.size());
                nodes.push_back(node{.depth = common, .parent = parent});
                nodes[internal].children.push_back(adopted);
                nodes[adopted].parent = internal;
                nodes[parent].children.push_back(internal);
                stack.push_back(internal);
            }

            const substringoccurrence occurrence{source, left, left + length};
            if (nodes[stack.back()].depth == length) {
                ++nodes[stack.back()].terminal_count;
                if (nodes[stack.back()].representative.string_index < 0) {
                    nodes[stack.back()].representative = occurrence;
                }
                continue;
            }
            assert(nodes[stack.back()].depth < length);
            const int leaf = static_cast<int>(nodes.size());
            nodes.push_back(node{.depth = length,
                                 .parent = stack.back(),
                                 .terminal_count = 1,
                                 .representative = occurrence});
            nodes[stack.back()].children.push_back(leaf);
            stack.push_back(leaf);
        }

        std::vector<int> traversal{0};
        for (std::size_t index = 0; index < traversal.size(); ++index) {
            for (int child : nodes[traversal[index]].children) {
                traversal.push_back(child);
            }
        }
        for (auto iterator = traversal.rbegin(); iterator != traversal.rend(); ++iterator) {
            node& current = nodes[*iterator];
            current.leaf_count = current.terminal_count;
            for (int child : current.children) {
                current.leaf_count += nodes[child].leaf_count;
                if (current.representative.string_index < 0) {
                    current.representative = nodes[child].representative;
                }
            }
        }

        std::vector<int> preorder;
        for (auto iterator = nodes[0].children.rbegin();
             iterator != nodes[0].children.rend(); ++iterator) {
            preorder.push_back(*iterator);
        }
        while (!preorder.empty()) {
            const int current = preorder.back();
            preorder.pop_back();
            const node& value = nodes[current];
            const int parent_depth = nodes[value.parent].depth;
            size_ += static_cast<long long>(value.depth - parent_depth) *
                     value.leaf_count;
            blocks_.push_back(
                block{size_, value.leaf_count, parent_depth, value.representative});
            for (auto iterator = value.children.rbegin();
                 iterator != value.children.rend(); ++iterator) {
                preorder.push_back(*iterator);
            }
        }
    }

    /// O(L)。一つの文字列textの非空な全部分文字列を辞書順に並べる索引を構築する。
    explicit substringorder(const std::string& text)
        : substringorder(std::vector<std::string>{text}) {}

    /// O(1)。重複する出現を区別した非空部分文字列の総数を返す。
    long long size() const { return size_; }

    /// O(log L)。0-indexedでindex番目の辞書順部分文字列を表す出現位置を返す。
    substringoccurrence kth(long long index) const {
        assert(0 <= index && index < size_);
        const auto iterator = std::lower_bound(
            blocks_.begin(), blocks_.end(), index + 1,
            [](const block& value, long long target) { return value.end < target; });
        const long long begin = iterator == blocks_.begin() ? 0 : (iterator - 1)->end;
        const int length = iterator->parent_depth +
                           static_cast<int>((index - begin) / iterator->copies) + 1;
        auto result = iterator->representative;
        result.right = result.left + length;
        return result;
    }

    /// O(log L)。kth(index)と同じ。
    substringoccurrence operator[](long long index) const { return kth(index); }
};

}
