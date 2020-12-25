/*
* Туманов Георгий М8О-201Б
* ========================
* Вариант 4:
* Трапеция
* Ромб
* 5-угольник
*/

#include <sstream>     //std::stringstream
#include <variant>     //std::variant
#include "figures.h"

#define v(vis, fig) std::visit(vis, fig)
#define nv(nvis, vis, fig) v(nvis, fig) << ": " << v(vis, fig)

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
        "trap ax ay bx by cx cy" << std::endl <<
        "- ax,ay,bx,by -- vertices of lower base of trapezoid" << std::endl <<
        "- cx,cy -- vertex of upper base" << std::endl <<
        "romb ax ay bx by phi" << std::endl <<
        "- ax,ay,bx,by -- vertices of diagonal of rhombus" << std::endl <<
        "- h -- length of other diagonal" << std::endl <<
        "pent ox oy ax ay" << std::endl <<
        "-- ox,oy -- center of circumscribed circle" << std::endl <<
        "-- ax,ay -- one of the vertices of pentagon" << std::endl;
}

int main()
{
    std::vector<std::variant<Trapezoid<float>, Rhombus<float>, Pentagon<float>>> figures; //vector of figures
    std::string input; //user input
    bool loop = true;

    std::cout.precision(3); //set output format to %.3f
    std::cout << std::fixed <<
        "+---------------------------------+" << std::endl <<
        "|Welcome to the figure calculator!|" << std::endl <<
        "+---------------------------------+" << std::endl <<
        "Type 'help' to get list of comands" << std::endl;

    //visitors
    AreaVisitor<float> a_vis;
    ShowVisitor<float> s_vis;
    CenterVisitor<float> c_vis;
    NameVisitor<float> n_vis;

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
                    else figures.erase(figures.begin() + i);
                }
            }
        }
        else if (words[0] == "clear") //clear list
        {
            if (words.size() > 1) std::cout << words[0] << " has 0 arguments" << std::endl;
            else figures.clear();
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
                        float ax, ay, bx, by, cx, cy;
                        if (checkFloat(words[2], ax) && checkFloat(words[3], ay) && checkFloat(words[4], bx)
                            && checkFloat(words[5], by) && checkFloat(words[6], cx) && checkFloat(words[7], cy))
                            figures.push_back(Trapezoid<float>(ax, ay, bx, by, cx, cy));
                    }
                }
                else if (words[1] == "romb") //rhombus
                {
                    if (words.size() != 7) std::cout << words[1] << " has 5 arguments" << std::endl;
                    else
                    {
                        float ax, ay, bx, by, h;
                        if (checkFloat(words[2], ax) && checkFloat(words[3], ay) && checkFloat(words[4], bx)
                            && checkFloat(words[5], by) && checkFloat(words[6], h))
                            figures.push_back(Rhombus<float>(ax, ay, bx, by, h));
                    }
                }
                else if (words[1] == "pent") //pentagon
                {
                    if (words.size() != 6) std::cout << words[1] << " has 4 arguments" << std::endl;
                    else
                    {
                        float ox, oy, ax, ay;
                        if (checkFloat(words[2], ox) && checkFloat(words[3], oy) &&
                            checkFloat(words[4], ax) && checkFloat(words[5], ay))
                            figures.push_back(Pentagon<float>(ox, oy, ax, ay));
                    }
                }
                else std::cout << "unknown figure " << words[1] << std::endl;
            }
        }
        else if (words[0] == "area") //show areas
        {
            if (words.size() > 2) std::cout << words[0] << " has 0 or 1 arguments" << std::endl;
            else if (words.size() == 1) //no args
            {
                float summ = 0, s;
                for (auto &a : figures)
                {
                    summ += (s = v(a_vis, a));
                    std::cout << v(n_vis, a) << ": " << s << std::endl;
                }
                std::cout << "Total: " << summ << std::endl;
            }
            else //arg
            {
                int arg;
                if (!toInt(words[1], arg)) std::cout << "argument must be positive integer" << std::endl;
                else if (arg >= figures.size()) std::cout << "out of bounds" << std::endl;
                else std::cout << nv(n_vis, a_vis, figures[arg]) << std::endl;
            }
        }
        else if (words[0] == "show") //show figures
        {
            if (words.size() > 2) std::cout << words[0] << " has 0 or 1 arguments" << std::endl;
            else if (words.size() == 1) //no args
            {
                for (auto &a : figures) { std::cout << v(n_vis, a) << ": "; v(s_vis, a); }
            }
            else //arg
            {
                int arg;
                if (!toInt(words[1], arg)) std::cout << "argument must be positive integer" << std::endl;
                else if (arg >= figures.size()) std::cout << "out of bounds" << std::endl;
                else { std::cout << v(n_vis, figures[arg]) << ": "; v(s_vis, figures[arg]); }
            }
        }
        else if (words[0] == "middle") //show figures
        {
            if (words.size() > 2) std::cout << words[0] << " has 0 or 1 arguments" << std::endl;
            else if (words.size() == 1) //no args
            {
                for (auto &a : figures) std::cout << nv(n_vis, c_vis, a) << std::endl;
            }
            else //arg
            {
                int arg;
                if (!toInt(words[1], arg)) std::cout << "argument must be positive integer" << std::endl;
                else if (arg >= figures.size()) std::cout << "out of bounds" << std::endl;
                else std::cout << nv(n_vis, c_vis, figures[arg]) << std::endl;
            }
        }
        else std::cout << "unknown command" << std::endl;
    }

    return 0;
}