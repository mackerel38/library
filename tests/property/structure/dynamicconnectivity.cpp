#include "structure/dynamicconnectivity.hpp"

int main() {
    std::mt19937 random(731927);
    for (int trial = 0; trial < 200; ++trial) {
        const int n = 1 + random() % 12;
        const int q = 1 + random() % 80;
        poe::dynamicconnectivity dc(n, q);
        std::set<std::pair<int, int>> edges;
        std::vector<int> ids;
        std::vector<int> expected;

        for (int time = 0; time < q; ++time) {
            if (random() % 3 == 0) {
                int a = random() % n;
                int b = random() % n;
                if (a > b) std::swap(a, b);
                if (!edges.contains({a, b})) {
                    edges.emplace(a, b);
                    dc.add(time, a, b);
                }
            } else if (!edges.empty() && random() % 4 == 0) {
                auto iterator = edges.begin();
                std::advance(iterator, random() % edges.size());
                dc.remove(time, iterator->first, iterator->second);
                edges.erase(iterator);
            }

            std::vector<std::vector<int>> graph(n);
            for (auto [a, b] : edges) {
                graph[a].push_back(b);
                graph[b].push_back(a);
            }
            const int start = random() % n;
            std::vector<bool> seen(n);
            std::queue<int> queue;
            seen[start] = true;
            queue.push(start);
            while (!queue.empty()) {
                const int vertex = queue.front();
                queue.pop();
                for (int next : graph[vertex]) {
                    if (!seen[next]) seen[next] = true, queue.push(next);
                }
            }
            const int target = random() % n;
            ids.push_back(dc.same(time, start, target));
            expected.push_back(seen[target]);
            ids.push_back(dc.size(time, start));
            expected.push_back(std::count(seen.begin(), seen.end(), true));

            int count = 0;
            std::vector<bool> visited(n);
            for (int vertex = 0; vertex < n; ++vertex) {
                if (visited[vertex]) continue;
                ++count;
                std::queue<int> component;
                component.push(vertex);
                visited[vertex] = true;
                while (!component.empty()) {
                    int current = component.front();
                    component.pop();
                    for (int next : graph[current]) {
                        if (!visited[next]) visited[next] = true, component.push(next);
                    }
                }
            }
            ids.push_back(dc.count(time));
            expected.push_back(count);
        }

        const auto actual = dc.solve();
        for (int index = 0; index < static_cast<int>(ids.size()); ++index) {
            assert(actual[ids[index]] == expected[index]);
        }
    }
}
