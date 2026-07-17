#pragma once
#include <bits/stdc++.h>

namespace poe {

/// Traveling Salesman DPの最小費用と閉路。到達不能ならreachable=false。
template<class T>
struct travelingsalesman_result {
    T cost{};
    std::vector<int> path;
    bool reachable = false;
};

/// O(n^2 2^n)時間・O(n 2^n)領域。travelingsalesman(cost, inf, start): 全頂点を巡る最小閉路。
template<class T>
travelingsalesman_result<T> travelingsalesman(
    const std::vector<std::vector<T>>& cost,
    T inf,
    int start = 0
) {
    const int n = static_cast<int>(cost.size());
    for (const auto& row : cost) assert(static_cast<int>(row.size()) == n);
    if (n == 0) return {T{}, {}, true};
    assert(0 <= start && start < n && n < 63);
    if (n == 1) return {T{}, {start, start}, true};
    const std::uint64_t states = 1ULL << n;
    std::vector<T> dp(static_cast<std::size_t>(states) * n, inf);
    std::vector<int> parent(static_cast<std::size_t>(states) * n, -1);
    auto index = [n](std::uint64_t mask, int vertex) {
        return static_cast<std::size_t>(mask) * n + vertex;
    };
    dp[index(1ULL << start, start)] = T{};
    for (std::uint64_t mask = 0; mask < states; ++mask) {
        if (!(mask >> start & 1ULL)) continue;
        for (int last = 0; last < n; ++last) {
            if (!(mask >> last & 1ULL) || dp[index(mask, last)] == inf) continue;
            for (int next = 0; next < n; ++next) {
                if (mask >> next & 1ULL || cost[last][next] == inf) continue;
                const std::uint64_t next_mask = mask | (1ULL << next);
                const T candidate = dp[index(mask, last)] + cost[last][next];
                if (candidate < dp[index(next_mask, next)]) {
                    dp[index(next_mask, next)] = candidate;
                    parent[index(next_mask, next)] = last;
                }
            }
        }
    }
    const std::uint64_t full = states - 1;
    T answer = inf;
    int last = -1;
    for (int vertex = 0; vertex < n; ++vertex) {
        if (vertex == start || dp[index(full, vertex)] == inf || cost[vertex][start] == inf) continue;
        const T candidate = dp[index(full, vertex)] + cost[vertex][start];
        if (candidate < answer) {
            answer = candidate;
            last = vertex;
        }
    }
    if (last == -1) return {inf, {}, false};
    std::vector<int> path;
    std::uint64_t mask = full;
    while (last != -1) {
        path.push_back(last);
        const int previous = parent[index(mask, last)];
        mask ^= 1ULL << last;
        last = previous;
    }
    std::ranges::reverse(path);
    path.push_back(start);
    return {answer, std::move(path), true};
}

}
