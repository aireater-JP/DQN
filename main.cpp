#include "NN/NN.hpp"

#include "Game.hpp"

#include "NN/io.hpp"

#include <deque>
#include <random>

struct exp_data
{
    vvvd s;
    vd t;
    int turn;
};

vvvd conversion(const vvd &s, int first);
pss choice(NN &nn, Game &game, Random<std::uniform_int_distribution<>> &R, Random<std::uniform_real_distribution<>> &E);

void DQN(int epoch)
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

    dqn.set_Loss(Identity_with_Loss());

    Game game;

    std::vector<exp_data> exp;

    Random<std::uniform_int_distribution<>> R(0, 0);
    Random<std::uniform_real_distribution<>> E(0, 1);

    for (int i = 0; i < epoch; i++)
    {
        double loss = 0;
        game.refresh();
        while (true)
        {
            exp.push_back({conversion(game.get_board(), game.get_turn()), {game.get_my(), game.get_op()}, game.get_turn()});
            pss a = choice(dqn, game, R, E);
            int status = game.main(a.first, a.second);

            if (status == game.END)
            {
                exp.push_back({conversion(game.get_board(), game.get_turn()), {game.get_my(), game.get_op()}, game.get_turn()});
                break;
            }
            if (status == game.PASS)
            {
                continue;
            }
            if (status == game.TURN_CHANGE)
            {
                continue;
            }
        }
        R.set(0, exp.size() - 2);
        for (int i = 0; i < exp.size() * 0.25; i++)
        {
            int r = R();
            loss += dqn.gradient(exp[r].s, exp[r + 1].t);
        }
        dqn.update(0.1 / (exp.size() * 0.25));

        out(game.get_my(), "/", game.get_op());
        newline();
        out("Loss=", loss / (exp.size() * 0.25));
        newline();
        exp.clear();
    }
}

pss choice(NN &nn, Game &game, Random<std::uniform_int_distribution<>> &R, Random<std::uniform_real_distribution<>> &E)
{
    // 確率で行動するZe
    if (E() < 0.3)
    {
        std::vector<pss> list;
        for (size_t i = 0; i < 8; ++i)
        {
            for (size_t j = 0; j < 8; ++j)
            {
                if (game.get_can_set_f(i, j) == false)
                {
                    continue;
                }
                list.push_back({i, j});
            }
        }
        R.set(0, list.size() - 1);
        return list[R()];
    }

    size_t x, y;
    double diff_max = -DBL_MAX;
    for (size_t i = 0; i < 8; ++i)
    {
        for (size_t j = 0; j < 8; ++j)
        {
            if (game.get_can_set_f(i, j) == false)
            {
                continue;
            }
            // 盤面データを3次元化
            vvvd input = conversion(game.get_next_board(i, j), game.get_turn());
            vd output = nn.predict(input)[0][0];

            double diff = output[0] - output[1];
            if (diff < diff_max)
            {
                continue;
            }
            diff_max = diff;
            x = i;
            y = j;
        }
    }
    return {x, y};
}

// 0がプレイヤー側
// 1が相手側
vvvd conversion(const vvd &s, int first)
{
    vvvd res(2, vvd(8, vd(8)));
    for (size_t i = 0; i < 8; ++i)
    {
        for (size_t j = 0; j < 8; ++j)
        {
            if (s[i][j] == 1)
            {
                res[0][i][j] = 1.0;
            }
            if (s[i][j] == -1)
            {
                res[1][i][j] = 1.0;
            }
        }
    }
    if (first == -1)
    {
        res[0].swap(res[1]);
    }
    return res;
}

int main()
{
    DQN(100);
}