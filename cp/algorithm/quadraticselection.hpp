#pragma once
#include <bits/stdc++.h>

namespace poe {

namespace detail {

struct quadraticselectionevent {
    __int128_t numerator;
    __int128_t denominator;
    int first;
    int second;
};

inline int quadraticselection_compare_positive_fraction(
    __uint128_t left_numerator,
    __uint128_t left_denominator,
    __uint128_t right_numerator,
    __uint128_t right_denominator
) {
    bool reversed = false;
    while (true) {
        const __uint128_t left_quotient = left_numerator / left_denominator;
        const __uint128_t right_quotient = right_numerator / right_denominator;
        if (left_quotient != right_quotient) {
            const int comparison = left_quotient < right_quotient ? -1 : 1;
            return reversed ? -comparison : comparison;
        }
        const __uint128_t left_remainder = left_numerator % left_denominator;
        const __uint128_t right_remainder = right_numerator % right_denominator;
        if (left_remainder == 0 || right_remainder == 0) {
            if (left_remainder == right_remainder) return 0;
            const int comparison = left_remainder == 0 ? -1 : 1;
            return reversed ? -comparison : comparison;
        }
        left_numerator = left_denominator;
        left_denominator = left_remainder;
        right_numerator = right_denominator;
        right_denominator = right_remainder;
        reversed = !reversed;
    }
}

inline int quadraticselection_compare_event(
    const quadraticselectionevent& left,
    const quadraticselectionevent& right
) {
    __int128_t lhs, rhs;
    if (!__builtin_mul_overflow(left.numerator, right.denominator, &lhs)
        && !__builtin_mul_overflow(right.numerator, left.denominator, &rhs)) {
        if (lhs < rhs) return -1;
        if (lhs > rhs) return 1;
        return 0;
    }
    if (left.numerator < 0 && right.numerator >= 0) return -1;
    if (left.numerator >= 0 && right.numerator < 0) return 1;
    if (left.numerator >= 0) {
        return quadraticselection_compare_positive_fraction(
            left.numerator, left.denominator, right.numerator, right.denominator
        );
    }
    return -quadraticselection_compare_positive_fraction(
        -left.numerator, left.denominator, -right.numerator, right.denominator
    );
}

inline bool quadraticselection_event_less(
    const quadraticselectionevent& left,
    const quadraticselectionevent& right
) {
    const int comparison = quadraticselection_compare_event(left, right);
    if (comparison != 0) return comparison > 0;
    if (left.first != right.first) return left.first < right.first;
    return left.second < right.second;
}

inline bool quadraticselection_same_event(
    const quadraticselectionevent& left,
    const quadraticselectionevent& right
) {
    return quadraticselection_compare_event(left, right) == 0;
}

struct quadraticselectiondsu {
    std::vector<int> parent;

    explicit quadraticselectiondsu(int size) : parent(size, -1) {}

    int leader(int vertex) {
        if (parent[vertex] < 0) return vertex;
        return parent[vertex] = leader(parent[vertex]);
    }

    void merge(int left, int right) {
        left = leader(left);
        right = leader(right);
        if (left == right) return;
        if (parent[left] > parent[right]) std::swap(left, right);
        parent[left] += parent[right];
        parent[right] = left;
    }
};

}

/// maximum_quadratic_k_selection(points, k): O(n^2 log n)時間・O(n^2)領域で、k点を選ぶときの(sum x)^2+sum yの最大値を返す。
inline __int128_t maximum_quadratic_k_selection(
    const std::vector<std::pair<long long, long long>>& points,
    int k
) {
    const int size = static_cast<int>(points.size());
    assert(0 <= k && k <= size);

    std::vector<int> order(size), position(size);
    std::iota(order.begin(), order.end(), 0);
    std::ranges::sort(order, [&](int left, int right) {
        if (points[left].first != points[right].first) {
            return points[left].first > points[right].first;
        }
        if (points[left].second != points[right].second) {
            return points[left].second > points[right].second;
        }
        return left < right;
    });
    for (int rank = 0; rank < size; ++rank) position[order[rank]] = rank;

    __int128_t x_sum = 0;
    __int128_t y_sum = 0;
    for (int rank = 0; rank < k; ++rank) {
        x_sum += points[order[rank]].first;
        y_sum += points[order[rank]].second;
    }
    __int128_t answer = x_sum * x_sum + y_sum;
    if (k == 0 || k == size) return answer;

    std::vector<detail::quadraticselectionevent> events;
    events.reserve(static_cast<std::size_t>(size) * (size - 1) / 2);
    for (int first = 0; first < size; ++first) {
        for (int second = first + 1; second < size; ++second) {
            if (points[first].first == points[second].first) continue;
            __int128_t numerator = static_cast<__int128_t>(points[second].second)
                - points[first].second;
            __int128_t denominator = static_cast<__int128_t>(points[first].first)
                - points[second].first;
            if (denominator < 0) {
                numerator = -numerator;
                denominator = -denominator;
            }
            events.push_back({numerator, denominator, first, second});
        }
    }
    std::ranges::sort(events, detail::quadraticselection_event_less);

    std::vector<int> local_index(size, -1);
    for (std::size_t event_left = 0; event_left < events.size();) {
        std::size_t event_right = event_left + 1;
        while (event_right < events.size()
               && detail::quadraticselection_same_event(events[event_left], events[event_right])) {
            ++event_right;
        }

        std::vector<int> vertices;
        vertices.reserve((event_right - event_left) * 2);
        for (std::size_t event = event_left; event < event_right; ++event) {
            vertices.push_back(events[event].first);
            vertices.push_back(events[event].second);
        }
        std::ranges::sort(vertices);
        vertices.erase(std::unique(vertices.begin(), vertices.end()), vertices.end());
        for (int index = 0; index < static_cast<int>(vertices.size()); ++index) {
            local_index[vertices[index]] = index;
        }

        detail::quadraticselectiondsu dsu(static_cast<int>(vertices.size()));
        for (std::size_t event = event_left; event < event_right; ++event) {
            dsu.merge(local_index[events[event].first], local_index[events[event].second]);
        }
        std::vector<std::vector<int>> components(vertices.size());
        for (int vertex : vertices) {
            components[dsu.leader(local_index[vertex])].push_back(vertex);
        }

        for (auto& component : components) {
            if (component.empty()) continue;
            std::vector<int> positions;
            positions.reserve(component.size());
            for (int vertex : component) positions.push_back(position[vertex]);
            std::ranges::sort(positions);
            std::ranges::sort(component, [&](int left, int right) {
                if (points[left].first != points[right].first) {
                    return points[left].first < points[right].first;
                }
                return left < right;
            });

            for (int rank : positions) {
                if (rank < k) {
                    x_sum -= points[order[rank]].first;
                    y_sum -= points[order[rank]].second;
                }
            }
            for (int index = 0; index < static_cast<int>(positions.size()); ++index) {
                const int rank = positions[index];
                order[rank] = component[index];
                position[component[index]] = rank;
                if (rank < k) {
                    x_sum += points[order[rank]].first;
                    y_sum += points[order[rank]].second;
                }
            }
        }
        answer = std::max(answer, x_sum * x_sum + y_sum);

        for (int vertex : vertices) local_index[vertex] = -1;
        event_left = event_right;
    }
    return answer;
}

}
