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
    int mm = m;
    vector<vector<int>> q(n, vector<int>(n));
    while (mm--) {
        int i, j, v;
        cin >> i >> j >> v;
        q[i][j] = v;
        q[j][i] = v;
    }

    int POPULATION_SIZE = 100;
    int ELITE_COUNT = 0.05 * POPULATION_SIZE;
    int MUTATION_BITS = 1;
    int TOURNAMENT_SIZE = 3;
    constexpr int N = 500;

    struct Candidate {
        bitset<N> x;
        ll score;
    };

    vector<Candidate> population(POPULATION_SIZE);

    auto evaluate = [&](const bitset<N>& x) -> ll {
        ll score = 0;
        for (int i = 0; i < n; i++) {
            if (!x[i]) {
                continue;
            }
            for (int j = 0; j < n; j++) {
                if (x[j]) {
                    score += q[i][j];
                }
            }
        }
        return score;
    };

    auto generateCandidates = [&]() {
        for (int p = 0; p < POPULATION_SIZE; p++) {
            Candidate candidate;
            for (int i = 0; i < n; i++) {
                candidate.x[i] = randomNumber(1);
            }
            candidate.score = evaluate(candidate.x);
            population[p] = candidate;
        }
    };

    auto selectCandidateIndex = [&]() {
        int best_index = randomNumber(POPULATION_SIZE - 1);
        for (int i = 1; i < TOURNAMENT_SIZE; i++) {
            int index = randomNumber(POPULATION_SIZE - 1);
            if (population[index].score > population[best_index].score) {
                best_index = index;
            }
        }
        return best_index;
    };

    auto crossOver = [&](const Candidate& a, const Candidate& b) -> Candidate {

    };

    auto mutate = [&](Candidate& candidate) {
        for (int i = 0; i < MUTATION_BITS; i++) {
            int j = randomNumber(n - 1);
            candidate.x.flip(j);
        }
    };

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