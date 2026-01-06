#include "bigint.hpp"
#include "sstream"
#include "cstdlib"

bigint::bigint(): str("0")
{
    // std::cout << str << std::endl;
}

bigint::bigint(unsigned int n)
{
    std::stringstream ss;
    ss << n;
    str = ss.str();
    // std::cout << str << std::endl;
}

bigint::bigint(const bigint& other): str(other.str)
{}


bigint::~bigint(){}


std::ostream& operator<<(std::ostream& out, const bigint& src)
{
    out << src.str;
    return out;
}


// member operator overloads
bigint &bigint::operator+=(const bigint& num)
{
    *this = *this + num;
    return *this;
}
bigint &bigint::operator++()
{
    bigint a(1);
    // std::cout << "this is: " << this->str << std::endl;
    (*this) += a;
    // std::cout << "incremented this is: " << this->str << std::endl;

    return *this;
}

bigint bigint::operator++(int)
{
    bigint temp(*this);
    ++(*this);
    return temp;
}

bigint bigint::operator<<=(const unsigned int& shift)
{
    *this = *this<<shift;
    return *this;
}
bigint bigint::operator>>=(const unsigned int& shift)
{
    *this = *this>>shift;
    return *this;
}

bigint bigint::operator<<=(const bigint& shift)
{
    *this = *this<<shift;
    return *this;
}
bigint bigint::operator>>=(const bigint& shift)
{
    *this = *this>>shift;
    return *this;
}

// non member overloads

bigint operator+(const bigint& a, const bigint& b)
{
    bigint result;
    result.str = "";
    std::string backwards = "";
    int remainder = 0;

    unsigned int digits = (a.str.length() > b.str.length()) ? a.str.length() : b.str.length();
    for (unsigned int i = 0; i < digits || remainder; i++)
    {
        char cdig_a = (a.str.length() > i) ? a.str[a.str.length() - 1 - i] : '0';
        char cdig_b = (b.str.length() > i) ? b.str[b.str.length() - 1 - i] : '0';
        int dig_a = cdig_a - '0';
        int dig_b = cdig_b - '0';
        int digit = (dig_a + dig_b) % 10 + remainder;
        remainder = (dig_a + dig_b) / 10;
        char newdigit = digit + '0';
        backwards += newdigit;
    }
    for (unsigned int i = 0; i < backwards.length(); i++)
    {
        result.str += backwards[backwards.length() - 1 - i];
    }
    // std::cout << "addition result: " << result.str << std::endl;
    return result;
}

bigint operator<<(const bigint& a, const unsigned int& shift)
{
    bigint result = a;
    for (unsigned int i = 0; i < shift; i++)
    {
        result.str += "0";
    }
    return result;
}

bigint operator>>(const bigint& a, const unsigned int& shift)
{
    bigint result = a;
    // std::cout << "before shift: " << result.str <<std::endl;
    if (result.str.length() > shift)
        result.str.resize(result.str.length() - shift);
    else
    {
        result.str = "0";
    }
    return result;
    
}

bigint operator<<(const bigint& a, const bigint& shift)
{
    int ishift = atoi(shift.str.c_str());
    return (a << ishift);
}

bigint operator>>(const bigint& a, const bigint& shift)
{
    int ishift = atoi(shift.str.c_str());
    return (a >> ishift);
}

bool operator>(const bigint&a, const bigint& b)
{
    return (a.str.length() > b.str.length() || (a.str.length() == b.str.length() && a.str > b.str));
}
bool operator<(const bigint&a, const bigint& b)
{
    return (a.str.length() < b.str.length() || (a.str.length() == b.str.length() && a.str < b.str));
}
bool operator>=(const bigint&a, const bigint& b)
{
    return (a.str.length() >= b.str.length() || (a.str.length() == b.str.length() && a.str >= b.str));
}
bool operator<=(const bigint&a, const bigint& b)
{
    return (a.str.length() <= b.str.length() || (a.str.length() == b.str.length() && a.str <= b.str));
}
bool operator==(const bigint&a, const bigint& b)
{
    return (a.str == b.str);
}
bool operator!=(const bigint&a, const bigint& b)
{
    return a.str != b.str;
}