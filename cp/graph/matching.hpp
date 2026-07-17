#pragma once
#include <bits/stdc++.h>

namespace poe {

/// 二部最大matchingの結果: result[left], result.right[right], result.size(); 取得O(1)。
struct bipartitematchingresult {
    std::vector<int> left;
    std::vector<int> right;
    std::vector<int> left_cover;
    std::vector<int> right_cover;
    int matching_size = 0;

    /// O(1)。matchingの辺数を返す。
    int size() const noexcept {
        return matching_size;
    }

    /// O(1)。二部グラフの最大独立集合の頂点数を返す。
    int independent_size() const noexcept {
        return static_cast<int>(left.size() + right.size()) - matching_size;
    }

    /// O(L)。最大独立集合に含まれる左頂点を返す。
    std::vector<int> left_independent() const {
        std::vector<bool> covered(left.size());
        for (int vertex : left_cover) covered[vertex] = true;
        std::vector<int> result;
        for (int vertex = 0; vertex < static_cast<int>(left.size()); ++vertex) {
            if (!covered[vertex]) result.push_back(vertex);
        }
        return result;
    }

    /// O(R)。最大独立集合に含まれる右頂点を返す。
    std::vector<int> right_independent() const {
        std::vector<bool> covered(right.size());
        for (int vertex : right_cover) covered[vertex] = true;
        std::vector<int> result;
        for (int vertex = 0; vertex < static_cast<int>(right.size()); ++vertex) {
            if (!covered[vertex]) result.push_back(vertex);
        }
        return result;
    }

    /// O(1)。左頂点vertexと対応する右頂点。未matchingなら-1。
    int operator[](int vertex) const {
        assert(0 <= vertex && vertex < static_cast<int>(left.size()));
        return left[vertex];
    }

    /// O(L)。matchingに使う(left, right)の組を返す。
    std::vector<std::pair<int, int>> edges() const {
        std::vector<std::pair<int, int>> result;
        result.reserve(size());
        for (int vertex = 0; vertex < static_cast<int>(left.size()); ++vertex) {
            if (left[vertex] != -1) result.emplace_back(vertex, left[vertex]);
        }
        return result;
    }
};

/// 左右の頂点集合を分けた二部グラフ: bipartitematching graph(L, R); solveはO(m sqrt(L+R))。
struct bipartitematching {
    /// O(L+R)。辺のない二部グラフを作る。
    bipartitematching(int left_size, int right_size)
        : left_size_(left_size), right_size_(right_size), adjacency_(left_size) {
        assert(left_size >= 0 && right_size >= 0);
    }

    /// ならしO(1)。左頂点leftと右頂点rightを結ぶ辺を追加し、辺番号を返す。
    int add_edge(int left, int right) {
        assert(0 <= left && left < left_size_);
        assert(0 <= right && right < right_size_);
        const int id = static_cast<int>(edges_.size());
        edges_.emplace_back(left, right);
        adjacency_[left].push_back(right);
        return id;
    }

    /// O(1)。左頂点数を返す。
    int left_size() const noexcept {
        return left_size_;
    }

    /// O(1)。右頂点数を返す。
    int right_size() const noexcept {
        return right_size_;
    }

    /// O(1)。追加した辺数を返す。
    int edge_count() const noexcept {
        return static_cast<int>(edges_.size());
    }

    /// O(1)。左頂点から出る右頂点一覧を返す。
    const std::vector<int>& operator[](int left) const {
        assert(0 <= left && left < left_size_);
        return adjacency_[left];
    }

    /// O(m sqrt(L+R))。最大matchingと同じ大きさの最小頂点coverを返す。
    bipartitematchingresult solve() const {
        bipartitematchingresult result;
        result.left.assign(left_size_, -1);
        result.right.assign(right_size_, -1);
        std::vector<int> distance(left_size_);

        auto bfs = [&] {
            std::queue<int> queue;
            std::fill(distance.begin(), distance.end(), -1);
            for (int left = 0; left < left_size_; ++left) {
                if (result.left[left] == -1) {
                    distance[left] = 0;
                    queue.push(left);
                }
            }
            bool reachable_free = false;
            while (!queue.empty()) {
                const int left = queue.front();
                queue.pop();
                for (int right : adjacency_[left]) {
                    const int next = result.right[right];
                    if (next == -1) {
                        reachable_free = true;
                    } else if (distance[next] == -1) {
                        distance[next] = distance[left] + 1;
                        queue.push(next);
                    }
                }
            }
            return reachable_free;
        };

        auto dfs = [&](auto&& self, int left) -> bool {
            for (int right : adjacency_[left]) {
                const int next = result.right[right];
                if (next == -1 ||
                    (distance[next] == distance[left] + 1 && self(self, next))) {
                    result.left[left] = right;
                    result.right[right] = left;
                    return true;
                }
            }
            distance[left] = -1;
            return false;
        };

        while (bfs()) {
            for (int left = 0; left < left_size_; ++left) {
                if (result.left[left] == -1 && dfs(dfs, left)) {
                    ++result.matching_size;
                }
            }
        }

        std::vector<bool> seen_left(left_size_), seen_right(right_size_);
        std::queue<int> queue;
        for (int left = 0; left < left_size_; ++left) {
            if (result.left[left] == -1) {
                seen_left[left] = true;
                queue.push(left);
            }
        }
        while (!queue.empty()) {
            const int left = queue.front();
            queue.pop();
            for (int right : adjacency_[left]) {
                if (result.left[left] == right || seen_right[right]) continue;
                seen_right[right] = true;
                const int next = result.right[right];
                if (next != -1 && !seen_left[next]) {
                    seen_left[next] = true;
                    queue.push(next);
                }
            }
        }
        for (int left = 0; left < left_size_; ++left) {
            if (!seen_left[left]) result.left_cover.push_back(left);
        }
        for (int right = 0; right < right_size_; ++right) {
            if (seen_right[right]) result.right_cover.push_back(right);
        }
        return result;
    }

private:
    int left_size_;
    int right_size_;
    std::vector<std::pair<int, int>> edges_;
    std::vector<std::vector<int>> adjacency_;
};

/// O(LR+R log R)。完全二部グラフで右頂点の要求次数を左頂点の次数上限内に割り当てられるか返す。
inline bool bipartite_degree_feasible(
    const std::vector<int>& left_capacity,
    std::vector<int> right_degree
) {
    for (const int capacity : left_capacity) assert(capacity >= 0);
    for (const int degree : right_degree) assert(degree >= 0);
    std::ranges::sort(right_degree, std::greater{});
    long long demand = 0;
    for (int count = 1; count <= static_cast<int>(right_degree.size()); ++count) {
        demand += right_degree[count - 1];
        long long supply = 0;
        for (const int capacity : left_capacity) supply += std::min(capacity, count);
        if (demand > supply) return false;
    }
    return true;
}

/// O(LR^3)時間・O(LR^2)領域。要求次数を割当て可能な右頂点部分集合の最大報酬を返す。
template<class T>
T max_bipartite_degree_reward(
    const std::vector<int>& left_capacity,
    const std::vector<int>& right_degree,
    const std::vector<T>& reward
) {
    static_assert(std::is_arithmetic_v<T>);
    assert(right_degree.size() == reward.size());
    for (const int capacity : left_capacity) assert(capacity >= 0);
    for (const int degree : right_degree) assert(degree >= 0);
    const int right_size = static_cast<int>(right_degree.size());

    std::vector<std::pair<int, T>> items(right_size);
    for (int right = 0; right < right_size; ++right) {
        items[right] = {right_degree[right], reward[right]};
    }
    std::ranges::sort(items, std::greater{}, &std::pair<int, T>::first);

    std::vector<int> capacity(right_size + 1);
    for (int count = 1; count <= right_size; ++count) {
        long long sum = 0;
        for (const int limit : left_capacity) sum += std::min(limit, count);
        assert(sum <= std::numeric_limits<int>::max());
        capacity[count] = static_cast<int>(sum);
    }
    const int max_sum = capacity[right_size];
    const T unreachable = std::numeric_limits<T>::lowest();
    std::vector<std::vector<T>> dp(
        static_cast<std::size_t>(right_size) + 1,
        std::vector<T>(static_cast<std::size_t>(max_sum) + 1, unreachable)
    );
    dp[0][0] = T{};
    T answer{};
    for (int item = 0; item < right_size; ++item) {
        const auto [degree, value] = items[item];
        for (int count = item + 1; count >= 1; --count) {
            for (int sum = capacity[count]; sum >= degree; --sum) {
                const T previous = dp[count - 1][sum - degree];
                if (previous == unreachable) continue;
                dp[count][sum] = std::max(dp[count][sum], previous + value);
                answer = std::max(answer, dp[count][sum]);
            }
        }
    }
    return answer;
}

}
