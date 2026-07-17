#define PROBLEM "https://atcoder.jp/contests/abc317/tasks/abc317_h"
#include <bits/stdc++.h>
#include "fps/polynomial.hpp"
#include "fps/rational.hpp"

using namespace std;
using namespace poe;

int main() {
    using mint = modint998244353;
    using series = fps998244353;
    int n;
    long long walk_length;
    cin >> n >> walk_length;
    vector<int> self_loop(n), next_edge(n), skip_edge(n), return_edge(n);
    for (int& value : self_loop) cin >> value;
    for (int& value : next_edge) cin >> value;
    for (int& value : skip_edge) cin >> value;
    for (int& value : return_edge) cin >> value;

    vector<matrix<series>> transitions;
    transitions.reserve(n - 1);
    int denominator_power = 0;
    for (int vertex = n - 1; vertex >= 1; --vertex) {
        matrix<series> transition(3, 3);
        const series denominator{1, -self_loop[vertex]};
        const series from_previous{0, next_edge[vertex - 1]};
        const series from_two_before{
            0, vertex >= 2 ? skip_edge[vertex - 2] : 0
        };
        transition[0][0] = from_previous;
        transition[0][1] = from_two_before;
        transition[1][0] = denominator;
        transition[2][0] = from_previous * mint{return_edge[vertex]};
        transition[2][1] = from_two_before * mint{return_edge[vertex]};
        transition[2][2] = denominator;
        transitions.push_back(std::move(transition));
        denominator_power += self_loop[vertex];
    }
    const auto product = polynomial_matrix_product<998244353>(
        std::move(transitions), n + 1
    );
    series numerator_to_goal = product[0][0]
        + product[0][2] * mint{return_edge[0]};
    series numerator_return = product[2][0]
        + product[2][2] * mint{return_edge[0]};
    const series common_denominator =
        series{1, -1}.pow(denominator_power, n + 1);
    const series answer_denominator =
        common_denominator - series{0, 1} * numerator_return;
    cout << rationalfps<998244353>{
        std::move(numerator_to_goal), answer_denominator
    }.coeff(walk_length) << '\n';
}
