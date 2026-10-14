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

void print(const vector<ll>& p) {
    for (int i : p) {
        cout << i << ' ';
    }
    cout << '\n';
}

void solve() {
    int n, s;
    cin >> n >> s;
    vector<ll> p(n);
    for (ll& i : p) {
        cin >> i;
    }
    for (int i = 0; i < n; i++) {
        p[i] -= i * s;
    }
    ll total = 0;
    ll INF = LLONG_MAX;
    priority_queue<ll,vector<ll>,greater<ll>> sold;
    priority_queue<ll,vector<ll>,greater<ll>> bought;
    bought.push(p[0]);
    for (int i = 1; i < n; i++) {
        ll minSold = (sold.empty() ? INF : sold.top());
        ll minBought = (bought.empty() ? INF : bought.top());
        if (p[i] <= minSold && p[i] <= minBought) {
            bought.push(p[i]);
        } else {
            if (minSold <= minBought) {
                total += p[i] - minSold;
                sold.pop();
                sold.push(p[i]);
                bought.push(minSold);
            } else {
                total += p[i] - minBought;
                bought.pop();
                sold.push(p[i]);
            }
        }
    }
    // print(p);
    cout << total;
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