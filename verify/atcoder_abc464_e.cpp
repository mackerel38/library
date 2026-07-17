#include <bits/stdc++.h>
#include "structure/cumulative.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int height, width, query_count;
    cin >> height >> width >> query_count;
    vector<int> latest(height * width);
    vector<char> letters = {'A'};
    for (int query = 1; query <= query_count; ++query) {
        int row, column;
        char letter;
        cin >> row >> column >> letter;
        latest[(row - 1) * width + column - 1] = query;
        letters.push_back(letter);
    }
    latest = multidimensional_cumulative(
        vector{height, width}, std::move(latest),
        [](int first, int second) { return max(first, second); },
        cumulativedirection::suffix);
    for (int row = 0; row < height; ++row) {
        for (int column = 0; column < width; ++column) {
            cout << letters[latest[row * width + column]];
        }
        cout << '\n';
    }
}
