#include <bits/stdc++.h>

using namespace std;

int main() {
    string source;
    cin >> source;
    int a = 0, b = 0, c = 0;
    for (char character : source) {
        if (character == 'A') ++a;
        if (character == 'B') b = min(a, b + 1);
        if (character == 'C') c = min(b, c + 1);
    }
    cout << c << '\n';
}
