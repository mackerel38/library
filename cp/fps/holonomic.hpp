#pragma once
#include <bits/stdc++.h>

namespace poe {

/// O(nr)。sum_{d=0}^r coefficient(i,d)*a[i-d]=0を満たすP再帰列を列挙する。
/// singular(i,a)がoptionalの値を返した添字では、その値を漸化式より優先する。
template<class T, class Coefficient, class Singular>
std::vector<T> holonomic_sequence(
    int size,
    int order,
    Coefficient coefficient,
    Singular singular
) {
    assert(size >= 0 && order >= 0);
    std::vector<T> result(size);
    for (int index = 0; index < size; ++index) {
        if (auto value = singular(index, result)) {
            result[index] = *value;
            continue;
        }
        const T lead = coefficient(index, 0);
        assert(lead != T{});
        T right{};
        for (int distance = 1; distance <= order && distance <= index; ++distance) {
            right += coefficient(index, distance) * result[index - distance];
        }
        result[index] = -right / lead;
    }
    return result;
}

}
