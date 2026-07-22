#include "geometry/line.hpp"

using namespace std;
using namespace poe;

int main() {
    mt19937 random(263);
    for (int iteration = 0; iteration < 3000; ++iteration) {
        const int n = 2 + static_cast<int>(random() % 18);
        vector<int> slopes;
        while (static_cast<int>(slopes.size()) < n) {
            const int slope = static_cast<int>(random() % 101) - 50;
            if (ranges::find(slopes, slope) == slopes.end()) slopes.push_back(slope);
        }
        vector<line<long long>> lines;
        for (const int slope : slopes) {
            lines.push_back({slope, -1, static_cast<int>(random() % 101) - 50});
        }
        vector<long double> distances;
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                const auto point = line_intersection(lines[i], lines[j]).value();
                distances.push_back(std::hypotl(point.x, point.y));
            }
        }
        ranges::sort(distances);
        const int rank = static_cast<int>(random() % distances.size());
        const long double radius = rank + 1 < static_cast<int>(distances.size())
            ? std::midpoint(distances[rank], distances[rank + 1])
            : distances.back() + 1;
        const long long expected = lower_bound(distances.begin(), distances.end(), radius)
                                 - distances.begin();
        assert(count_line_intersections_in_circle(lines, {}, radius) == expected);

        const int kth = 1 + static_cast<int>(random() % distances.size());
        const long double actual = kth_line_intersection_distance(lines, kth, 70);
        assert(std::abs(actual - distances[kth - 1]) <=
               1e-9L * std::max(1.0L, distances[kth - 1]));
    }
}
