#pragma once

#include "Layer.hpp"
#include "Loss.hpp"

#include <memory>
#include <vector>

class NN
{
    std::vector<std::unique_ptr<Layer>> layer;
    std::unique_ptr<Loss> m_loss;

    std::tuple<std::size_t, std::size_t, std::size_t> output;

public:
    template <class T>
    void add_Layer(T &&l)
    {
        layer.emplace_back(std::make_unique<T>(std::move(l)));
        if (layer.size() != 1)
        {
            layer.back()->set(output);
        }
        output = layer.back()->get();
    }

    template <class T>
    void set_Loss(T &&l)
    {
        m_loss = std::make_unique<T>(std::move(l));
    }

    vvvd predict(const vvvd &x)
    {
        vvvd y = x;
        for (size_t i = 0; i < layer.size(); ++i)
        {
            y = layer[i]->forward(y);
        }
        return y;
    }

    double loss(const vvvd &x, const vd &t)
    {
        return m_loss->forward(predict(x), t);
    }

    double gradient(const vvvd &x, const vd &t)
    {
        double y = loss(x, t);
        vvvd d = m_loss->backward();

        for (size_t i = layer.size() - 1; i < layer.size(); i--)
        {
            d = layer[i]->backward(d);
        }
        return y;
    }

    void update(const double lr)
    {
        for (size_t i = 0; i < layer.size(); ++i)
        {
            layer[i]->update(lr);
        }
    }
};