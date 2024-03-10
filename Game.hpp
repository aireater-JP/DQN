#pragma once
#include <vector>

class Game
{
    using S = std::vector<std::vector<double>>;
    using size_t = std::size_t;
    // 盤面
    S board;

    std::vector<std::vector<bool>> can_set_f;
    std::vector<std::vector<S>> next_board;

    const int d_x[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    const int d_y[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

    int turn;

    int my_point, op_point;

public:
    const int END = -1;
    const int TURN_CHANGE = 0;
    const int PASS = -2;

    S get_board() { return board; }
    bool get_can_set_f(int x, int y) { return can_set_f[x][y]; }
    S get_next_board(int x, int y) { return next_board[x][y]; }

    double get_my() { return my_point; }
    double get_op() { return op_point; }

    int get_turn() { return turn; }

    void refresh()
    {
        board = std::vector<std::vector<double>>(8, std::vector<double>(8));
        board[3][3] = -1;
        board[3][4] = 1;
        board[4][3] = 1;
        board[4][4] = -1;

        turn = -1;

        turn_change();
    }

    int main(size_t x, size_t y)
    {

        board = next_board[x][y];

        if (my_point + op_point == 64)
        {
            return END;
        }

        if (turn_change() == false)
        {
            if (turn_change() == false)
            {
                return END;
            };
            return PASS;
        };

        return TURN_CHANGE;
    }

private:
    bool can_set(size_t x, size_t y)
    {
        S new_board = board;

        if (board[x][y] != 0)
        {
            return false;
        }

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

                if (board[new_x][new_y] == 0)
                {
                    break;
                }
            }
        }
        if (f == false)
        {
            return false;
        }
        next_board[x][y] = new_board;
        return true;
    }

    bool research()
    {
        my_point = 0;
        op_point = 0;

        // パスする
        bool f = false;

        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                bool pass = can_set_f[i][j] = can_set(i, j);
                // 置けるマスがあるのでパスしなくて済む
                if (pass == true)
                {
                    f = true;
                }

                if (board[i][j] == turn)
                {
                    my_point++;
                }
                if (board[i][j] == -turn)
                {
                    op_point++;
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
};