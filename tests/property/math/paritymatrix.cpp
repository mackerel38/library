#include "math/modint.hpp"
#include "math/paritymatrix.hpp"

namespace {

using mint = poe::staticmodint<1000000007>;

long long brute(int rows, const std::vector<int>& column_sums) {
    std::vector<std::vector<int>> matrix(rows, std::vector<int>(column_sums.size()));
    long long answer = 0;
    auto enumerate_column = [&](auto&& self, int column, int row, int remaining) -> void {
        if (column == static_cast<int>(column_sums.size())) {
            for (int i = 0; i < rows; ++i) {
                bool all_even = true;
                for (const int value : matrix[i]) all_even &= value % 2 == 0;
                if (all_even) return;
            }
            ++answer;
            return;
        }
        if (row + 1 == rows) {
            matrix[row][column] = remaining;
            self(self, column + 1, 0,
                 column + 1 == static_cast<int>(column_sums.size())
                     ? 0 : column_sums[column + 1]);
            return;
        }
        for (int value = 0; value <= remaining; ++value) {
            matrix[row][column] = value;
            self(self, column, row + 1, remaining - value);
        }
    };
    if (column_sums.empty()) return rows == 0 ? 1 : 0;
    enumerate_column(enumerate_column, 0, 0, column_sums[0]);
    return answer;
}

}

int main() {
    for (int rows = 1; rows <= 4; ++rows) {
        for (int columns = 1; columns <= 3; ++columns) {
            std::vector<int> sums(columns);
            auto enumerate = [&](auto&& self, int index) -> void {
                if (index == columns) {
                    assert(poe::count_matrices_without_even_rows<mint>(rows, sums)
                           == mint{brute(rows, sums)});
                    return;
                }
                for (sums[index] = 0; sums[index] <= 5; ++sums[index]) {
                    self(self, index + 1);
                }
            };
            enumerate(enumerate, 0);
        }
    }
}
