#include <bits/stdc++.h>
#include "string/rotation.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string text;
    cin >> text;
    cout << rotated(text, minimum_rotation(text)) << '\n';
    cout << rotated(text, maximum_rotation(text)) << '\n';
}
