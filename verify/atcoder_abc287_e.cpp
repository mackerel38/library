#include <bits/stdc++.h>
#include "string/trie.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<string> words(n);
    trie<char> dictionary;
    for (string& word : words) {
        cin >> word;
        dictionary.add(word);
    }
    for (const string& word : words) {
        int node = 0;
        int answer = 0;
        for (int index = 0; index < static_cast<int>(word.size()); ++index) {
            node = dictionary.next(node, word[index]);
            if (dictionary[node].prefix_count >= 2) answer = index + 1;
        }
        cout << answer << '\n';
    }
}
