/*
* Туманов Георгий М8О-201Б
* ========================
* Вариант 6:
* Создать класс BitString для работы с 96-битовыми строками. Битовая строка
* должна быть представлена двумя полями: старшая часть unsigned long long,
* младшая часть unsigned int. Должны быть реализованы все традиционные операции
* для работы с битами: and, or, xor, not. Реализовать сдвиг влево shiftLeft и
* сдвиг вправо shiftRight на заданное количество битов. Реализовать операцию
* вычисления количества единичных битов, операции сравнения по количеству единичных
* битов. Реализовать операцию проверки включения.
* Операции and, or, xor, not, сравнения (на равенство, больше и меньше) должны быть
* выполнены в виде перегрузки операторов.
* Необходимо реализовать пользовательский литерал для работы с константами типа BitString.
*/
#include <iostream> //for I/O
#include <bitset> //for viewing binary
#include <sstream> //for parsing
#include <vector> //for parsing

class BitString
{
public:
    BitString() { mA = 0; mB = 0; } //default constructor
    BitString(uint64_t a, uint32_t b) : mA(a), mB(b) {} //set values manualy
    BitString(const char* c) //from string
    {
        mA = 0; mB = 0;

        uint64_t aa = 1; uint32_t bb = 1; //iterators for setting bits
        int len; //length of string

        for (len = 0; len < 96; len++) if (c[len] == '\0') break; //get length

        //read b
        for (int i = 0; i < len && i < 32; i++)
        {
            if (c[len - i - 1] == '1') mB |= bb;
            else if (c[len - i - 1] != '0') //error
            {
                std::cerr << "Unable to read " << c << " as BitString" << std::endl;
                mA = mB = 0;
                return;
            }
            bb <<= 1;
        }
        //read a
        for (int i = 32; i < len && i < 96; i++)
        {
            if (c[len - i - 1] == '1') mA |= aa;
            else if (c[len - i - 1] != '0') //error
            {
                std::cerr << "Unable to read " << c << " as BitString" << std::endl;
                mA = mB = 0;
                return;
            }
            aa <<= 1;
        }
    }

    int Ones() //number of non-zero bits
    {
        int res = 0; uint64_t a = 1; uint32_t b = 1;
        for (int i = 0; i < 64; i++) { if (mA & a) res++; a <<= 1; }
        for (int i = 0; i < 32; i++) { if (mB & b) res++; b <<= 1; }
        return res;
    }
    bool Inside(BitString where) { return (~where & *this).Ones() == 0; } //checks if bitString is inside other one

    BitString operator~ () { return BitString(~mA, ~mB); } //bit inverse

    //bit operations
    BitString operator& (BitString other) { return BitString(mA & other.mA, mB & other.mB); }
    BitString operator| (BitString other) { return BitString(mA | other.mA, mB | other.mB); }
    BitString operator^ (BitString other) { return BitString(mA ^ other.mA, mB ^ other.mB); }

    BitString& operator&= (BitString other) { mA &= other.mA; mB &= other.mB; return *this; }
    BitString& operator|= (BitString other) { mA |= other.mA; mB |= other.mB; return *this; }
    BitString& operator^= (BitString other) { mA ^= other.mA; mB ^= other.mB; return *this; }

    //bit shifts
    BitString operator<< (unsigned int n) { return BitString((mA << n) | (mB >> (32 - n)), mB << n); }
    BitString operator>> (unsigned int n) { return BitString(mA >> n, ((mA & 0xFFFFFFFF) << (32 - n)) | (mB >> n)); }

    BitString& operator<<= (unsigned int n) { mA = (mA << n) | (mB >> (32 - n)); mB = mB << n; return *this; }
    BitString& operator>>= (unsigned int n) { mB = ((mA & 0xFFFFFFFF) << (32 - n)) | (mB >> n); mA = mA >> n; return *this; }

    //compare strings by ammount of non-zero bits
    bool operator< (BitString other) { return Ones() < other.Ones(); }
    bool operator> (BitString other) { return Ones() > other.Ones(); }
    bool operator<= (BitString other) { return Ones() <= other.Ones(); }
    bool operator>= (BitString other) { return Ones() >= other.Ones(); }

    //compare bits of strings
    bool operator== (BitString other) { return mA == other.mA && mB == other.mB; }
    bool operator!= (BitString other) { return mA != other.mA || mB != other.mB; }

private:
    uint64_t mA;
    uint32_t mB;

    friend std::ostream& operator<<(std::ostream& os, BitString b);
};

//show string on screen
std::ostream& operator<<(std::ostream& os, BitString b)
{
    os << std::bitset<64>(b.mA) << ':' << std::bitset<32>(b.mB);
    return os;
}
//user literal
BitString operator "" _bs(const char* c) { return BitString(c); }

//show help
void print_help()
{
    std::cout <<
        "Terminology:" << std::endl <<
        "- a, b, ..., z -- variables" << std::endl <<
        "- any sequence of 0 or 1 (1001, 0001110) -- constant" << std::endl <<
        "- variable, constant or their negation (a, ~a, 101, ~101) -- term" << std::endl <<
        "- term, term & term, t | t, t ^ t, t << integer, t >> int -- expression" << std::endl <<
        std::endl <<
        "Commands:" << std::endl <<
        "- quit -- close program" << std::endl <<
        "- help -- show this text" << std::endl <<
        "- ones <expression> -- number of non-zero bits in result of expression" << std::endl <<
        "- inside <expression> of <expression> -- checks if left expr is inside right one" << std::endl <<
        "- <variable> = <expression> -- set variable value" << std::endl <<
        "- term < term, t > t, t <= t, t >= t -- compare number of non-zero bits of terms" << std::endl <<
        "- <expression> -- show result of expression" << std::endl;
}

bool isvar(std::string s) //is "s" a variable name
{
    return s.size() == 1 && s[0] >= 'a' && s[0] <= 'z';
}
int toInt(std::string s) //convert string to integer, return -1 if not integer
{
    int n = 0;
    for (int i = 0; i < s.size(); i++)
    {
        if (s[i] < '0' || s[i] > '9') return -1; //any non-digit is unacceptable
        else n = n * 10 + s[i] - '0';
    }
    return n;
}
BitString getVal(std::string s, BitString arr[26], bool &good) //get value of term
{
    bool inv = false;
    good = false;

    BitString ret;
    if (s[0] == '~') { s = s.replace(0, 1, ""); inv = true; } //remove ~ and inverse value later
    if (isvar(s)) ret = arr[s[0] - 'a']; //term is variable
    else //term is constant
    {
        for (int i = 0; i < s.size(); i++)  //check constant for unacceptable characters
            if (s[i] != '0' && s[i] != '1')
                return BitString(0, 0);

        ret = BitString(s.c_str());
    }
    if (inv) ret = ~ret; //inverse

    good = true;
    return ret;
}
BitString expr(std::vector<std::string> &input, int start, BitString arr[26], bool &good) //get result of expression
{
    bool good1;
    good = false;

    BitString b = getVal(input[start], arr, good1); //get left term
    if (!good1) return BitString(0, 0); //error

    std::string s = input[start + 1]; //operation
    
    if (s == "<<" || s == ">>") //if shifting, right argument is integer
    {
        int n = toInt(input[start + 2]);
        if (n < 0) return BitString(0, 0); //not integer

        good = true;
        if (s == "<<") return b << n; //perform shifting
        return b >> n;
    }
    //if not shifting, right argument is term
    BitString b2 = getVal(input[start + 2], arr, good1); //get right term
    if (!good1) return BitString(0, 0); //error

    good = true;
    if (s == "&") return b & b2; //perform operations
    if (s == "|") return b | b2;
    if (s == "^") return b ^ b2;

    good = false; //incorrect operation
    return BitString(0, 0);
}

int main() //entry point
{
    BitString b[26]; //variables
    std::string input; //user input
    bool loop = true;

    std::cout << 
        "+-------------------------------------+" << std::endl <<
        "|Welcome to the bit string calculator!|" << std::endl <<
        "+-------------------------------------+" << std::endl <<
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

        BitString temp, temp2; //for calculations
        
        if (words[0] == "help") //help
        {
            if (words.size() == 1) print_help();
            else std::cout << "help requires 0 arguments" << std::endl;
        }
        else if (words[0] == "quit") //quit
        {
            if (words.size() == 1) loop = false;
            else std::cout << "quit requires 0 arguments" << std::endl;
        }
        else if (words[0] == "ones") //ones
        {
            if (words.size() == 2 || words.size() == 4) //is argument expression
            {
                bool good;
                temp = (words.size() == 2) ? getVal(words[1], b, good) : expr(words, 1, b, good); //get value expression
                if (good) std::cout << temp.Ones() << std::endl; //assign value if expression is good
                else std::cout << "bad expression" << std::endl; //error
            }
            else std::cout << "argument is not expression" << std::endl; //not an expression on right side
        }
        else if (words[0] == "inside") //inside .. of ..
        {
            //find of
            int i;
            for (i = 1; i < words.size(); i++) if (words[i] == "of") break;

            if (i == words.size()) std::cout << "keyword 'of' not found" << std::endl; //no of
            else
            {
                int sz1 = i - 1, sz2 = words.size() - i - 1;

                if ((sz1 != 1 && sz1 != 3) || (sz2 != 1 && sz2 != 3))
                    std::cout << "arguments are not expressions" << std::endl; //error
                else
                {
                    //get left side
                    bool good;
                    temp = (sz1 == 1) ? getVal(words[1], b, good) : expr(words, 1, b, good);
                    if (!good) std::cout << "bad expression" << std::endl; //error
                    else
                    {
                        //get right side
                        temp2 = (sz2 == 1) ? getVal(words[i + 1], b, good) : expr(words, i + 1, b, good);
                        if (!good) std::cout << "bad expression" << std::endl; //error
                        else
                        {
                            //check left inside of right
                            if (temp.Inside(temp2)) std::cout << "true" << std::endl;
                            else std::cout << "false" << std::endl;
                        }
                    }
                }
            }
        }
        else if (isvar(words[0]) && words.size() > 2 && words[1] == "=") //assignment of variable
        {
            if (words.size() == 3 || words.size() == 5) //is right side expression
            {
                bool good;
                temp = (words.size() == 3) ? getVal(words[2], b, good) : expr(words, 2, b, good); //get value expression
                if (good) b[words[0][0] - 'a'] = temp; //assign value if expression is good
                else std::cout << "bad expression" << std::endl; //error
            }
            else std::cout << "impossible assignment" << std::endl; //not an expression on right side
        }
        else if (words.size() == 1 || words.size() == 3) //expression or comparing
        {
            if (words.size() == 3 && (words[1] == "<" || words[1] == ">" || words[1] == "<=" || words[1] == ">=")) //comparing
            {
                bool good, res;
                temp = getVal(words[0], b, good); //left term
                if (!good) std::cout << "cannot compare" << std::endl;
                else
                {
                    temp2 = getVal(words[2], b, good); //right term
                    if (!good) std::cout << "cannot compare" << std::endl;
                    else
                    {
                        if (words[1] == "<") res = (temp < temp2);
                        else if (words[1] == "<=") res = (temp <= temp2);
                        else if (words[1] == ">") res = (temp > temp2);
                        else res = (temp >= temp2); //no errors here because of entry condition

                        if (res) std::cout << "true" << std::endl;
                        else std::cout << "false" << std::endl;
                    }
                }
            }
            else //show expression value
            {
                bool good;
                temp = (words.size() == 1) ? getVal(words[0], b, good) : expr(words, 0, b, good); //get value expression
                if (good) std::cout << temp << std::endl; //show value if expression is good
                else std::cout << "bad expression" << std::endl; //error
            }
        }
        else std::cout << "unknown construction" << std::endl; //wrong input
    }

    return 0;
}