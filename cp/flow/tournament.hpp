#pragma once
#include <bits/stdc++.h>
#include "flow/dinic.hpp"

namespace poe {

/// 総当たり戦を単独優勝で完了できるかと、その全試合の{勝者,敗者}を表す。
struct roundrobincompletion {
    bool possible = false;
    std::vector<std::pair<int, int>> results;
};

/// O(n^3)時間・O(n^2)領域。既知結果を保ち、winnerが単独優勝する総当たり戦を一つ構成する。
inline roundrobincompletion complete_roundrobin_for_unique_winner(
    int players,
    const std::vector<std::pair<int, int>>& decided_results,
    int winner
) {
    assert(players >= 2);
    assert(0 <= winner && winner < players);
    std::vector played(players, std::vector<char>(players));
    std::vector<int> fixed_wins(players);
    for (const auto& [win, lose] : decided_results) {
        assert(0 <= win && win < players && 0 <= lose && lose < players && win != lose);
        assert(!played[win][lose]);
        played[win][lose] = played[lose][win] = true;
        ++fixed_wins[win];
    }

    roundrobincompletion result{true, decided_results};
    int winner_score = fixed_wins[winner];
    for (int opponent = 0; opponent < players; ++opponent) {
        if (opponent == winner || played[winner][opponent]) continue;
        played[winner][opponent] = played[opponent][winner] = true;
        result.results.push_back({winner, opponent});
        ++winner_score;
    }
    if (winner_score == 0) return {};

    std::vector<std::pair<int, int>> undecided;
    for (int left = 0; left < players; ++left) {
        for (int right = left + 1; right < players; ++right) {
            if (!played[left][right]) undecided.push_back({left, right});
        }
    }
    const int games = static_cast<int>(undecided.size());
    const int game_base = 0;
    const int player_base = games;
    const int source = player_base + players;
    const int sink = source + 1;
    flowgraph<int> graph(sink + 1);
    std::vector<std::array<int, 2>> assignment_edges(games);
    for (int game = 0; game < games; ++game) {
        graph.add_edge(source, game_base + game, 1);
        const auto [left, right] = undecided[game];
        assignment_edges[game][0] = graph.add_edge(game_base + game, player_base + left, 1);
        assignment_edges[game][1] = graph.add_edge(game_base + game, player_base + right, 1);
    }
    for (int player = 0; player < players; ++player) {
        if (player == winner) continue;
        const int capacity = winner_score - 1 - fixed_wins[player];
        if (capacity < 0) return {};
        graph.add_edge(player_base + player, sink, capacity);
    }
    if (dinic(graph, source, sink) != games) return {};
    for (int game = 0; game < games; ++game) {
        const auto [left, right] = undecided[game];
        if (graph.edge_at(assignment_edges[game][0]).flow == 1) {
            result.results.push_back({left, right});
        } else {
            assert(graph.edge_at(assignment_edges[game][1]).flow == 1);
            result.results.push_back({right, left});
        }
    }
    assert(static_cast<int>(result.results.size()) == players * (players - 1) / 2);
    return result;
}

/// O(n^4)時間・O(n^2)領域。既知結果から単独優勝できる選手を昇順で返す。
inline std::vector<int> possible_unique_roundrobin_winners(
    int players,
    const std::vector<std::pair<int, int>>& decided_results
) {
    std::vector<int> result;
    for (int winner = 0; winner < players; ++winner) {
        if (complete_roundrobin_for_unique_winner(players, decided_results, winner).possible) {
            result.push_back(winner);
        }
    }
    return result;
}

}
