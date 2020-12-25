#pragma once

#include <memory>
#include "figure.h"

//figure factory
template<class T>
class Factory
{
public:
    static std::shared_ptr<Ngon<T>> Generate(int n, T ox, T oy, T ax, T ay)
    {
        switch (n)
        {
        case 5: //pentagon
            return std::shared_ptr<Ngon<T>>(new Pentagon<T>(ox, oy, ax, ay));
        case 6: //hexagon
            return std::shared_ptr<Ngon<T>>(new Hexagon<T>(ox, oy, ax, ay));
        case 8: //octagon
            return std::shared_ptr<Ngon<T>>(new Octagon<T>(ox, oy, ax, ay));
        }
        throw std::logic_error("Unknow figure");
    }
};