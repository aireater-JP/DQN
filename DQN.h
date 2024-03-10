#pragma once

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