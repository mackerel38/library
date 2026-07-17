#include <bits/stdc++.h>
#include "geometry/rectangleunion.hpp"
#include "structure/dsu.hpp"

using namespace std;
using namespace poe;

using rectangle = axisalignedrectangle<long long>;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    long long width, height;
    cin >> n >> width >> height;
    vector<rectangle> rectangles{{0, width, 0, height}};
    while (n--) {
        char type;
        long long coordinate, shift;
        cin >> type >> coordinate >> shift;
        vector<rectangle> next;
        for (const rectangle& current : rectangles) {
            const auto pieces = type == 'X'
                ? split_rectangle_x(current, coordinate)
                : split_rectangle_y(current, coordinate);
            for (const rectangle& piece : pieces) {
                if (type == 'X') {
                    next.push_back(translate_rectangle(
                        piece, 0LL, piece.right <= coordinate ? -shift : shift));
                } else {
                    next.push_back(translate_rectangle(
                        piece, piece.top <= coordinate ? -shift : shift, 0LL));
                }
            }
        }
        rectangles = move(next);
    }

    dsu components(rectangles.size());
    for (int first = 0; first < static_cast<int>(rectangles.size()); ++first) {
        for (int second = first + 1; second < static_cast<int>(rectangles.size()); ++second) {
            if (rectangles_edge_adjacent(rectangles[first], rectangles[second])) {
                components.merge(first, second);
            }
        }
    }
    vector<long long> sizes(rectangles.size());
    for (int index = 0; index < static_cast<int>(rectangles.size()); ++index) {
        sizes[components.leader(index)] += rectangle_area(rectangles[index]);
    }
    erase(sizes, 0);
    ranges::sort(sizes);
    cout << sizes.size() << '\n';
    for (int index = 0; index < static_cast<int>(sizes.size()); ++index) {
        if (index) cout << ' ';
        cout << sizes[index];
    }
    cout << '\n';
}
