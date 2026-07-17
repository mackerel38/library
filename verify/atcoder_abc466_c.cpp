#include <bits/stdc++.h>

using namespace std;

int main() {
    int n;
    cin >> n;
    int left = 1;
    int right = 2;
    long long answer = 0;
    while (right <= n) {
        cout << "? " << left << ' ' << right << endl;
        string response;
        cin >> response;
        if (response == "Yes") {
            ++right;
        } else {
            answer += right - left - 1;
            ++left;
            if (left == right) ++right;
        }
    }
    while (left < n) answer += right - ++left;
    cout << "! " << answer << endl;
}
