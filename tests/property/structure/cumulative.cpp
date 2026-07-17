#include <algorithm>
#include <cassert>
#include <random>
#include <vector>
#include "structure/cumulative.hpp"

using namespace std;
using namespace poe;

vector<int> coordinates(int index, const vector<int>& shape) {
    vector<int> result(shape.size());
    for (int dimension = static_cast<int>(shape.size()) - 1; dimension >= 0; --dimension) {
        result[dimension] = index % shape[dimension];
        index /= shape[dimension];
    }
    return result;
}

int main() {
    mt19937 random(20260715);
    for (int dimensions = 1; dimensions <= 5; ++dimensions) {
        for (int repetition = 0; repetition < 100; ++repetition) {
            vector<int> shape(dimensions);
            int size = 1;
            for (int& extent : shape) {
                extent = 1 + random() % 3;
                size *= extent;
            }
            vector<int> values(size);
            for (int& value : values) value = static_cast<int>(random() % 1000);
            for (auto direction : {cumulativedirection::prefix, cumulativedirection::suffix}) {
                auto result = multidimensional_cumulative(
                    shape, values, [](int a, int b) { return max(a, b); }, direction);
                for (int index = 0; index < size; ++index) {
                    const auto target = coordinates(index, shape);
                    int expected = 0;
                    for (int source = 0; source < size; ++source) {
                        const auto current = coordinates(source, shape);
                        bool included = true;
                        for (int dimension = 0; dimension < dimensions; ++dimension) {
                            if (direction == cumulativedirection::prefix) {
                                included &= current[dimension] <= target[dimension];
                            } else {
                                included &= target[dimension] <= current[dimension];
                            }
                        }
                        if (included) expected = max(expected, values[source]);
                    }
                    assert(result[index] == expected);
                }
            }
        }
    }
}
