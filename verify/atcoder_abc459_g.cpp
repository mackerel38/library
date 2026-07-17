#include <bits/stdc++.h>
#include "math/math.hpp"

using namespace std;
using namespace poe;

using i128 = __int128_t;

struct line {
    long long a, b, c;
};

long long floor_div(i128 numerator, i128 denominator) {
    if (denominator < 0) numerator = -numerator, denominator = -denominator;
    i128 quotient = numerator / denominator;
    if (numerator % denominator < 0) --quotient;
    return static_cast<long long>(quotient);
}

pair<long long, long long> solve_linear(long long a, long long b, long long value) {
    if (b == 1) return {0, value};
    const long long remainder = (value % b + b) % b;
    const long long first = static_cast<long long>(
        static_cast<i128>(remainder) * inv_mod(a, b) % b);
    return {first, (value - a * first) / b};
}

long long solve(long long a, long long b, long long x, long long y) {
    const long long divisor = gcd(a, b);
    if (x % divisor || y % divisor) return -1;
    a /= divisor;
    b /= divisor;
    x /= divisor;
    y /= divisor;

    long long answer = numeric_limits<long long>::max();
    for (int parity1 = 0; parity1 < 2; ++parity1) {
        for (int parity2 = 0; parity2 < 2; ++parity2) {
            const long long first_rhs = x - parity1 * a - parity2 * b;
            const long long second_rhs = y - parity1 * b - parity2 * a;
            if (first_rhs % 2 || second_rhs % 2) continue;

            const auto [x1, x2] = solve_linear(a, b, first_rhs / 2);
            const auto [y1, y2] = solve_linear(b, a, second_rhs / 2);
            const long long u0 = 2 * x1 + parity1;
            const long long v0 = 2 * x2 + parity2;
            const long long p0 = 2 * y1 + parity1;
            const long long q0 = 2 * y2 + parity2;

            array<line, 4> lines;
            for (int sign_index = 0; sign_index < 2; ++sign_index) {
                const long long sign = sign_index ? -1 : 1;
                lines[sign_index] = {2 * b, -sign * 2 * a, sign * p0 - u0};
                lines[2 + sign_index] = {-2 * a, sign * 2 * b, sign * q0 - v0};
            }

            auto evaluate = [&](long long n, long long m) {
                const long long u = u0 + 2 * b * n;
                const long long v = v0 - 2 * a * n;
                const long long p = p0 + 2 * a * m;
                const long long q = q0 - 2 * b * m;
                answer = min(answer, max(abs(u), abs(p)) + max(abs(v), abs(q)));
            };
            for (int i = 0; i < 4; ++i) {
                for (int j = i + 1; j < 4; ++j) {
                    const i128 determinant = static_cast<i128>(lines[i].a) * lines[j].b -
                                             static_cast<i128>(lines[j].a) * lines[i].b;
                    if (determinant == 0) continue;
                    const i128 n_numerator = static_cast<i128>(lines[i].c) * lines[j].b -
                                             static_cast<i128>(lines[j].c) * lines[i].b;
                    const i128 m_numerator = static_cast<i128>(lines[i].a) * lines[j].c -
                                             static_cast<i128>(lines[j].a) * lines[i].c;
                    const long long center_n = floor_div(n_numerator, determinant);
                    const long long center_m = floor_div(m_numerator, determinant);
                    for (long long dn = -3; dn <= 3; ++dn) {
                        for (long long dm = -3; dm <= 3; ++dm) {
                            evaluate(center_n + dn, center_m + dm);
                        }
                    }
                }
            }
        }
    }
    return answer == numeric_limits<long long>::max() ? -1 : answer;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int test_count;
    cin >> test_count;
    while (test_count--) {
        long long a, b, x, y;
        cin >> a >> b >> x >> y;
        cout << solve(a, b, x, y) << '\n';
    }
}
