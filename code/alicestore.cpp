#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <map>
#include <set>
#include <queue>
#include <stack>
#include <deque>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <tuple>
#include <numeric>
#include <functional>
#include <iomanip>
#include <cassert>
#include <bitset>
#include <climits>
using namespace std;

#define fastio ios::sync_with_stdio(false); cin.tie(nullptr)
using ll = long long;
constexpr bool has_mutiple_tests = false;

void solve() {
    int n;
    cin >> n;
    vector<int> a(n), b(n);
    for (int& i : a) {
        cin >> i;
    }
    for (int& i : b) {
        cin >> i;
    }
    vector<bool> x(n);
    priority_queue<pair<int,int>> pq;
    ll total = 0;
    int cnt = 0;
    for (int i = 0; i < n; i++) {
        total += a[i];
        if (total >= b[i]) {
            total -= b[i];
            x[i] = true;
            pq.push({b[i], i});
            cnt++;
        } else if (!pq.empty()) {
            auto[B, j] = pq.top();
            if (B > b[i]) {
                total = total + B - b[i];
                x[j] = false;
                pq.pop();
                x[i] = true;
                pq.push({b[i], i});
            }
        }
    }
    cout << cnt << '\n';
    for (int i = 0; i < n; i++) {
        if (x[i]) {
            cout << i + 1 << ' ';
        }
    }
}

int main() {
    fastio;
    int t = 1;
    if (has_mutiple_tests)
        cin >> t;
    while (t--)
        solve();
    return 0;
}