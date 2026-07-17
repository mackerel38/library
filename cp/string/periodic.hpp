#pragma once
#include <bits/stdc++.h>
#include "string/prefix.hpp"

namespace poe {

/// O(n)。primitive_period(text): textを構成する最短周期文字列を返す。空文字列は空。
inline std::string primitive_period(const std::string& text) {
    if (text.empty()) return {};
    const auto prefix = prefix_function(text);
    const int candidate = static_cast<int>(text.size()) - prefix.back();
    const int length = text.size() % candidate == 0
        ? candidate : static_cast<int>(text.size());
    return text.substr(0, length);
}

/// O(L alpha(n))目安。順に各文字列を正整数回反復し、過去と異なる最小反復回数を返す。
inline std::vector<long long> minimum_unique_repetition_counts(
    const std::vector<std::string>& strings
) {
    struct successorset {
        std::unordered_map<long long, long long> parent;

        long long find(long long value) {
            std::vector<long long> path;
            auto iterator = parent.find(value);
            while (iterator != parent.end()) {
                path.push_back(value);
                value = iterator->second;
                iterator = parent.find(value);
            }
            for (const long long current : path) parent[current] = value;
            return value;
        }

        void insert(long long value) {
            assert(!parent.contains(value));
            parent[value] = find(value + 1);
        }
    };

    struct groupstate {
        successorset used;
        std::unordered_map<long long, long long> next_multiple;
    };

    std::unordered_map<std::string, groupstate> groups;
    std::vector<long long> result;
    result.reserve(strings.size());
    for (const std::string& text : strings) {
        assert(!text.empty());
        const std::string period = primitive_period(text);
        const long long base_repetitions = text.size() / period.size();
        auto& group = groups[period];
        long long total_repetitions =
            group.next_multiple.try_emplace(base_repetitions, base_repetitions).first->second;
        while (true) {
            const long long unused = group.used.find(total_repetitions);
            const long long rounded =
                (unused + base_repetitions - 1) / base_repetitions * base_repetitions;
            if (rounded == unused) {
                group.used.insert(rounded);
                group.next_multiple[base_repetitions] = rounded + base_repetitions;
                result.push_back(rounded / base_repetitions);
                break;
            }
            total_repetitions = rounded;
        }
    }
    return result;
}

}
