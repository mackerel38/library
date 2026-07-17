#define PROBLEM "https://judge.yosupo.jp/problem/suffixarray"

#include "string/suffixarray.hpp"

using namespace std;
using namespace poe;

int main() {
    string text;
    cin >> text;
    const auto suffixes = suffix_array(text);
    for (size_t i = 0; i < suffixes.size(); ++i) {
        cout << suffixes[i] << " \n"[i + 1 == suffixes.size()];
    }
}
