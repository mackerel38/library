#include <bits/stdc++.h>
#include "algorithm/rangemin.hpp"
#include "math/math.hpp"
#include "math/modint.hpp"

using namespace std;
using namespace poe;

using mint = modint998244353;

static mint solve_coprime(const vector<long long>& a, const vector<long long>& b,
                          unsigned long long length) {
    using query = rangeminsumquery<long long>;
    struct contribution { int query_index; unsigned long long multiplier; };
    vector<long long> permuted_b(b.size());
    for (int i = 0; i < static_cast<int>(b.size()); ++i) {
        permuted_b[i] = b[1LL * a.size() * i % b.size()];
    }
    const long long inverse = inv_mod(a.size(), b.size());
    vector<query> queries;
    vector<contribution> contributions;
    for (int index = 0; index < static_cast<int>(a.size()) &&
         static_cast<unsigned long long>(index) < length; ++index) {
        const unsigned long long count = (length - 1 - index) / a.size() + 1;
        const unsigned long long full = count / b.size();
        const int remainder = count % b.size();
        const int start = 1LL * index * inverse % b.size();
        if (full > 0) {
            contributions.push_back({static_cast<int>(queries.size()), full});
            queries.push_back({0, static_cast<int>(b.size()), a[index]});
        }
        if (remainder > 0) {
            const int middle = min<int>(b.size(), start + remainder);
            contributions.push_back({static_cast<int>(queries.size()), 1});
            queries.push_back({start, middle, a[index]});
            if (middle < start + remainder) {
                contributions.push_back({static_cast<int>(queries.size()), 1});
                queries.push_back({0, start + remainder - middle, a[index]});
            }
        }
    }
    const auto sums = offline_range_min_sum(permuted_b, queries);
    mint answer = 0;
    for (const auto& [query_index, multiplier] : contributions) {
        answer += mint{sums[query_index]} * mint{static_cast<long long>(multiplier)};
    }
    return answer;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    unsigned long long k;
    cin >> n >> m >> k;
    vector<long long> a(n), b(m);
    for (auto& value : a) cin >> value;
    for (auto& value : b) cin >> value;

    const int divisor = gcd(n, m);
    mint answer = 0;
    for (int residue = 0; residue < divisor && static_cast<unsigned long long>(residue) < k; ++residue) {
        vector<long long> reduced_a, reduced_b;
        for (int i = residue; i < n; i += divisor) reduced_a.push_back(a[i]);
        for (int i = residue; i < m; i += divisor) reduced_b.push_back(b[i]);
        const unsigned long long length = (k - 1 - residue) / divisor + 1;
        answer += solve_coprime(reduced_a, reduced_b, length);
    }
    cout << answer.val() << '\n';
}
