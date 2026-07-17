#pragma once
#include <bits/stdc++.h>

namespace poe {

/// 各頂点の行先が一つのグラフ: functionalgraph fg(next); 構築O(n log U)、jump O(log U)。
struct functionalgraph {
    /// O(n log U)。next[v]を辺v->next[v]として、閉路とdoublingを前計算する。U=2^64。
    explicit functionalgraph(const std::vector<int>& next) : next_(next) {
        const int n = size();
        std::vector<std::vector<int>> reverse(n);
        std::vector<int> indegree(n);
        for (int vertex = 0; vertex < n; ++vertex) {
            assert(0 <= next_[vertex] && next_[vertex] < n);
            reverse[next_[vertex]].push_back(vertex);
            ++indegree[next_[vertex]];
        }

        std::queue<int> queue;
        std::vector<bool> in_cycle(n, true);
        for (int vertex = 0; vertex < n; ++vertex) {
            if (indegree[vertex] == 0) queue.push(vertex);
        }
        while (!queue.empty()) {
            const int vertex = queue.front();
            queue.pop();
            in_cycle[vertex] = false;
            if (--indegree[next_[vertex]] == 0) queue.push(next_[vertex]);
        }

        cycle_id_.assign(n, -1);
        cycle_position_.assign(n, -1);
        entry_.assign(n, -1);
        distance_.assign(n, -1);
        for (int start = 0; start < n; ++start) {
            if (!in_cycle[start] || cycle_id_[start] != -1) continue;
            const int id = static_cast<int>(cycles_.size());
            cycles_.push_back({});
            int vertex = start;
            do {
                cycle_id_[vertex] = id;
                cycle_position_[vertex] = static_cast<int>(cycles_.back().size());
                entry_[vertex] = vertex;
                distance_[vertex] = 0;
                cycles_.back().push_back(vertex);
                vertex = next_[vertex];
            } while (vertex != start);
        }

        for (const auto& cycle : cycles_) {
            for (int root : cycle) queue.push(root);
        }
        while (!queue.empty()) {
            const int vertex = queue.front();
            queue.pop();
            for (int previous : reverse[vertex]) {
                if (distance_[previous] != -1) continue;
                cycle_id_[previous] = cycle_id_[vertex];
                entry_[previous] = entry_[vertex];
                distance_[previous] = distance_[vertex] + 1;
                queue.push(previous);
            }
        }

        jump_.assign(64, std::vector<int>(n));
        if (n > 0) {
            jump_[0] = next_;
            for (int bit = 1; bit < 64; ++bit) {
                for (int vertex = 0; vertex < n; ++vertex) {
                    jump_[bit][vertex] = jump_[bit - 1][jump_[bit - 1][vertex]];
                }
            }
        }
    }

    /// O(1)。頂点数を返す。
    int size() const noexcept {
        return static_cast<int>(next_.size());
    }

    /// O(1)。fg[vertex]でvertexの次の頂点を返す。
    int operator[](int vertex) const {
        assert_vertex(vertex);
        return next_[vertex];
    }

    /// O(log steps)。vertexからsteps回進んだ頂点を返す。
    int jump(int vertex, unsigned long long steps) const {
        assert_vertex(vertex);
        for (int bit = 0; bit < 64; ++bit) {
            if ((steps >> bit) & 1ULL) vertex = jump_[bit][vertex];
        }
        return vertex;
    }

    /// O(1)。vertexが最終的に入る閉路番号を返す。
    int cycle_id(int vertex) const {
        assert_vertex(vertex);
        return cycle_id_[vertex];
    }

    /// O(1)。vertexから閉路へ入るまでの辺数を返す。閉路上なら0。
    int distance_to_cycle(int vertex) const {
        assert_vertex(vertex);
        return distance_[vertex];
    }

    /// O(1)。vertexから最初に到達する閉路上の頂点を返す。
    int entry(int vertex) const {
        assert_vertex(vertex);
        return entry_[vertex];
    }

    /// O(1)。vertexが属する行先閉路の長さを返す。
    int cycle_size(int vertex) const {
        assert_vertex(vertex);
        return static_cast<int>(cycles_[cycle_id_[vertex]].size());
    }

    /// O(1)。閉路番号idの頂点列を辺の向き順で返す。
    const std::vector<int>& cycle(int id) const {
        assert(0 <= id && id < static_cast<int>(cycles_.size()));
        return cycles_[id];
    }

    /// O(1)。閉路数を返す。
    int cycle_count() const noexcept {
        return static_cast<int>(cycles_.size());
    }

    /// O(log n)。fromからtoへ到達する最小step数。到達不能ならnullopt。
    std::optional<unsigned long long> steps(int from, int to) const {
        assert_vertex(from);
        assert_vertex(to);
        if (cycle_id_[from] != cycle_id_[to]) {
            return std::nullopt;
        }
        if (distance_[to] > 0) {
            if (distance_[from] < distance_[to]) return std::nullopt;
            const unsigned long long difference =
                static_cast<unsigned long long>(distance_[from] - distance_[to]);
            return jump(from, difference) == to
                       ? std::optional<unsigned long long>(difference)
                       : std::nullopt;
        }
        const int start = entry_[from];
        const int length = cycle_size(from);
        const int around =
            (cycle_position_[to] - cycle_position_[start] + length) % length;
        return static_cast<unsigned long long>(distance_[from]) + around;
    }

private:
    /// O(1)。頂点番号を確認する。
    void assert_vertex(int vertex) const {
        assert(0 <= vertex && vertex < size());
    }

    std::vector<int> next_;
    std::vector<int> cycle_id_;
    std::vector<int> cycle_position_;
    std::vector<int> entry_;
    std::vector<int> distance_;
    std::vector<std::vector<int>> cycles_;
    std::vector<std::vector<int>> jump_;
};

}
