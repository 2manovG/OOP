/*
* Туманов Георгий М8О-201Б
* ========================
* Вариант 14:
* 5-угольник
* Список
*/

#include <algorithm>   //std::count_if
#include <sstream>     //std::stringstream
#include <vector>      //std::vector
#include "figure.h"
#include "container.h"
#include "allocator.h"

int toInt(std::string s) //convert string to int
{
    int n = 0;
    for (int i = 0; i < s.size(); i++)
    {
        if (s[i] < '0' || s[i] > '9') throw std::logic_error("argument is not a positive integer");
        n = n * 10 + (s[i] - '0');
    }
    return n;
}
int toInt2(std::string s) //convert string to int
{
    try
    {
        if (s[0] == '-') return -toInt(s.substr(1));
        return toInt(s);
    }
    catch(const std::exception& e)
    {
        throw std::logic_error("argument is not an integer");
    }
}
double toDouble(std::string s) //convert string to float
{
    //check string for being a floating number
    int start = 0, npoints = 0;
    for (int i = 0; i < s.size(); i++)
    {
        if (s[i] == '-') //minus only as first symbol
        {
            if (i > 0) throw std::logic_error("argument is not a double");
            start = 1;
        }
        else if (s[i] == '.') //only one point and in middle
        {
            if (i == s.size() - 1 || npoints++) throw std::logic_error("argument is not a double");
        }
        else if (s[i] < '0' || s[i] > '9') throw std::logic_error("argument is not a double"); //any other non-digit is unavailable
    }
    //if it is -- convert to double
    return ::atof(s.c_str());
}

void help() //show help
{
    std::cout <<
        "quit -- exit program" << std::endl <<
        "help -- show this text" << std::endl <<
        "clear -- clear list of figures" << std::endl <<
        "area -- show area of all figures" << std::endl <<
        "area i -- show area of i-th figure" << std::endl <<
        "show -- show points of all figures" << std::endl <<
        "show i -- show points of i-th figure" << std::endl <<
        "smaller S -- show number of figures, which area is smaller than S" << std::endl <<
        "remove i -- remove i-th figure" << std::endl <<
        "insert i *figure* -- insert figure BEFORE i-th figure" << std::endl <<
        "save fname -- serialize list in fname" << std::endl <<
        "load fname -- deserialize list from fname" << std::endl <<
        "===FIGURES===" << std::endl <<
        "pent ox oy ax ay" << std::endl <<
        "-- ox,oy -- center of circumscribed circle" << std::endl <<
        "-- ax,ay -- one of the vertices of pentagon" << std::endl;
}

int main()
{
    List<Pentagon<int>, Allocator<Pentagon<int>, 10000>> figures; //list of figures
    std::string input; //user input
    bool loop = true;

    std::cout <<
        "+---------------------------------+" << std::endl <<
        "|Welcome to the figure calculator!|" << std::endl <<
        "+---------------------------------+" << std::endl <<
        "Type 'help' to get list of commands" << std::endl;

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
        
        try
        {
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
                else figures.clear();
            }
            else if (words[0] == "area") //show areas
            {
                if (words.size() > 2) std::cout << words[0] << " has 0 or 1 arguments" << std::endl;
                else if (words.size() == 1) //no args
                {
                    int i = 0;
                    for (auto &a : figures) std::cout << "Pentagon #" << i++ << ": " << area(a) << std::endl;
                }
                else //arg
                {
                    int arg = toInt(words[1]);
                    double val = area(figures[arg]);
                    std::cout << "Pentagon #" << arg << ": " << val << std::endl;
                }
            }
            else if (words[0] == "show") //show figures
            {
                if (words.size() > 2) std::cout << words[0] << " has 0 or 1 arguments" << std::endl;
                else if (words.size() == 1) //no args
                {
                    int i = 0;
                    for (auto &a : figures) std::cout << "Pentagon #" << i++ << ": " << a << std::endl;
                }
                else //arg
                {
                    int arg = toInt(words[1]);
                    Pentagon<int>& val = figures[arg];
                    std::cout << "Pentagon #" << arg << ": " << val << std::endl;
                }
            }
            else if (words[0] == "smaller") //exit program
            {
                if (words.size() != 2) std::cout << words[0] << " has 1 argument" << std::endl;
                else
                {
                    double S = toDouble(words[1]);
                    std::cout << "Count: " << std::count_if(figures.begin(), figures.end(),
                        [&S](Pentagon<int> &p){ return area(p) < S; }) << std::endl;
                }
            }
            else if (words[0] == "remove") //remove element from list
            {
                if (words.size() != 2) std::cout << words[0] << " has 1 argument" << std::endl;
                else figures.erase(std::next(figures.begin(), toInt(words[1])));
            }
            else if (words[0] == "insert") //add element to the list
            {
                //insert i pent ox oy ax ay
                if (words.size() != 7) std::cout << words[0] << " has 6 argument" << std::endl;
                else
                {
                    if (words[2] != "pent") std::cout << "unknown figure" << std::endl;
                    else
                    {
                        figures.insert(std::next(figures.begin(), toInt(words[1])),
                            Pentagon<int>(toInt2(words[3]), toInt2(words[4]), toInt2(words[5]), toInt2(words[6])));
                    }
                }
            }
            else if (words[0] == "save") //serialize list
            {
                if (words.size() != 2) std::cout << words[0] << " has 1 argument" << std::endl;
                else figures.save(words[1]);
            }
            else if (words[0] == "load") //deserialize list
            {
                if (words.size() != 2) std::cout << words[0] << " has 1 argument" << std::endl;
                else figures.load(words[1]);
            }
            else std::cout << "unknown command" << std::endl;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }
    }
    return 0;
}