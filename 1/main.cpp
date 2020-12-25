/*
* Туманов Георгий М8О-201Б
* ========================
* Вариант 10:
* Создать класс Angle для работы с углами на плоскости, задаваемыми величиной в градусах и минутах.
* Обязательно должны быть реализованы: перевод в радианы, приведение к диапазону 0–360, сложение и
* вычитание углов, деление углов, получение значений тригонометрических функций, сравнение углов.
*/

#include <iostream> //for cout and cerr
#define _USE_MATH_DEFINES //for M_PI
#include <cmath> //fot trigonometry

const double eps = 0.001; //for checking double

class Angle //describes angle in two variables: angle and minutes(1/60 part of angle)
{
public:
    Angle(int a, int m) : m_angle(a), m_minute(m) { Clamp(); } //set angle and minutes and transform
    //convert to radians
    double Radian() { return (static_cast<double>(m_angle) + static_cast<double>(m_minute) / 60.0) * M_PI / 180.0; }

    void Clamp() //transforms angle to [0 - 359.59'] range
    {
        int full = m_angle * 60 + m_minute; //whole angle represented as minutes
        full = (full % 21600 + 21600) % 21600; //to [0 - 359.59'] range (21600 = 60*360)
        m_angle = full / 60; //get angle
        m_minute = full % 60; //get minute
    }

    //trigonometric functions
    double Sin() { return sin(Radian()); }
    double Cos() { return cos(Radian()); }
    double Tan() { return tan(Radian()); }
    double Csc() { return 1.0/sin(Radian()); }
    double Sec() { return 1.0/cos(Radian()); }
    double Ctg() { return 1.0/tan(Radian()); }

    //getters and setters for angle and minutes
    int GetAngle() { return m_angle; }
    int GetMinute() { return m_minute; }
    void Set(int a, int m) { m_angle = a; m_minute = m; Clamp(); }

private:
    int m_angle, m_minute; //angle and mitutes themselves
};

//operator overloadings for angles
//addition
Angle operator+(Angle left, Angle right) { return Angle(left.GetAngle()+right.GetAngle(), left.GetMinute()+right.GetMinute()); }
Angle operator+=(Angle &left, Angle right) { left.Set(left.GetAngle()+right.GetAngle(), left.GetMinute()+right.GetMinute()); return left; }
//subtraction
Angle operator-(Angle left, Angle right) { return Angle(left.GetAngle()-right.GetAngle(), left.GetMinute()-right.GetMinute()); }
Angle operator-=(Angle &left, Angle right) { left.Set(left.GetAngle()-right.GetAngle(), left.GetMinute()-right.GetMinute()); return left; }
//division by number (handles division by zero)
Angle operator/(Angle a, double d)
{
    if (d > -eps || d < eps)
    {
        std::cerr << "Error! Dividing by zero!\n" << std::endl;
        return Angle(0, 0);
    }
    int res = static_cast<int>(static_cast<double>(a.GetAngle()*60 + a.GetMinute())/d);
    return Angle(res / 60, res % 60);
}
Angle operator/=(Angle &a, double d)
{
    if (d == 0.0)
    {
        std::cerr << "Error! Dividing by zero!\n" << std::endl;
        a.Set(0, 0);
        return a;
    }
    int res = static_cast<int>(static_cast<double>(a.GetAngle()*60 + a.GetMinute())/d);
    a.Set(res / 60, res % 60);
    return a;
}
//comparing angles
bool operator==(Angle a, Angle b) { return a.GetAngle() == b.GetAngle() && a.GetMinute() == b.GetMinute(); }
bool operator!=(Angle a, Angle b) { return a.GetAngle() != b.GetAngle() || a.GetMinute() != b.GetMinute(); }
bool operator<(Angle a, Angle b) { return a.GetAngle()*60 + a.GetMinute() < b.GetAngle()*60 + b.GetMinute(); }
bool operator>(Angle a, Angle b) { return a.GetAngle()*60 + a.GetMinute() > b.GetAngle()*60 + b.GetMinute(); }
bool operator<=(Angle a, Angle b) { return a.GetAngle()*60 + a.GetMinute() <= b.GetAngle()*60 + b.GetMinute(); }
bool operator>=(Angle a, Angle b) { return a.GetAngle()*60 + a.GetMinute() >= b.GetAngle()*60 + b.GetMinute(); }
//output through any output stream
std::ostream& operator<<(std::ostream& o, Angle a)
{
    o << a.GetAngle() << '.' << a.GetMinute() << '\'';
    return o;
}

//for inputing angles
void readAngle(Angle &b)
{
    int a, m;
    std::cout << "Write the integer part of angle: ";
    std::cin >> a;

    std::cout << "Write the minutes of angle: ";
    std::cin >> m;
    b.Set(a, m);
}

//entry point
int main()
{
    std::cout << 
        "+--------------------------------+" << std::endl <<
        "|Welcome to the angle calculator!|" << std::endl <<
        "+--------------------------------+" << std::endl;

    int menu; //user command
    double den; //fon division
    Angle angle(0, 0), b(0, 0); //angles for calculations

    //main loop
    bool loop = true;
    while (loop)
    {
        //draw menu
        std::cout << std::endl << "Angle = " << angle << std::endl <<
            "1. Set angle" << std::endl <<
            "2. Add angle" << std::endl <<
            "3. Subtract angle" << std::endl <<
            "4. Divide angle" << std::endl <<
            "5. Compare with angle" << std::endl <<
            "6. Get radians" << std::endl <<
            "7. Trigonometric functions" << std::endl <<
            "0. Quit" << std::endl << "> ";
        //get user input
        std::cin >> menu;

        //functions
        switch (menu)
        {
        case 1: readAngle(angle); break;
        case 2: readAngle(b); angle += b; break;
        case 3: readAngle(b); angle -= b; break;
        case 4:
            std::cout << "Write the denominator: ";
            std::cin >> den;
            angle /= den;
            break;
        case 5:
            readAngle(b);
            if (b == angle) std::cout << "Equal." << std::endl;
            else if (b < angle) std::cout << "Less." << std::endl;
            else std::cout << "Greater." << std::endl;
            break;
        case 6: std::cout << "Radians: " << angle.Radian() << std::endl; break;
        case 7:
        {
            std::cout << "Choose function:" << std::endl <<
                "1. Sin" << std::endl <<
                "2. Cos" << std::endl <<
                "3. Tan" << std::endl <<
                "4. Ctg" << std::endl <<
                "5. Sec" << std::endl <<
                "6. Csc" << std::endl <<
                "0. Cancel" << std::endl << "> ";
            std::cin >> menu;

            switch (menu)
            {
            case 1: std::cout << "Sin(" << angle << ") = " << angle.Sin() << std::endl; break;
            case 2: std::cout << "Cos(" << angle << ") = " << angle.Cos() << std::endl; break;
            case 3: std::cout << "Tan(" << angle << ") = " << angle.Tan() << std::endl; break;
            case 4: std::cout << "Ctg(" << angle << ") = " << angle.Ctg() << std::endl; break;
            case 5: std::cout << "Sec(" << angle << ") = " << angle.Sec() << std::endl; break;
            case 6: std::cout << "Csc(" << angle << ") = " << angle.Csc() << std::endl; break;
            }
            break;
        }
        case 0: loop = false; break;
        }
    }

    return 0;
}

