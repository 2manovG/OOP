#pragma once

#include <iostream>
#include <vector>
#include <cmath>

#define PI 3.141592653589793
#define mkPT(v, a, b) v.first = a; v.second = b

//n-gon
template<class T>
class Ngon
{
public:
    using vertex = std::pair<T, T>;
    std::vector<vertex> v; //array of vertecies

    Ngon() : Ngon(0, 0, 0, 0, 1) {}
    Ngon(T Ox, T Oy, T Ax, T Ay, int N) //define n-gon by 2 points
    {
        v.push_back(std::pair(Ax,Ay));
        double OA = sqrt((Ax - Ox) * (Ax - Ox) + (Ay - Oy) * (Ay - Oy));
        double c = (Ax - Ox) / OA, s = (Ay - Oy) / OA;

        for (int i = 1; i < N; i++)
        {
            double angle = 2 * i * PI / N, ca = cos(angle), sa = sin(angle);
            v.push_back(std::pair(Ox + OA * (ca * c - sa * s), Oy + OA * (sa * c + ca * s)));
        }
    }
};
//pentagon
template<class T>
class Pentagon : public Ngon<T>
{
public:
    Pentagon() : Pentagon(0, 0, 0, 0, 5) {}
    Pentagon(T Ox, T Oy, T Ax, T Ay) : Ngon<T>(Ox, Oy, Ax, Ay, 5) {}
};
//hexagon
template<class T>
class Hexagon : public Ngon<T>
{
public:
    Hexagon() : Hexagon(0, 0, 0, 0, 6) {}
    Hexagon(T Ox, T Oy, T Ax, T Ay) : Ngon<T>(Ox, Oy, Ax, Ay, 6) {}
};
//octagon
template<class T>
class Octagon : public Ngon<T>
{
public:
    Octagon() : Octagon(0, 0, 0, 0, 8) {}
    Octagon(T Ox, T Oy, T Ax, T Ay) : Ngon<T>(Ox, Oy, Ax, Ay, 8) {}
};

//show vertex
template<class T>
std::ostream& operator<<(std::ostream &os, const std::pair<T, T> &p)
{
    os << "{ " << p.first << ", " << p.second << " }";
    return os;
}

//show vertecies of pentagon
template<class T>
std::ostream& operator<<(std::ostream &os, const Ngon<T> &p)
{
    for (int i = 0; i < p.v.size(); i++)
    {
        os << p.v[i];
        if (i < p.v.size()-1) os << ", ";
    }
    return os;
}

//get area of pentagon
template<class T>
double area(Ngon<T> p)
{
    double a2 = (p.v[0].first - p.v[1].first) * (p.v[0].first - p.v[1].first) +
        (p.v[0].second - p.v[1].second) * (p.v[0].second - p.v[1].second);
    return p.v.size() * a2 / (4 * tan(PI / p.v.size()));
}
template<class T>
std::pair<T, T> center(Ngon<T> p)
{
    std::pair<T, T> c = { 0, 0 };
    for (auto &v : p.v)
    {
        c.first += v.first;
        c.second += v.second;
    }
    return c;
}
template<class T>
std::string name(Ngon<T> p)
{
    switch(p.v.size())
    {
        case 5: return "Pentagon";
        case 6: return "Hexagon";
        case 8: return "Octagon";
    }
    throw std::logic_error("Unknow figure");
}