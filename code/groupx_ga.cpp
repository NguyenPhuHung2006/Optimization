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

int n, m;
vector<vector<int>> q;

int POPULATION_SIZE;
int ELITE_COUNT;
int MUTATION_BITS;
int TOURNAMENT_SIZE;
int NUM_GENERATION;
constexpr int N = 500;

void solve() {
    cin >> n >> m;
    int mm = m;
    q.assign(n, vector<int>(n));
    while (mm--) {
        int i, j, v;
        cin >> i >> j >> v;
        q[i][j] = v;
        q[j][i] = v;
    }

    POPULATION_SIZE = 100;
    ELITE_COUNT = 0.05 * POPULATION_SIZE;
    MUTATION_BITS = 1;
    TOURNAMENT_SIZE = 3;
    NUM_GENERATION = 1000;

    struct Candidate {
        bitset<N> x;
        ll score;
        vector<ll> delta;

        void setRandomX() {
            for (int i = 0; i < n; i++) {
                x[i] = randomNumber(1);
            }
        } 

        // this function should only be called when setRandomX is called
        void init() {
            delta.assign(n, 0LL);
            score = 0LL;
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    score += q[i][j] * x[i] * x[j];
                    delta[i] += (i == j ? 1LL : 2LL) * q[i][j] * x[j];
                }
                if (x[i] == 1) {
                    delta[i] *= -1;
                }
            }
        }

        void flip(int i) {
            score += delta[i];
            for (int j = 0; j < n; j++) {
                if (i == j) {
                    delta[i] *= -1;
                } else {
                    delta[i] += 2LL * (x[i] ^ x[j] ? -1 : 1) * q[i][j];
                }
            }
            x.flip(i);
        }

        void mutate() {
            for (int i = 0; i < MUTATION_BITS; i++) {
                int j = randomNumber(n - 1);
                flip(j);
            }
        }

        Candidate crossOver(const Candidate& other) {

        }

        void search() {

        }
    };

    vector<Candidate> population(POPULATION_SIZE);
    bitset<N> best_x;
    ll INF = LLONG_MAX;
    ll best_score = -INF;

    auto generateCandidates = [&]() {
        for (int p = 0; p < POPULATION_SIZE; p++) {
            Candidate candidate;
            candidate.setRandomX();
            candidate.init();
            population[p] = candidate;
            if (candidate.score > best_score) {
                best_score = candidate.score;
                best_x = candidate.x;
            }
        }
    };

    auto getRandomCandidateIndex = [&]() {
        int best_index = randomNumber(POPULATION_SIZE - 1);
        for (int i = 1; i < TOURNAMENT_SIZE; i++) {
            int index = randomNumber(POPULATION_SIZE - 1);
            if (population[index].score > population[best_index].score) {
                best_index = index;
            }
        }
        return best_index;
    };

    auto sortPopulation = [&]() {
        partial_sort(
            population.begin(),
            population.begin() + ELITE_COUNT,
            population.end(),
            [](const Candidate& a, const Candidate& b) {
                return a.score > b.score;
            }
        );
    };

    auto generating = [&]() {
        vector<Candidate> new_population;
        for (int generation = 0; generation < NUM_GENERATION; generation++) {
            sortPopulation();
            new_population.clear();
            for (int elite = 0; elite < ELITE_COUNT; elite++) {
                new_population.push_back(population[elite]);
            }
            while ((int)new_population.size() < POPULATION_SIZE) {
                Candidate a = population[getRandomCandidateIndex()];
                Candidate b = population[getRandomCandidateIndex()];
                Candidate child = a.crossOver(b);
                child.mutate();
                child.search();
                new_population.push_back(child);
                if (child.score > best_score) {
                    best_score = child.score;
                    best_x = child.x;
                }
            }
            population = new_population;
        }
    };

    generateCandidates();
    generating();

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