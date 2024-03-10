#pragma once
#include <vector>
#include <map>

const int END = -1;
const int TURN_CHANGE = 0;
const int PASS = -2;

const int BLACK = 1;
const int WHITE = -1;

class Game
{
    // 定義系
    using S = std::vector<std::vector<double>>;
    using size_t = std::size_t;

    const int d_x[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    const int d_y[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

    // 盤面
    S board;

    std::vector<std::vector<bool>> can_set_f;
    std::vector<std::vector<S>> next_board;

    int turn;

    std::map<int, int> point;

    int winner;

public:
    S get_board() { return board; }
    bool get_can_set_f(int x, int y) { return can_set_f[x][y]; }
    S get_next_board(int x, int y) { return next_board[x][y]; }

    double get_point(int c) { return point[c]; }

    int get_turn() { return turn; }

    void refresh()
    {
        board = S(8, std::vector<double>(8));
        board[3][3] = -1;
        board[3][4] = 1;
        board[4][3] = 1;
        board[4][4] = -1;

        turn = WHITE;

        turn_change();
    }

    int main(size_t x, size_t y)
    {

        board = next_board[x][y];

        // コマを置けない
        if (turn_change() == false)
        {
            // もう一回おけない
            if (turn_change() == false)
            {
                // 終了
                set_winner();
                return END;
            };
            // 置けるからパス
            return PASS;
        };

        return TURN_CHANGE;
    }

private:
    // コマを置けるかどうか調べる
    bool can_set(size_t x, size_t y)
    {
        S new_board = board;

        // 置いてあるなら終了
        if (board[x][y] != 0)
        {
            return false;
        }

        // コマを置けたかどうか
        bool f = false;

        for (int i = 0; i < 8; i++)
        {
            // 各方向
            size_t new_x = x + d_x[i], new_y = y + d_y[i];

            // 範囲外アクセス禁止法
            if (new_x >= 8 or new_y >= 8)
            {
                continue;
            }

            // 隣が置く色と反対の色かどうか
            if (board[new_x][new_y] != -turn)
            {
                continue;
            }

            for (int j = 2; j < 8; j++)
            {
                new_x = x + d_x[i] * j;
                new_y = y + d_y[i] * j;

                // 範囲外アクセス禁止法
                if (new_x >= 8 or new_y >= 8)
                {
                    break;
                }

                // 同じ色ならひっくり返す
                if (board[new_x][new_y] == turn)
                {
                    f = true;

                    for (int k = 0; k < j; k++)
                    {
                        new_x = x + d_x[i] * k;
                        new_y = y + d_y[i] * k;
                        new_board[new_x][new_y] = turn;
                    }
                    break;
                }
                // 違うなら続ける
                if (board[new_x][new_y] == -turn)
                {
                    continue;
                }

                // 何も置かれてなければ終了
                if (board[new_x][new_y] == 0)
                {
                    break;
                }
            }
        }
        // 何も置けませんでした!!!
        if (f == false)
        {
            return false;
        }

        // logを保存して終了
        next_board[x][y] = new_board;
        return true;
    }

    // 置き終わったあとの初期化
    bool research()
    {
        point[BLACK] = 0;
        point[WHITE] = 0;

        // 置く場所があるかどうか
        bool f = false;

        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                can_set_f[i][j] = can_set(i, j);
                // 置けるマスがある
                if (can_set_f[i][j] == true)
                {
                    f = true;
                }

                // ついでにポイントを計算
                if (board[i][j] == BLACK)
                {
                    point[BLACK]++;
                }
                if (board[i][j] == WHITE)
                {
                    point[WHITE]++;
                }
            }
        }
        return f;
    }

    bool turn_change()
    {
        turn *= -1;
        can_set_f = std::vector<std::vector<bool>>(8, std::vector<bool>(8));
        next_board = std::vector<std::vector<S>>(8, std::vector<S>(8));
        return research();
    }

    void set_winner()
    {
        if (point[WHITE] == point[BLACK])
        {
            winner = 0;
        }
        if (point[WHITE] > point[BLACK])
        {
            winner = WHITE;
        }
        if (point[WHITE] < point[BLACK])
        {
            winner = BLACK;
        }
    }
};