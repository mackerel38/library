#include <bits/stdc++.h>

using namespace std;

struct line_data {
    double slope;
    double intercept;
    int index;
};

pair<double, vector<int>> envelope_minimum(vector<line_data> lines) {
    sort(lines.begin(), lines.end(), [](const line_data& first, const line_data& second) {
        if (first.slope != second.slope) return first.slope < second.slope;
        return first.intercept > second.intercept;
    });
    vector<line_data> unique_lines;
    for (const auto& value : lines) {
        if (unique_lines.empty() || unique_lines.back().slope != value.slope)
            unique_lines.push_back(value);
    }

    vector<pair<line_data, double>> envelope;
    for (const auto& value : unique_lines) {
        bool unnecessary = false;
        while (!envelope.empty()) {
            const auto& [last, start] = envelope.back();
            const double crossing = (last.intercept - value.intercept)
                                    / (value.slope - last.slope);
            if (crossing >= 1.0) {
                unnecessary = true;
                break;
            }
            if (crossing <= start) envelope.pop_back();
            else {
                envelope.push_back({value, crossing});
                unnecessary = true;
                break;
            }
        }
        if (!unnecessary) envelope.push_back({value, 0.0});
    }

    if (envelope.front().first.slope >= 0.0)
        return {envelope.front().first.intercept, {envelope.front().first.index}};
    if (envelope.back().first.slope < 0.0) {
        const auto& value = envelope.back().first;
        return {value.slope + value.intercept, {value.index}};
    }
    for (int i = 1; i < static_cast<int>(envelope.size()); ++i) {
        if (envelope[i - 1].first.slope < 0.0 && envelope[i].first.slope >= 0.0) {
            const auto& value = envelope[i].first;
            const double x = envelope[i].second;
            return {value.slope * x + value.intercept,
                    {envelope[i - 1].first.index, value.index}};
        }
    }
    assert(false);
    return {};
}

struct plane {
    array<double, 3> normal;
    double offset;
};

array<double, 3> cross(array<double, 3> first, array<double, 3> second) {
    return {first[1] * second[2] - first[2] * second[1],
            first[2] * second[0] - first[0] * second[2],
            first[0] * second[1] - first[1] * second[0]};
}

double dot(array<double, 3> first, array<double, 3> second) {
    return first[0] * second[0] + first[1] * second[1] + first[2] * second[2];
}

optional<array<double, 3>> intersection(const plane& first,
                                        const plane& second,
                                        const plane& third) {
    const auto second_third = cross(second.normal, third.normal);
    const double determinant = dot(first.normal, second_third);
    if (abs(determinant) <= 1e-8) return nullopt;
    const auto third_first = cross(third.normal, first.normal);
    const auto first_second = cross(first.normal, second.normal);
    array<double, 3> result{};
    for (int axis = 0; axis < 3; ++axis) {
        result[axis] = (first.offset * second_third[axis]
                        + second.offset * third_first[axis]
                        + third.offset * first_second[axis]) / determinant;
    }
    return result;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int test_cases;
    cin >> test_cases;
    cout << fixed << setprecision(15);
    while (test_cases--) {
        int n;
        cin >> n;
        vector<array<double, 3>> probability(n);
        for (auto& row : probability)
            for (double& value : row) cin >> value;

        vector<array<double, 3>> game_value(n);
        for (int banned = 0; banned < 3; ++banned) {
            auto make_lines = [&](int excluded = -1) {
                vector<line_data> lines;
                for (int i = 0; i < n; ++i) {
                    if (i == excluded) continue;
                    const double first = probability[i][banned];
                    const double second = probability[i][(banned + 1) % 3];
                    lines.push_back({first - second, second, i});
                }
                return lines;
            };
            const auto [minimum, exceptional] = envelope_minimum(make_lines());
            for (auto& value : game_value) value[banned] = minimum;
            for (int index : exceptional)
                game_value[index][banned] = envelope_minimum(make_lines(index)).first;
        }

        sort(game_value.begin(), game_value.end());
        game_value.erase(unique(game_value.begin(), game_value.end()), game_value.end());
        vector<plane> planes;
        for (const auto [p, q, r] : game_value)
            planes.push_back({{r - p, r - q, 1.0}, r});
        const int payoff_planes = planes.size();
        planes.push_back({{1.0, 0.0, 0.0}, 0.0});
        planes.push_back({{0.0, 1.0, 0.0}, 0.0});
        planes.push_back({{1.0, 1.0, 0.0}, 1.0});

        double answer = 0.0;
        for (int i = 0; i < static_cast<int>(planes.size()); ++i) {
            for (int j = 0; j < i; ++j) {
                for (int k = 0; k < j; ++k) {
                    const auto point = intersection(planes[i], planes[j], planes[k]);
                    if (!point) continue;
                    const auto [x, y, z] = *point;
                    if (x < -1e-6 || y < -1e-6 || x + y > 1.0 + 1e-6) continue;
                    double value = numeric_limits<double>::infinity();
                    for (int index = 0; index < payoff_planes; ++index) {
                        const auto& [normal, offset] = planes[index];
                        value = min(value, offset - normal[0] * x - normal[1] * y);
                    }
                    answer = max(answer, value);
                }
            }
        }
        cout << answer / 1e6 << '\n';
    }
}
