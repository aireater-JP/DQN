#include "NN/NN.hpp"

#include "Game.hpp"

#include "NN/out.hpp"

#include "DQN.h"

#include <deque>
#include <random>

struct play_log
{
    vvvd s;
    std::map<int, int> point;
    int turn;
};

void DQN(int epoch, double lr)
{
    NN dqn;
    dqn.add_Layer(Conv2d(32, {3, 3}, {1, 1}, {-1, -1}, {2, 8, 8})); // 32*8*8
    dqn.add_Layer(ReLU());

    dqn.add_Layer(flatten()); // 2048

    dqn.add_Layer(Dense(256, He)); // 256
    dqn.add_Layer(ReLU());

    dqn.add_Layer(Dense(32, He)); // 32
    dqn.add_Layer(ReLU());

    dqn.add_Layer(Dense(3, He));
    // 0 白
    // 1 黒
    // 確率

    dqn.set_Loss(Identity_with_Loss());

    Game game;

    std::vector<play_log> exp;

    Random<std::uniform_int_distribution<>> R(0, 0);
    Random<std::uniform_real_distribution<>> E(0, 1);

    for (int i = 0; i < epoch; i++)
    {
        game.refresh();
        while (true)
        {
            exp.push_back({conversion(game.get_board()), game.get_point(), game.get_turn()});

            // 行動選択アルゴリズムはよく考える必要あり
            pss a = choice(dqn, game, R, E, 2, true);

            // ゲーム終了
            if (game.main(a.first, a.second) == END)
            {
                exp.push_back({conversion(game.get_board()), game.get_point(), game.get_turn()});
                break;
            }
        }

        double loss = 0;

        // 1ゲームで1/4回選んで学習
        int batch = exp.size() * 0.25;

        R.set(0, exp.size() - 2);

        int winner = game.get_winner();

        for (int i = 0; i < batch; i++)
        {
            int r = R();
            vd t = {double(exp[r + 1].point[WHITE]), double(exp[r + 1].point[BLACK]), 0.5};

            // 勝者なら1
            // 敗者なら0
            // 引き分けなら0.5
            if (winner == exp[r].turn)
            {
                t[2] = 1;
            }
            if (winner == -exp[r].turn)
            {
                t[2] = 0;
            }

            loss += dqn.gradient(exp[r].s, t);
        }
        dqn.update(lr / batch);

        out("白 : 黒 =", game.get_point()[WHITE], ":", game.get_point()[BLACK], "\n");
        out("Loss=", loss / batch);
        newline();

        exp.clear();
    }
}

int main()
{
    DQN(100, 0.05);
}