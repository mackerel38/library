#pragma once
#include <bits/stdc++.h>

namespace poe {

/// 行submask profile DPの結果。costが削除bit数、masksが選んだ各行mask。
struct submaskprofileresult {
    int cost;
    std::vector<std::uint64_t> masks;
};

/// O(h 4^w)時間・O(h 2^w)空間。各行のsubmaskを隣接compatibleに選び削除bit数を最小化する。
template <class Compatible>
submaskprofileresult minimum_submask_profile(
    const std::vector<std::uint64_t>& available,
    int width,
    Compatible compatible
) {
    assert(0 <= width && width < 63);
    const std::uint64_t count = std::uint64_t{1} << width;
    for (const auto mask : available) assert(mask < count);
    if (available.empty()) return {0, {}};

    constexpr int infinity = std::numeric_limits<int>::max() / 4;
    std::vector<int> previous(count, infinity);
    std::vector<std::vector<std::uint64_t>> parent(
        available.size(), std::vector<std::uint64_t>(count, count));
    for (std::uint64_t mask = available[0];; mask = (mask - 1) & available[0]) {
        previous[mask] = std::popcount(available[0] ^ mask);
        if (mask == 0) break;
    }

    for (int row = 1; row < static_cast<int>(available.size()); ++row) {
        std::vector<int> next(count, infinity);
        for (std::uint64_t mask = available[row];; mask = (mask - 1) & available[row]) {
            const int remove = std::popcount(available[row] ^ mask);
            for (std::uint64_t before = 0; before < count; ++before) {
                if (previous[before] == infinity
                    || !compatible(before, mask)) continue;
                const int candidate = previous[before] + remove;
                if (candidate < next[mask]) {
                    next[mask] = candidate;
                    parent[row][mask] = before;
                }
            }
            if (mask == 0) break;
        }
        previous.swap(next);
    }

    const auto iterator = std::ranges::min_element(previous);
    const int cost = *iterator;
    std::uint64_t mask = iterator - previous.begin();
    std::vector<std::uint64_t> masks(available.size());
    for (int row = static_cast<int>(available.size()) - 1; row >= 0; --row) {
        masks[row] = mask;
        if (row > 0) {
            mask = parent[row][mask];
            assert(mask < count);
        }
    }
    return {cost, std::move(masks)};
}

/// O(width)。normalize_profile(labels): frontier DPの連結成分番号を出現順に0,1,...へ正規化する。
/// -1は空マス。distinguished>=0ならその位置の成分を必ず0にする。
/// reserve_zero=trueならdistinguishedの成分が消えた後も0を他成分へ再利用しない。
inline std::vector<int> normalize_profile(
    std::vector<int> labels,
    int distinguished = -1,
    bool reserve_zero = false
) {
    assert(distinguished == -1 || (0 <= distinguished && distinguished < static_cast<int>(labels.size())));
    std::map<int, int> converted;
    int next = reserve_zero ? 1 : 0;
    if (distinguished != -1 && labels[distinguished] != -1) {
        converted[labels[distinguished]] = 0;
        if (!reserve_zero) next = 1;
    }
    for (int& label : labels) {
        if (label == -1) continue;
        if (!converted.contains(label)) converted[label] = next++;
        label = converted[label];
    }
    return labels;
}

/// connectivityprofile state(width): frontier上の連結成分を持つ。-1は空、0は注目成分。
struct connectivityprofile {
    std::vector<int> labels;

    /// O(width)。connectivityprofile(width): 幅widthの空状態を作る。
    explicit connectivityprofile(int width = 0) : labels(width, -1) {}

    /// O(width)。connectivityprofile(labels): 0を注目成分として正規化して持つ。
    explicit connectivityprofile(std::vector<int> labels_) : labels(std::move(labels_)) {
        normalize();
    }

    /// O(width^2)。from_mask(width, mask, marked): 1のマスを上下に連結し、markedの成分を0にする。
    static connectivityprofile from_mask(int width, std::uint64_t mask, int marked = -1) {
        assert(0 <= width && width < 63);
        assert(marked == -1 || (0 <= marked && marked < width));
        connectivityprofile result(width);
        for (int position = 0; position < width; ++position) {
            if (mask >> position & 1ULL) result.labels[position] = position + 1;
        }
        for (int position = 1; position < width; ++position) {
            result.connect_raw(position - 1, position);
        }
        result.normalize();
        if (marked != -1 && result.occupied(marked)) result.mark(marked);
        return result;
    }

    /// O(width^2)。state.advance(mask): 次の列をmaskとして、横辺と縦辺をすべて張った状態を返す。
    connectivityprofile advance(std::uint64_t mask) const {
        assert(size() < 63);
        connectivityprofile result(size());
        for (int position = 0; position < size(); ++position) {
            if (!(mask >> position & 1ULL)) continue;
            result.labels[position] = occupied(position) ? labels[position] : size() + position + 1;
        }
        for (int position = 1; position < size(); ++position) {
            result.connect_raw(position - 1, position);
        }
        result.normalize();
        return result;
    }

    /// O(1)。state.size(): frontierの幅を返す。
    int size() const noexcept { return static_cast<int>(labels.size()); }

    /// O(1)。state.occupied(position): positionが空でなければtrueを返す。
    bool occupied(int position) const {
        assert(0 <= position && position < size());
        return labels[position] != -1;
    }

    /// O(1)。state[position]: positionの成分番号を返す。空なら-1、注目成分なら0。
    int operator[](int position) const {
        assert(0 <= position && position < size());
        return labels[position];
    }

    /// O(width)。state.add(position): 空のpositionへ新しい独立成分を置く。
    void add(int position) {
        assert(0 <= position && position < size());
        assert(!occupied(position));
        labels[position] = size() + 1;
        normalize();
    }

    /// O(width)。state.erase(position): positionを空にして状態を正規化する。
    void erase(int position) {
        assert(0 <= position && position < size());
        labels[position] = -1;
        normalize();
    }

    /// O(width)。state.connect(a, b): 占有済みのaとbの成分を併合する。
    void connect(int a, int b) {
        assert(occupied(a) && occupied(b));
        connect_raw(a, b);
        normalize();
    }

    /// O(width)。state.mark(position): positionを含む成分を注目成分0にする。
    void mark(int position) {
        assert(occupied(position));
        assert(!has_marked() || labels[position] == 0);
        const int previous = labels[position];
        for (int& label : labels) {
            if (label == previous) label = 0;
        }
        normalize();
    }

    /// O(1)。state.is_marked(position): positionが注目成分ならtrueを返す。
    bool is_marked(int position) const {
        assert(0 <= position && position < size());
        return labels[position] == 0;
    }

    /// O(width)。state.has_marked(): 注目成分がfrontier上に残っていればtrueを返す。
    bool has_marked() const {
        return std::ranges::find(labels, 0) != labels.end();
    }

    /// O(width log width)。state.normalize(): 同じ連結状態を同じ成分番号へ正規化する。
    void normalize() {
        int marked = -1;
        for (int position = 0; position < size(); ++position) {
            if (labels[position] == 0) {
                marked = position;
                break;
            }
        }
        labels = normalize_profile(std::move(labels), marked, true);
    }

    friend bool operator==(const connectivityprofile&, const connectivityprofile&) = default;
    friend auto operator<=>(const connectivityprofile&, const connectivityprofile&) = default;

private:
    void connect_raw(int a, int b) {
        if (!occupied(a) || !occupied(b) || labels[a] == labels[b]) return;
        const int first = labels[a];
        const int second = labels[b];
        const int keep = first == 0 || second == 0 ? 0 : first;
        const int remove = keep == first ? second : first;
        for (int& label : labels) {
            if (label == remove) label = keep;
        }
    }
};

}
