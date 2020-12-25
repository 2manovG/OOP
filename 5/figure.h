#pragma once

#include <iostream>
#include <vector>
#include <cmath>

#define PI 3.141592653589793
#define mkPT(v, a, b) v.first = a; v.second = b

//pentagon
template<class T>
class Pentagon
{
public:
    using vertex = std::pair<T, T>;
    vertex v[5]; //array of vertecies

    Pentagon() : Pentagon(0, 0, 0, 0) {}
    Pentagon(T Ox, T Oy, T Ax, T Ay) //define pantagon by 2 points
    {
        mkPT(v[0], Ax, Ay);
        double OA = sqrt((Ax - Ox) * (Ax - Ox) + (Ay - Oy) * (Ay - Oy));
        double c = (Ax - Ox) / OA, s = (Ay - Oy) / OA;

        for (int i = 1; i < 5; i++)
        {
            double angle = 2 * i * PI / 5, ca = cos(angle), sa = sin(angle);
            mkPT(v[i], Ox + OA * (ca * c - sa * s), Oy + OA * (sa * c + ca * s));
        }
    }
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
std::ostream& operator<<(std::ostream &os, const Pentagon<T> &p)
{
    for (int i = 0; i < 5; i++)
    {
        os << p.v[i];
        if (i < 4) os << ", ";
    }
    return os;
}

//get area of pentagon
template<class T>
double area(Pentagon<T> p)
{
    double a2 = (p.v[0].first - p.v[1].first) * (p.v[0].first - p.v[1].first) +
        (p.v[0].second - p.v[1].second) * (p.v[0].second - p.v[1].second);
    return 1.25 * a2 * tan(PI * 0.3);
}