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
int rows[N];
int cols[N];
int diag1[2 * N - 1];
int diag2[2 * N - 1];
int fixedQueen[N];
int n;
vector<vector<int>> board;

int MAX_ITERATIONS;
int RUN;

int randomNumber(int n) {
    static std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(0, n);
    return dist(rng);
}

void reset() {
    for (int r = 0; r < n; r++) {
        for (int c : board[r]) {
            if (fixedQueen[r] == c) {
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

void initRandomBoard() {
    for (int r = 0; r < n; r++) {
        int num_queens = 2 - rows[r];
        for (int i = 0; i < num_queens; i++) {
            int c = -1;
            do {
                c = randomNumber(n - 1);
            } while (c == fixedQueen[r] || (board[r].size() == 1 && board[r].back() == c));
            board[r].push_back(c);
            rows[r]++;
            cols[c]++;
            diag1[r - c + n - 1]++;
            diag2[r + c]++;

        }
    }
}

pair<int,int> mostConflictPos() {
    int max_conflict = 0;
    int R = -1, C = -1;
    int count = 0;
    for (int r = 0; r < n; r++) {
        for (int c : board[r]) {
            if (rows[r] == 2 && cols[c] == 2 && diag1[r - c + n - 1] == 2 && diag2[r + c] == 2) {
                continue;
            }
            int num_conflict = rows[r] + cols[c] + diag1[r - c + n - 1] + diag2[r + c];
            if (num_conflict > max_conflict) {
                max_conflict = num_conflict;
                R = r;
                C = c;
                count = 1;
            } else if (num_conflict == max_conflict) {
                count++;
                if (randomNumber(count - 1) == 0) {
                    R = r;
                    C = c;
                }
            }
        }
    }
    return {R, C};
}

bool minConflict() {
    for (int iteration = 0; iteration < MAX_ITERATIONS; iteration++) {
        auto[R, C] = mostConflictPos();
        if (R == -1 && C == -1) {
            return true;
        }

        int min_conflict = 2 * N + 1;
        int min_conflict_col = -1;
        int count = 0;

        for (int c = 0; c < n; c++) {
            if (c == fixedQueen[R] || c == C) {
                continue;
            }
            int num_conflict = rows[R] + cols[c] + diag1[R - c + n - 1] + diag2[R + c];
            if (num_conflict < min_conflict) {
                min_conflict = num_conflict;
                min_conflict_col = c;
                count = 1;
            } else if (num_conflict == min_conflict) {
                count++;
                if (randomNumber(count - 1) == 0) {
                    min_conflict_col = c;
                }
            }
        }

        cols[C]--;
        diag1[R - C + n - 1]--;
        diag2[R + C]--;

        cols[min_conflict_col]++;
        diag1[R - min_conflict_col + n - 1]++;
        diag2[R + min_conflict_col]++;

        for (int i = 0; i < (int)board[R].size(); i++) {
            if (board[R][i] == C) {
                board[R][i] = min_conflict_col;
                break;
            }
        }
    }
    return false;
}

void solve() {
    while (cin >> n) {
        fill(rows, rows + N, 0);
        fill(cols, cols + N, 0);
        fill(diag1, diag1 + 2 * N - 1, 0);
        fill(diag2, diag2 + 2 * N - 1, 0);
        fill(fixedQueen, fixedQueen + N, -1);

        board.assign(n, vector<int>());

        int num_fixed_queen = 0;
        // for (int r = 0; r < n; r++) {
        //     int c;
        //     cin >> c;
        //     c--;
        //     if (c >= 0) {
        //         num_fixed_queen++;
        //         fixedQueen[r] = c;
        //         rows[r]++;
        //         cols[c]++;
        //         diag1[r - c + n - 1]++;
        //         diag2[r + c]++;
        //     }
        // }

        MAX_ITERATIONS = 100 * n;
        RUN = 1e8 / MAX_ITERATIONS;

        for (int run = 0; run < RUN; run++) {
            reset();
            initRandomBoard();
            if (minConflict()) {
                cout << "found\n";
                break;
            }
        }

        cout << 2 * n - num_fixed_queen << '\n';
        for (int r = 0; r < n; r++) {
            for (int c : board[r]) {
                cout << r + 1 << ' ' << c + 1 << '\n';
            }
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