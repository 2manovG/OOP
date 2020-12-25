#include <type_traits> //std::is_same
#include <utility>     //std::pair
#include <vector>      //std::vector
#include <tuple>       //std::tuple
#include <iostream>    //for I/O
#include <cmath>       //sin cos sqrt

#define mkP(a, b) std::make_pair(a, b)
#define mkPT(a, b) std::make_pair<T, T>(std::move(a), std::move(b))

//check for tuple
template<class T> bool isTuple(T t) { return false; }
template<class... List> bool isTuple(std::tuple<List...> t) { return true; }

//check for being a class T
template<class T, class U> bool isClass(U u) { return std::is_same<T, U>::value; }

//print pair
template<class T, class U>
std::ostream& operator<<(std::ostream& os, std::pair<T, U> p)
{
    os << "(" << p.first << ", " << p.second << ")";
    return os;
}

//tuple to vector
template<class T>
std::vector<T> tuple2vector(std::tuple<> t)
{
    std::vector<T> v;
    return v;
}
template<class T, class... List>
std::vector<T> tuple2vector(std::tuple<T, List...> t)
{
    std::vector<T> v = tuple2vector<T>(*((std::tuple<List...>*)&t));
    v.insert(v.begin(), std::get<0>(t));
    return v;
}

//list to vector
template<class T>
std::vector<T> list2vector()
{
    return std::vector<T>();
}
template<class T, class... List>
std::vector<T> list2vector(T h, List... l)
{
    std::vector<T> v = list2vector<T>(l...);
    v.insert(v.begin(), h);
    return v;
}

//areas
//area of vector of points
template<class T>
T vector_area(std::vector<std::pair<T, T>> points)
{
    auto dist = [](std::pair<T, T> p1, std::pair<T, T> p2) //distance between two points
    {
        return sqrt((p1.first - p2.first) * (p1.first - p2.first) + (p1.second - p2.second) * (p1.second - p2.second));
    };
    auto geron = [dist](std::pair<T, T> p1, std::pair<T, T> p2, std::pair<T, T> p3) //area of triangle
    {
        float a = dist(p1, p2), b = dist(p2, p3), c = dist(p1, p3), p = (a + b + c) / 2;
        return sqrt(p * (p - a) * (p - b) * (p - c));
    };

    float area = 0;
    for (int i = 1; i < points.size() - 1; i++) area += geron(points[0], points[i], points[i + 1]);
    return area;
}

//area of tuple
template<class T, class... List> T tuple_area(std::tuple<List...> t) { return vector_area<T>(tuple2vector(t)); }
template<class T, class Obj> T tuple_area(Obj o) { return -1; } //area of non-tuple

//area of list of params
template<class T, class... List> T list_area(List... l) { return vector_area<T>(list2vector<std::pair<T, T>>(l...)); }

//show
//show tuple
void tuple_show(std::tuple<> t) {}
template<class T> void tuple_show(std::tuple<T> t)
{
    std::cout << std::get<0>(t) << std::endl;
}
template<class T, class U, class... List> void tuple_show(std::tuple<T, U, List...> t)
{
    std::cout << std::get<0>(t) << ", ";
    tuple_show(*((std::tuple<U, List...>*)&t));
}
template<class T, class Obj> T tuple_show(Obj o) { } //non-tuple

//show list of params
template<class T> void list_show(T h) { std::cout << h << std::endl; }
template<class T, class U, class... List> void list_show(T h, U h1, List... l) { std::cout << h << ", "; list_show(h1, l...); }

//center
//center of tuple
template<class T>
std::pair<T, T> vector_center(std::vector<std::pair<T, T>> points)
{
    T avg1 = 0, avg2 = 0;
    for (auto p : points)
    {
        avg1 += p.first;
        avg2 += p.second;
    }
    return mkPT(avg1 / points.size(), avg2 / points.size());
}

//center of tuple
template<class T, class... List> std::pair<T, T> tuple_center(std::tuple<List...> t) { return vector_center<T>(tuple2vector(t)); }
template<class T, class Obj> std::pair<T, T> tuple_center(Obj o) { return mkPT(0, 0); } //area of non-tuple

//center of list of params
template<class T, class... List> std::pair<T, T> list_center(List... l) { return vector_center<T>(list2vector<std::pair<T, T>>(l...)); }