/*
* Туманов Георгий М8О-201Б
* ========================
* Вариант 14:
* 5-угольник
* 6-угольник
* 8-угольник
*/

#include <sstream>  //std::stringstream
#include <map>      //std::map
#include "document.h"

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
int fromName(std::string s) //get number of vertices by name
{
    if (s == "pent") return 5;
    if (s == "hex") return 6;
    if (s == "oct") return 8;
    throw std::logic_error("unknown figure " + s);
}

void help(std::vector<std::string>) //show help
{
    std::cout <<
        "quit -- exit program" << std::endl <<
        "help -- show this text" << std::endl <<
        "new -- create new document" << std::endl <<
        "load fname -- load document from file" << std::endl <<
        "save fname -- save document" << std::endl <<
        "area -- show area of all figures" << std::endl <<
        "area i -- show area of i-th figure" << std::endl <<
        "center -- show center of all figures" << std::endl <<
        "center i -- show center of i-th figure" << std::endl <<
        "show -- show points of all figures" << std::endl <<
        "show i -- show points of i-th figure" << std::endl <<
        "remove i -- remove i-th figure" << std::endl <<
        "insert i *figure* -- insert figure BEFORE i-th figure" << std::endl <<
        "undo -- undo adding/deleting figure" << std::endl <<
        "redo -- cancel undoing" << std::endl <<
        "===FIGURES===" << std::endl <<
        "pent ox oy ax ay" << std::endl <<
        "hex ox oy ax ay" << std::endl <<
        "oct ox oy ax ay" << std::endl <<
        "-- ox,oy -- center of circumscribed circle" << std::endl <<
        "-- ax,ay -- one of the vertices of figure" << std::endl;
}

int main()
{
    //current document
    Document<float> doc;
    bool loop = true; //variable for loop
    std::string input; //inputed string

    //map of comands and number of arguments
    std::map<std::pair<std::string, int>, std::function<void(std::vector<std::string>)>> commands;

    //bind all commands
    commands[std::pair("quit", 0)] = [&loop](std::vector<std::string>){ loop = false; };
    commands[std::pair("help", 0)] = help;

    //document functions
    commands[std::pair("new", 0)] = [&doc](std::vector<std::string>){ doc.New(); };
    commands[std::pair("load", 1)] = [&doc](std::vector<std::string> v){ doc.Load(v[0]); };
    commands[std::pair("save", 1)] = [&doc](std::vector<std::string> v){ doc.Save(v[0]); };

    //figure functions for all
    commands[std::pair("area", 0)] = [&doc](std::vector<std::string>)
    {
        doc.ForEach([](Ngon<float> n)
        {
            std::cout << name(n) << ": " << area(n) << std::endl;
        });
    };
    commands[std::pair("center", 0)] = [&doc](std::vector<std::string>)
    {
        doc.ForEach([](Ngon<float> n)
        {
            std::cout << name(n) << ": " << center(n) << std::endl;
        });
    };
    commands[std::pair("show", 0)] = [&doc](std::vector<std::string>)
    {
        doc.ForEach([](Ngon<float> n)
        {
            std::cout << name(n) << ": " << n << std::endl;
        });
    };
    //figure functions for index
    commands[std::pair("area", 1)] = [&doc](std::vector<std::string> v)
    {
        doc.Call<void>(toInt(v[0]), [](Ngon<float> n)
        {
            std::cout << name(n) << ": " << area(n) << std::endl;
        });
    };
    commands[std::pair("center", 1)] = [&doc](std::vector<std::string> v)
    {
        doc.Call<void>(toInt(v[0]), [](Ngon<float> n)
        {
            std::cout << name(n) << ": " << center(n) << std::endl;
        });
    };
    commands[std::pair("show", 1)] = [&doc](std::vector<std::string> v)
    {
        doc.Call<void>(toInt(v[0]), [](Ngon<float> n)
        {
            std::cout << name(n) << ": " << n << std::endl;
        });
    };
    //figure managment
    commands[std::pair("remove", 1)] = [&doc](std::vector<std::string> v) { doc.Remove(toInt(v[0])); };
    commands[std::pair("insert", 6)] = [&doc](std::vector<std::string> v)
    {
        doc.Add(toInt(v[0]), fromName(v[1]), toDouble(v[2]), toDouble(v[3]), toDouble(v[4]), toDouble(v[5]));
    };
    //undo redo
    commands[std::pair("undo", 0)] = [&doc](std::vector<std::string>){ doc.Undo(); };
    commands[std::pair("redo", 0)] = [&doc](std::vector<std::string>){ doc.Redo(); };

    //start UI
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

        auto iter = commands.find(std::pair(words[0], words.size() - 1));
        if (iter == commands.end()) //not a command in map
        {
            std::cout << "unknowm command \"" << words[0] << "\" with " << words.size() - 1 << " arguments" << std::endl;
            continue;
        }
        //get command data
        auto com = (*iter).second;
        //execute
        words.erase(words.begin());
        try
        {
            com(words);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
    return 0;
}