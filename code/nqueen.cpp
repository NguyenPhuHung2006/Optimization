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

constexpr int N = 500;
int n;

bool rows[N];
int cols[N];
int diag1[2 * N - 1]; // left to right (r - c + (n - 1) = const)
int diag2[2 * N - 1]; // right to left (r + c = const)
int board[N];

int MAX_ITERATIONS;
int RUN;

void initialize_random_board() {
    for (int r = 0; r < n; r++) {
        if (!rows[r]) {
            int prevCol = board[r];
            if (prevCol >= 0) {
                cols[prevCol]--;
                diag1[r - prevCol + n - 1]--;
                diag2[r + prevCol]--;
            }
            int c = randomNumber(n - 1);
            cols[c]++;
            diag1[r - c + n - 1]++;
            diag2[r + c]++;
            board[r] = c;
        }
    }
}

int get_most_conflict_row() {
    int maxConflict = -1;
    int mostConflictRow = -1;
    int count = 0;

    for (int r = 0; r < n; r++) {
        int c = board[r];
        if (rows[r] || (cols[c] == 1 && diag1[r - c + n - 1] == 1 && diag2[r + c] == 1)) {
            continue;
        }
        int numConflict = cols[c] + diag1[r - c + n - 1] + diag2[r + c];
        if (numConflict > maxConflict) {
            maxConflict = numConflict;
            mostConflictRow = r;
            count = 1;
        } else if (numConflict == maxConflict) {
            count++;
            if (randomNumber(count - 1) == 0) {
                mostConflictRow = r;
            }
        }
    }
    return mostConflictRow;
}

bool min_conflicts() {
    for (int iteration = 0; iteration < MAX_ITERATIONS; iteration++) {
        int r = get_most_conflict_row();
        if (r == -1) {
            return true;
        }

        int minConflict = N + 1;
        int leastConflictCol = -1;
        int count = 0;

        for (int c = 0; c < n; c++) {
            if (c == board[r]) {
                continue;
            }
            int numConflict = cols[c] + diag1[r - c + n - 1] + diag2[r + c];
            if (numConflict < minConflict) {
                minConflict = numConflict;
                leastConflictCol = c;
                count = 1;
            } else if (numConflict == minConflict) {
                count++;
                if (randomNumber(count - 1) == 0) {
                    leastConflictCol = c;
                }
            }
        }

        int prevCol = board[r];
        cols[prevCol]--;
        diag1[r - prevCol + n - 1]--;
        diag2[r + prevCol]--;

        cols[leastConflictCol]++;
        diag1[r - leastConflictCol + n - 1]++;
        diag2[r + leastConflictCol]++;
        board[r] = leastConflictCol;
    }
    return false;
}

void solve() {
    while (cin >> n) {

        fill(rows, rows + N, false);
        fill(cols, cols + N, 0);
        fill(diag1, diag1 + 2 * N - 1, 0);
        fill(diag2, diag2 + 2 * N - 1, 0);
        fill(board, board + N, -1);

        MAX_ITERATIONS = 100 * n;
        RUN = 1e5 / MAX_ITERATIONS;
        
        for (int r = 0; r < n; r++) {
            int c;
            cin >> c;
            c--;
            if (c >= 0) {
                rows[r] = true;
                cols[c]++;
                diag1[r - c + n - 1]++;
                diag2[r + c]++;
                board[r] = c;
            }
        }

        bool found = false;

        for (int run = 0; run < RUN; run++) {
            initialize_random_board();

            if (min_conflicts()) {
                found = true;
                break;
            }
        }

        // cout << (found ? "Success" : "Fail") << ' ';

        for (int r = 0; r < n; r++) {
            cout << board[r] + 1 << ' ';
        }
        cout << endl;
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