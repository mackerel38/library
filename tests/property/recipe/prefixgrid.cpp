#include "recipe/prefixgrid.hpp"

using namespace std;
using namespace poe;

int brute(const vector<string>& pattern) {
    const int height = pattern.size();
    const int width = pattern[0].size();
    set<unsigned long long> grids;
    vector<int> row_prefix(height), column_prefix(width);
    const auto enumerate_columns = [&](this auto&& self, int column) -> void {
        if (column < width) {
            for (column_prefix[column] = 0; column_prefix[column] <= height; ++column_prefix[column]) {
                self(column + 1);
            }
            return;
        }
        unsigned long long grid = 0;
        for (int row = 0; row < height; ++row) {
            for (int current = 0; current < width; ++current) {
                if (current < row_prefix[row] || row < column_prefix[current]) {
                    grid |= 1ULL << (row * width + current);
                }
            }
        }
        for (int row = 0; row < height; ++row) {
            for (int column = 0; column < width; ++column) {
                const bool value = grid >> (row * width + column) & 1ULL;
                if (pattern[row][column] != '?' && value != (pattern[row][column] == '1')) return;
            }
        }
        grids.insert(grid);
    };
    const auto enumerate_rows = [&](this auto&& self, int row) -> void {
        if (row < height) {
            for (row_prefix[row] = 0; row_prefix[row] <= width; ++row_prefix[row]) {
                self(row + 1);
            }
            return;
        }
        enumerate_columns(0);
    };
    enumerate_rows(0);
    return grids.size();
}

int main() {
    mt19937 random(20260716);
    for (int height = 1; height <= 3; ++height) {
        for (int width = 1; width <= 3; ++width) {
            for (int trial = 0; trial < 500; ++trial) {
                vector<string> pattern(height, string(width, '?'));
                for (string& row : pattern) {
                    for (char& cell : row) cell = "01?"[random() % 3];
                }
                assert(count_row_column_prefix_union<998244353>(pattern) == brute(pattern));
            }
        }
    }
}
