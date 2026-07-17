#include <bits/stdc++.h>
#include "string/runlength.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string source, target;
    cin >> source >> target;
    const auto first = runlength(source);
    const auto second = runlength(target);
    bool possible = first.size() == second.size();
    for (int index = 0; possible && index < static_cast<int>(first.size()); ++index) {
        const auto [source_character, source_count] = first[index];
        const auto [target_character, target_count] = second[index];
        possible = source_character == target_character && source_count <= target_count &&
                   (source_count >= 2 || target_count == 1);
    }
    cout << (possible ? "Yes\n" : "No\n");
}
