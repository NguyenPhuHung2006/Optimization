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
#include <random>
#include <numbers>
using namespace std;

#define fastio ios::sync_with_stdio(false); cin.tie(nullptr)
using ll = long long;
constexpr bool has_mutiple_tests = false;

int randomNumber(int n) {
    static std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(0, n);
    return dist(rng);
}

void solve() {
    int n, m;
    cin >> n >> m;
    vector<vector<int>> q(n, vector<int>(n));
    int mm = m;
    while (mm--) {
        int r, c, v;
        cin >> r >> c >> v;
        q[r][c] = v;
        q[c][r] = v;
    }

    int ITER_PER_RUN = 100 * n;
    int RUN = max(1, (int)1e5 / ITER_PER_RUN);
    int MAX_TELEPORT = (int)sqrt(ITER_PER_RUN);
    int MAX_FLIP = n / 4;
    constexpr int N = 500;

    bitset<N> cur_x;
    bitset<N> best_x;
    ll init_score;
    ll cur_score;
    ll INF = LLONG_MAX;
    ll best_score = -INF;
    int K = 4;
    vector<ll> delta(n);

    auto initializeRandomX = [&]() {
        for (int i = 0; i < n; i++) {
            cur_x[i] = randomNumber(1);
        }
        init_score = 0;
        delta.assign(n, 0LL);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                init_score += q[i][j] * cur_x[i] * cur_x[j];
                delta[i] += (i == j ? 1LL : 2LL) * q[i][j] * cur_x[j];
            }
            if (cur_x[i] == 1) {
                delta[i] *= -1;
            }
        }
        cur_score = init_score;
        if (cur_score > best_score) {
            best_score = cur_score;
            best_x = cur_x;
        }
    };

    auto getIndex = [&]() -> int {
        // int best_i = -1;
        // ll best_delta = -INF;
        // int count = 0;
        // for (int i = 0; i < n; i++) {
        //     if (delta[i] > best_delta) {
        //         best_delta = delta[i];
        //         best_i = i;
        //         count = 1;
        //     } else if (delta[i] == best_delta) {
        //         count++;
        //         if (randomNumber(count - 1) == 0) {
        //             best_i = i;
        //         }
        //     }
        // }
        // return best_i;

        priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>> pq;

        for (int i = 0; i < n; i++) {
            pq.push({delta[i], i});

            if ((int)pq.size() > K) {
                pq.pop();
            }
        }
        int k = randomNumber(K - 1);
        while (k--) {
            pq.pop();
        }
        return pq.top().second;
    };

    auto update = [&](int i) {
        cur_score += delta[i];
        for (int j = 0; j < n; j++) {
            if (i == j) {
                delta[j] *= -1;
            } else {
                delta[j] += 2LL * (cur_x[i] ^ cur_x[j] ? -1 : 1) * q[i][j];
            }
        }
        cur_x.flip(i);
        if (cur_score > best_score) {
            best_score = cur_score;
            best_x = cur_x;
        }
    };

    auto search = [&]() {
        int num_teleport = 1 + randomNumber(MAX_TELEPORT - 1);
        for (int iteration = 0; iteration < ITER_PER_RUN; iteration++) {
            int i = getIndex();
            if (delta[i] > 0) {
                update(i);
            } else {
                if (num_teleport == 0) {
                    break;
                }
                num_teleport--;
                int num_flip = 1 + randomNumber(MAX_FLIP - 1);
                for (int flip = 0; flip < num_flip; flip++) {
                    int j = randomNumber(n - 1);
                    update(j);
                }
            }
        }
    };

    for (int run = 0; run < RUN; run++) {
        initializeRandomX();
        search();
    }
    for (int i = 0; i < n; i++) {
        cout << best_x[i] << ' ';
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