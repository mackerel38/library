#pragma once
#include <bits/stdc++.h>

namespace poe {

/// O(n sqrt(n) log n)時間・O(n sqrt(n))領域。位置順・辞書順とも真に増加する非重複substring列の最大長を返す。
inline int maximum_increasing_disjoint_substrings(std::string_view text) {
    const int size = static_cast<int>(text.size());
    if (size == 0) return 0;
    int length_limit = 0;
    while (
        static_cast<long long>(length_limit + 1) * (length_limit + 2) / 2 <= size
    ) {
        ++length_limit;
    }

    struct prefixmaximum {
        std::vector<int> data;

        explicit prefixmaximum(int size) : data(size + 1) {}

        int get(int right) const {
            int result = 0;
            for (int index = right; index > 0; index -= index & -index) {
                result = std::max(result, data[index]);
            }
            return result;
        }

        void set(int index, int value) {
            for (++index; index < static_cast<int>(data.size()); index += index & -index) {
                if (data[index] >= value) break;
                data[index] = std::max(data[index], value);
            }
        }
    } best_ending_at(size);

    std::array<int, 256> character_rank;
    character_rank.fill(-1);
    std::vector<unsigned char> alphabet;
    for (const char raw_character : text) {
        const auto character = static_cast<unsigned char>(raw_character);
        if (character_rank[character] == -1) {
            character_rank[character] = 0;
            alphabet.push_back(character);
        }
    }
    std::sort(alphabet.begin(), alphabet.end());
    for (int rank = 0; rank < static_cast<int>(alphabet.size()); ++rank) {
        character_rank[alphabet[rank]] = rank;
    }

    int answer = 0;
    auto traverse = [&](auto&& self, std::vector<int> ends, int length) -> void {
        if (ends.size() == 1) {
            int end = ends[0];
            while (true) {
                const int left = end - length + 1;
                const int value = best_ending_at.get(left) + 1;
                best_ending_at.set(end, value);
                answer = std::max(answer, value);
                if (length == length_limit || end + 1 == size) break;
                ++end;
                ++length;
            }
            return;
        }
        std::vector<int> values;
        values.reserve(ends.size());
        for (const int end : ends) {
            const int left = end - length + 1;
            values.push_back(best_ending_at.get(left) + 1);
            answer = std::max(answer, values.back());
        }
        for (int index = 0; index < static_cast<int>(ends.size()); ++index) {
            best_ending_at.set(ends[index], values[index]);
        }
        if (length == length_limit) return;

        std::vector<std::vector<int>> children(alphabet.size());
        for (const int end : ends) {
            if (end + 1 == size) continue;
            const auto character = static_cast<unsigned char>(text[end + 1]);
            children[character_rank[character]].push_back(end + 1);
        }
        for (auto& child : children) {
            if (!child.empty()) self(self, std::move(child), length + 1);
        }
    };

    std::vector<std::vector<int>> roots(alphabet.size());
    for (int position = 0; position < size; ++position) {
        const auto character = static_cast<unsigned char>(text[position]);
        roots[character_rank[character]].push_back(position);
    }
    for (auto& root : roots) {
        if (!root.empty()) traverse(traverse, std::move(root), 1);
    }
    return answer;
}

}
