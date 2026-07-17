#pragma once
#include <bits/stdc++.h>

namespace poe {

/// O(n)。z[i]=LCP(sequence, sequence[i..])となるZ配列を返す。
template <class T>
std::vector<int> z_algorithm(const std::vector<T>& sequence) {
    const int n = static_cast<int>(sequence.size());
    if (n == 0) {
        return {};
    }
    std::vector<int> z(n);
    for (int i = 1, best = 0; i < n; ++i) {
        int& length = z[i];
        length = best + z[best] <= i ? 0 : std::min(best + z[best] - i, z[i - best]);
        while (i + length < n && sequence[length] == sequence[i + length]) {
            ++length;
        }
        if (best + z[best] < i + z[i]) {
            best = i;
        }
    }
    z[0] = n;
    return z;
}

/// O(n)。文字列textのZ配列を返す。
inline std::vector<int> z_algorithm(const std::string& text) {
    return z_algorithm(std::vector<char>(text.begin(), text.end()));
}

}
