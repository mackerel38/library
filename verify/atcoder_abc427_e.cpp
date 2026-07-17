#define PROBLEM "https://atcoder.jp/contests/abc427/tasks/abc427_e"
#include <bits/stdc++.h>
#include "graph/translation.hpp"

using namespace std;
using namespace poe;

int main() {
    int height, width;
    cin >> height >> width;
    vector<gridpoint> occupied;
    vector<gridpoint> forbidden;
    for (int row = 0; row < height; ++row) {
        string cells;
        cin >> cells;
        for (int column = 0; column < width; ++column) {
            if (cells[column] == '#') occupied.push_back({row, column});
            if (cells[column] == 'T') forbidden.push_back({row, column});
        }
    }
    cout << minimum_safe_translation_clear_steps(
        height, width, occupied, forbidden
    ) << '\n';
}
