#pragma once

// E_f=false 確率で行動しない
// TYPE=0 白黒の個数
// TYPE=1 確率
// TYPE=2 ハイブリット

vvvd conversion(const vvd &s);

pss choice(NN &nn, Game &game, Random<std::uniform_int_distribution<>> &R, Random<std::uniform_real_distribution<>> &E, int TYPE, bool E_f)
{
    // 確率で行動するZe
    if (E_f == true and E() < 0.3)
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

    // 白黒の個数を予測して
    if (TYPE == 0)
    {
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
                vvvd input = conversion(game.get_next_board(i, j));
                vd output = nn.predict(input)[0][0];

                double diff;

                if (game.get_turn() == WHITE)
                {
                    diff = output[0] - output[1];
                }
                if (game.get_turn() == BLACK)
                {
                    diff = output[1] - output[0];
                }

                // diffがmaxより小さい時はcontinue
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

    // 予測値を参照
    if (TYPE == 1)
    {
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
                vvvd input = conversion(game.get_next_board(i, j));
                vd output = nn.predict(input)[0][0];

                // diffがmaxより小さい時はcontinue
                if (output[2] < diff_max)
                {
                    continue;
                }

                diff_max = output[2];
                x = i;
                y = j;
            }
        }
        return {x, y};
    }

    // ハイブリット
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
            vvvd input = conversion(game.get_next_board(i, j));
            vd output = nn.predict(input)[0][0];

            double diff;

            if (game.get_turn() == WHITE)
            {
                diff = output[0] - output[1];
            }
            if (game.get_turn() == BLACK)
            {
                diff = output[1] - output[0];
            }
            diff *= output[2];

            // diffがmaxより小さい時はcontinue
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

// 盤面を三次元化
vvvd conversion(const vvd &s)
{
    vvvd res(2, vvd(8, vd(8)));
    for (size_t i = 0; i < 8; ++i)
    {
        for (size_t j = 0; j < 8; ++j)
        {
            if (s[i][j] == WHITE)
            {
                res[0][i][j] = 1.0;
            }
            if (s[i][j] == BLACK)
            {
                res[1][i][j] = 1.0;
            }
        }
    }
    return res;
}