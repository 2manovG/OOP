#include "tupleandlist.h"

#define PI 3.14159265f

//trapezoid
template<class T>
class Trapezoid
{
public:
    inline const static std::string name = "Trapezoid";

    std::pair<T, T> nw, ne, sw, se;
    Trapezoid(T Ax, T Ay, T Bx, T By, T Cx, T Cy)
    {
        sw = mkPT(Ax, Ay);
        se = mkPT(Bx, By);
        nw = mkPT(Cx, Cy);

        auto AB = mkPT(Bx - Ax, By - Ay);
        auto AC = mkPT(Cx - Ax, Cy - Ay);
        T dot = AB.first * AC.first + AB.second * AC.second;
        T lenAB = sqrt(AB.first * AB.first + AB.second * AB.second);
        T lenCD = lenAB - 2 * dot / lenAB;

        ne.first = Cx + AB.first * lenCD / lenAB;
        ne.second = Cy + AB.second * lenCD / lenAB;

        if (lenCD < 0) std::swap(nw, ne);
    }
    Trapezoid(std::tuple<std::pair<T, T>, std::pair<T, T>, std::pair<T, T>> t) : 
        Trapezoid<T>(std::get<0>(t).first, std::get<0>(t).second, std::get<1>(t).first, std::get<1>(t).second,
            std::get<2>(t).first, std::get<2>(t).second) {}
};
//rhombus
template<class T>
class Rhombus
{
public:
    inline const static std::string name = "Rhombus";

    std::pair<T, T> l, u, r, d;
    Rhombus(T Ax, T Ay, T Bx, T By, T h)
    {
        l = mkPT(Ax, Ay);
        r = mkPT(Bx, By);

        auto O = mkPT((Ax + Bx) / 2, (Ay + By) / 2), AB = mkPT(Bx - Ax, By - Ay);
        T k = h / sqrt(AB.first * AB.first + AB.second * AB.second) / 2;

        u = mkPT(O.first + AB.second * k, O.second - AB.first * k);
        d = mkPT(O.first - AB.second * k, O.second + AB.first * k);
    }
    Rhombus(std::tuple<std::pair<T, T>, std::pair<T, T>, T> t) : 
        Rhombus<T>(std::get<0>(t).first, std::get<0>(t).second, std::get<1>(t).first, std::get<1>(t).second, std::get<2>(t)) {}
};
//pentagon
template<class T>
class Pentagon
{
public:
    inline const static std::string name = "Pentagon";

    std::pair<T, T> p[5];
    Pentagon(T Ox, T Oy, T Ax, T Ay)
    {
        p[0] = mkPT(Ax, Ay);
        T OA = sqrt((Ax - Ox) * (Ax - Ox) + (Ay - Oy) * (Ay - Oy));
        T c = (Ax - Ox) / OA, s = (Ay - Oy) / OA;

        for (int i = 1; i < 5; i++)
        {
            T angle = 2 * i * PI / 5, ca = cos(angle), sa = sin(angle);
            p[i] = mkPT(Ox + OA * (ca * c - sa * s), Oy + OA * (sa * c + ca * s));
        }
    }
    Pentagon(std::tuple<std::pair<T, T>, std::pair<T, T>> t) : 
        Pentagon<T>(std::get<0>(t).first, std::get<0>(t).second, std::get<1>(t).first, std::get<1>(t).second) {}
};

//area of tuple or figure
template<class T, class Obj>
T area(Obj &o)
{
    if (isTuple(o)) return tuple_area<T>(o);
    if (isClass<Trapezoid<T>>(o))
    {
        Trapezoid<T> *t = (Trapezoid<T>*)&o;
        return list_area<T>(t->sw, t->se, t->ne, t->nw);
    }
    if (isClass<Rhombus<T>>(o))
    {
        Rhombus<T> *t = (Rhombus<T>*)&o;
        return list_area<T>(t->l, t->u, t->r, t->d);
    }
    if (isClass<Pentagon<T>>(o))
    {
        Pentagon<T> *t = (Pentagon<T>*)&o;
        return list_area<T>(t->p[0], t->p[1], t->p[2], t->p[3], t->p[4]);
    }
    return -1;
}
//show tuple or figure
template<class T, class Obj>
void show(Obj &o)
{
    if (isTuple(o)) tuple_show<T>(o);
    else if (isClass<Trapezoid<T>>(o))
    {
        Trapezoid<T> *t = (Trapezoid<T>*)&o;
        list_show(t->sw, t->se, t->ne, t->nw);
    }
    else if (isClass<Rhombus<T>>(o))
    {
        Rhombus<T> *t = (Rhombus<T>*)&o;
        list_show(t->l, t->u, t->r, t->d);
    }
    else if (isClass<Pentagon<T>>(o))
    {
        Pentagon<T> *t = (Pentagon<T>*)&o;
        list_show(t->p[0], t->p[1], t->p[2], t->p[3], t->p[4]);
    }
}
//center of tuple or figure
template<class T, class Obj>
std::pair<T, T> center(Obj &o)
{
    if (isTuple(o)) return tuple_center<T>(o);
    if (isClass<Trapezoid<T>>(o))
    {
        Trapezoid<T> *t = (Trapezoid<T>*)&o;
        return list_center<T>(t->sw, t->se, t->ne, t->nw);
    }
    if (isClass<Rhombus<T>>(o))
    {
        Rhombus<T> *t = (Rhombus<T>*)&o;
        return list_center<T>(t->l, t->u, t->r, t->d);
    }
    if (isClass<Pentagon<T>>(o))
    {
        Pentagon<T> *t = (Pentagon<T>*)&o;
        return list_center<T>(t->p[0], t->p[1], t->p[2], t->p[3], t->p[4]);
    }
    return mkPT(0, 0);
}

//get areas of figures
template<class T>
class AreaVisitor
{
public:
    T operator()(Trapezoid<T> &o) const { return area<T>(o); }
    T operator()(Rhombus<T> &o) const { return area<T>(o); }
    T operator()(Pentagon<T> &o) const { return area<T>(o); }
};

//show verticies
template<class T>
class ShowVisitor
{
public:
    void operator()(Trapezoid<T> &o) const { show<T>(o); }
    void operator()(Rhombus<T> &o) const { show<T>(o); }
    void operator()(Pentagon<T> &o) const { show<T>(o); }
};

//center of figures
template<class T>
class CenterVisitor
{
public:
    std::pair<T, T> operator()(Trapezoid<T> &o) const { return center<T>(o); }
    std::pair<T, T> operator()(Rhombus<T> &o) const { return center<T>(o); }
    std::pair<T, T> operator()(Pentagon<T> &o) const { return center<T>(o); }
};

//names of figures
template<class T>
class NameVisitor
{
public:
    std::string operator()(Trapezoid<T> &o) const { return o.name; }
    std::string operator()(Rhombus<T> &o) const { return o.name; }
    std::string operator()(Pentagon<T> &o) const { return o.name; }
};