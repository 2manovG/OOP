/*
* Туманов Георгий М8О-201Б
* ========================
* Вариант 4:
* Трапеция
* Ромб
* 5-угольник
*/

#include <iostream> //for I/O
#include <sstream> //for std::stringstream
#include <vector> //for std::vector
#include <cmath> //for sqrtf, sinf, cosf

#define PI 3.14159265f

struct Point //pair of x,y
{
    float x, y;
    Point(float x_, float y_) : x(x_), y(y_) {}
    void set(float x_, float y_) { x = x_; y = y_; }
    void add(float x_, float y_) { x += x_; y += y_; }
};
std::ostream& operator<< (std::ostream& os, Point p)
{
    os << "{ " << p.x << ", " << p.y << " }";
    return os;
}

class Figure //parent class of all convex shapes
{
public:
    Figure(std::string name = "figure") : mName(name) //set the name clears the array of verticies
    {
        mVertices.clear(); 
    }
    
    Point getMiddle() //returns the average point
    {
        Point p(0, 0);
        for (Point q : mVertices) p.add(q.x, q.y);
        p.x /= mVertices.size();
        p.y /= mVertices.size();
        return p;
    }

    float getArea() //returns the sum of areas of triangles
    {
        float area = 0;
        for (int i = 1; i < mVertices.size() - 1; i++) area += geron(mVertices[0], mVertices[i], mVertices[i + 1]);
        return area;
    }

    std::string getName()
    {
        return mName;
    }

protected:
    std::vector<Point> mVertices; //array of verticies
    std::string mName; //name of figure

private:
    float dist(Point p1, Point p2) //distance between two points
    {
        return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
    }
    float geron(Point p1, Point p2, Point p3) //area of triangle
    {
        float a = dist(p1, p2), b = dist(p2, p3), c = dist(p1, p3), p = (a + b + c) / 2;
        return sqrtf(p * (p - a) * (p - b) * (p - c));
    }

    friend std::ostream& operator<< (std::ostream& os, Figure f);
};

std::ostream& operator<< (std::ostream& os, Figure f) //show list of verticies
{
    for (int i = 0; i < f.mVertices.size(); i++)
    {
        os << f.mVertices[i];
        if (i < f.mVertices.size() - 1) os << ", ";
    }
    return os;
}

class Trap : public Figure //trapezoid
{
public:
    Trap(float x, float y, float a, float b, float h, float phi) : Figure("trapezoid")
    {
        a /= 2;
        b /= 2;
        h /= 2;
        phi *= PI / 180;

        float s = sinf(phi), c = cosf(phi);
        mVertices.push_back(Point(x + h * s - a * c, y + h * c + a * s));
        mVertices.push_back(Point(x + h * s + a * c, y + h * c - a * s));
        mVertices.push_back(Point(x - h * s + b * c, y - h * c - b * s));
        mVertices.push_back(Point(x - h * s - b * c, y - h * c + b * s));
    }
};
class Romb : public Figure //rhombus
{
public:
    Romb(float x, float y, float a, float b, float phi) : Figure("rhombus")
    {
        a /= 2;
        b /= 2;
        phi *= PI / 180;

        float s = sinf(phi), c = cosf(phi);
        mVertices.push_back(Point(x + a * c, y - a * s));
        mVertices.push_back(Point(x - b * s, y - b * c));
        mVertices.push_back(Point(x - a * c, y + a * s));
        mVertices.push_back(Point(x + b * s, y + b * c));
    }
};
class Pent : public Figure //pentagon
{
public:
    Pent(float x, float y, float r, float phi) : Figure("pentagon")
    {
        phi *= PI / 180;

        for (int i = 0; i < 5; i++)
        {
            float a = phi + i * 2 * PI / 5;
            mVertices.push_back(Point(x + r * cos(a), y - r * sin(a)));
        }
    }
};

bool toInt(std::string s, int &output) //convert string to int
{
    int n = 0;
    for (int i = 0; i < s.size(); i++)
    {
        if (s[i] < '0' || s[i] > '9') return false;
        n = n * 10 + (s[i] - '0');
    }
    output = n;
    return true;
}
bool toFloat(std::string s, float &output) //convert string to float
{
    //check string for being a floating number
    int start = 0, npoints = 0;
    for (int i = 0; i < s.size(); i++)
    {
        if (s[i] == '-') //minus only as first symbol
        {
            if (i > 0) return false;
            start = 1;
        }
        else if (s[i] == '.') //only one point and in middle
        {
            if (i == start || i == s.size() - 1 || npoints++) return false;
        }
        else if (s[i] < '0' || s[i] > '9') return false; //any other non-digit is unavailable
    }
    //if it is -- convert to float
    output = ::atof(s.c_str());
    return true;
}
bool checkFloat(std::string s, float &output) //show error if fail
{
    if (!toFloat(s, output))
    {
        std::cout << '\"' << s << "\" is not a floating number" << std::endl;
        return false;
    }
    return true;
}
void executeGeneral(std::vector<std::string> &words, std::vector<Figure*> &figures, void lambda(Figure *f, float &t), bool showTotal)
{
    float total = 0;

    if (words.size() > 2) std::cout << words[0] << " has 0 or 1 argument" << std::endl;
    else if (words.size() == 2) //middle i
    {
        int i;
        if (!toInt(words[1], i)) std::cout << "argument must be positive integer" << std::endl;
        else if (i >= figures.size()) std::cout << "out of bounds" << std::endl;
        else lambda(figures[i], total);
    }
    else
    {
        if (figures.size() == 0) std::cout << "array is empty" << std::endl;
        else for (Figure *f : figures)
        {
            std::cout << f->getName() << ": ";
            lambda(f, total);
        }

        if (showTotal) std::cout << "Total: " << total << std::endl;
    }
}

void help() //show help
{
    std::cout <<
        "quit -- exit program" << std::endl <<
        "help -- show this text" << std::endl <<
        "clear -- clear vector of figures" << std::endl <<
        "middle -- show mid-point of all figures" << std::endl <<
        "middle i -- show mid-point of i-th figure (index starts from 0)" << std::endl <<
        "area -- show area of all figures and sum of areas" << std::endl <<
        "area i -- show area of i-th figure" << std::endl <<
        "show -- show points of all figures" << std::endl <<
        "show i -- show points of i-th figure" << std::endl <<
        "swap i j -- swap i-th and j-th figures" << std::endl <<
        "remove i -- remove i-th figure" << std::endl <<
        "push *figure* -- add figure at the end of vector" << std::endl <<
        "===FIGURES===" << std::endl <<
        "trap x y a b h phi" << std::endl <<
        "- x,y -- center of mid-line of trapezoid" << std::endl <<
        "- a,b -- lengths of bases of trapezoid (lower and higher)" << std::endl <<
        "- h -- height of trapezoid" << std::endl <<
        "- phi -- angle of rotation" << std::endl <<
        "romb x y a b phi" << std::endl <<
        "- x,y -- point of intersection of diagonals" << std::endl <<
        "- a,b -- lengths of diagonals of rhombus (horisontal and vertical)" << std::endl <<
        "- phi -- angle of rotation" << std::endl <<
        "pent x y r phi" << std::endl <<
        "-- x,y -- center of circumscribed circle" << std::endl <<
        "-- r -- radius circumscribed circle" << std::endl <<
        "-- phi -- angle of rotation" << std::endl;
}

int main()
{
    std::vector<Figure*> figures; //array of figures
    std::string input; //user input
    bool loop = true;

    std::cout.precision(3); //set output format to %.3f
    std::cout << std::fixed <<
        "+---------------------------------+" << std::endl <<
        "|Welcome to the figure calculator!|" << std::endl <<
        "+---------------------------------+" << std::endl <<
        "Type 'help' to get list of comands" << std::endl;

    while (loop)
    {
        std::cout << "> ";
        std::getline(std::cin, input); //read uer input

        //split string
        std::stringstream ss(input);
        std::vector<std::string> words;
        for (std::string s; ss >> s; ) //split into words
        {
            for (int i = 0; i < s.size(); i++) s[i] = std::tolower(s[i]); //set register to lower
            words.push_back(s);
        }
        if (words.size() == 0) continue;
        //for (auto &o : words) std::cout << o << std::endl;

        if (words[0] == "quit") //exit program
        {
            if (words.size() > 1) std::cout << words[0] << " has 0 arguments" << std::endl;
            else loop = false;
        }
        else if (words[0] == "help") //show help
        {
            if (words.size() > 1) std::cout << words[0] << " has 0 arguments" << std::endl;
            else help();
        }
        else if (words[0] == "clear") //clear list
        {
            if (words.size() > 1) std::cout << words[0] << " has 0 arguments" << std::endl;
            else
            {
                for (int i = 0; i < figures.size(); i++) delete figures[i];
                figures.clear();
            }
        }
        else if (words[0] == "middle") //mid-point of figures or figure
            executeGeneral(words, figures, [](Figure *f, float &total)
            {
                std::cout << f->getMiddle() << std::endl;
            }, false);
        else if (words[0] == "area") //area of figures or figure
            executeGeneral(words, figures, [](Figure *f, float &total)
            {
                float a = f->getArea();
                std::cout << f->getArea() << std::endl;
                total += a;
            }, true);
        else if (words[0] == "show") //area of figures or figure
            executeGeneral(words, figures, [](Figure *f, float &total)
            {
                std::cout << *f << std::endl;
            }, false);
        else if (words[0] == "swap") //swap two elements of array
        {
            if (words.size() != 3) std::cout << words[0] << " has 2 arguments" << std::endl;
            else
            {
                int i, j;
                if (!toInt(words[1], i) || !toInt(words[2], j)) std::cout << "argument must be positive integers" << std::endl;
                else
                {
                    if (i >= figures.size() || j >= figures.size()) std::cout << "out of bounds" << std::endl;
                    else std::swap(figures[i], figures[j]);
                }
            }
        }
        else if (words[0] == "remove") //remove element of array
        {
            if (words.size() != 2) std::cout << words[0] << " has 1 argument" << std::endl;
            else
            {
                int i;
                if (!toInt(words[1], i)) std::cout << "argument must be positive integer" << std::endl;
                else
                {
                    if (i >= figures.size()) std::cout << "out of bounds" << std::endl;
                    else
                    {
                        delete figures[i];
                        figures.erase(figures.begin() + i);
                    }
                }
            }
        }
        else if (words[0] == "push") //add element to the end of array
        {
            if (words.size() < 2) std::cout << words[0] << " requires description of figure" << std::endl;
            else
            {
                if (words[1] == "trap") //trapezoid
                {
                    if (words.size() != 8) std::cout << words[1] << " has 6 arguments" << std::endl;
                    else
                    {
                        float x, y, a, b, h, phi;
                        if (checkFloat(words[2], x) && checkFloat(words[3], y) && checkFloat(words[4], a)
                            && checkFloat(words[5], b) && checkFloat(words[6], h) && checkFloat(words[7], phi))
                            figures.push_back(new Trap(x, y, a, b, h, phi));
                    }
                }
                else if (words[1] == "romb") //rhombus
                {
                    if (words.size() != 7) std::cout << words[1] << " has 5 arguments" << std::endl;
                    else
                    {
                        float x, y, a, b, phi;
                        if (checkFloat(words[2], x) && checkFloat(words[3], y) && checkFloat(words[4], a)
                            && checkFloat(words[5], b) && checkFloat(words[6], phi))
                            figures.push_back(new Romb(x, y, a, b, phi));
                    }
                }
                else if (words[1] == "pent") //pentagon
                {
                    if (words.size() != 6) std::cout << words[1] << " has 4 arguments" << std::endl;
                    else
                    {
                        float x, y, r, phi;
                        if (checkFloat(words[2], x) && checkFloat(words[3], y) &&
                            checkFloat(words[4], r) && checkFloat(words[6], phi))
                            figures.push_back(new Pent(x, y, r, phi));
                    }
                }
                else std::cout << "unknown figure " << words[1] << std::endl;
            }
        }
    }
    return 0;
}