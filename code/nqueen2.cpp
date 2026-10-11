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

constexpr int N = 500;
int n;
int rows[N];
int cols[N];
int diag1[2 * N - 1];
int diag2[2 * N - 1];
int fixedQueen[N];
int MAX_ITERATIONS;
int RUN;
int best_count;
int fixed_count;

vector<vector<int>> board;
vector<vector<int>> best_board;

int randomNumber(int n) {
    static std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(0, n);
    return dist(rng);
}

void reset() {
    for (int r = 0; r < n; r++) {
        for (int c : board[r]) {
            if (c == fixedQueen[r]) {
                continue;
            }
            rows[r]--;
            cols[c]--;
            diag1[r - c + n - 1]--;
            diag2[r + c]--;
        }
    }
    board.assign(n, vector<int>());
}

bool isValidPosition(int r, int c) {
    if (c == fixedQueen[r]) {
        return false;
    }

    for (int j : board[r]) {
        if (j == c) {
            return false;
        }
    }

    if (cols[c] >= 2 ||
        diag1[r - c + n - 1] >= 2 ||
        diag2[r + c] >= 2) {
        return false;
    }

    return true;
}

pair<int, int> getLeastConflictPos() {
    int R = -1;
    int count = 0;

    for (int r = 0; r < n; r++) {
        if (rows[r] >= 2) {
            continue;
        }
        count++;
        if (randomNumber(count - 1) == 0) {
            R = r;
        }
    }

    if (R == -1) {
        return {-1, -1};
    }

    int min_conflict = INT_MAX;
    int C = -1;
    count = 0;

    for (int c = 0; c < n; c++) {
        if (!isValidPosition(R, c)) {
            continue;
        }

        int conflicts = rows[R] + cols[c] + diag1[R - c + n - 1] + diag2[R + c];

        if (conflicts < min_conflict) {
            min_conflict = conflicts;
            C = c;
            count = 1;
        } else if (conflicts == min_conflict) {
            count++;
            if (randomNumber(count - 1) == 0) {
                C = c;
            }
        }
    }
    return {R, C};
}


void placeQueen(int r, int c) {
    board[r].push_back(c);
    rows[r]++;
    cols[c]++;
    diag1[r - c + n - 1]++;
    diag2[r + c]++;
}

int search() {
    int count_queens = 0;
    int failed = 0;
    for (int iteration = 0; iteration < MAX_ITERATIONS; iteration++) {
        auto[R, C] = getLeastConflictPos();
        if (R == -1 || C == -1) {
            failed++;
            if (failed >= n) {
                break;
            }
            continue;
        }
        placeQueen(R, C);
        count_queens++;
        failed = 0;
    }
    return count_queens;
}

void getInput() {
    for (int r = 0; r < n; r++) {
        int c;
        cin >> c;
        c--;
        if (c >= 0) {
            fixed_count++;
            fixedQueen[r] = c;
            rows[r]++;
            cols[c]++;
            diag1[r - c + n - 1]++;
            diag2[r + c]++;
        }
    }
}

void checkOutput() {
    bool valid = true;
    for (int r = 0; r < n; r++) {
        for (int c = 0; c < n; c++) {
            if (rows[r] > 2 || cols[c] > 2 || diag1[r - c + n - 1] > 2 || diag2[r + c] > 2) {
                valid = false;
                break;
            }
        }
    }
    cout << (valid ? "true" : "false") << ' ' << best_count << endl; 
}

void solve() {
    while (cin >> n) {
        fill(rows, rows + N, 0);
        fill(cols, cols + N, 0);
        fill(diag1, diag1 + 2 * N - 1, 0);
        fill(diag2, diag2 + 2 * N - 1, 0);
        fill(fixedQueen, fixedQueen + N, -1);
        board.assign(n, vector<int>());
        best_board.assign(n, vector<int>());
        best_count = 0;
        fixed_count = 0;

        getInput();

        MAX_ITERATIONS = 10 * n;
        RUN = max(2, (int)3e4 / MAX_ITERATIONS);

        for (int run = 0; run < RUN; run++) {
            reset();
            int cur_count = search();
            if (cur_count > best_count) {
                best_count = cur_count;
                best_board = board;
            }
            if (best_count + fixed_count == 2 * n) {
                break;
            }
        }
        cout << best_count << '\n';
        for (int r = 0; r < n; r++) {
            for (int c : best_board[r]) {
                cout << r + 1 << ' ' << c + 1 << '\n';
            }
        }
        // cout << 'a' << endl;
        // checkOutput();
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