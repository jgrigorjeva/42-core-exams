#ifndef BIGINT
#define BIGINT
#include <iostream>

class bigint
{
    public:
    std::string str;
    
    // constructors
    bigint();
    bigint(unsigned int n);
    bigint(const bigint& other);
    ~bigint();
    bigint &operator+=(const bigint& num);
    bigint &operator++();
    bigint operator++(int);
    bigint operator<<=(const unsigned int& shift);
    bigint operator>>=(const unsigned int& shift);
    bigint operator<<=(const bigint& shift);
    bigint operator>>=(const bigint& shift);



};

std::ostream& operator<<(std::ostream& out, const bigint& src);
bigint operator+(const bigint& a, const bigint& b);
bigint operator<<(const bigint& a, const unsigned int& shift);
bigint operator>>(const bigint& a, const unsigned int& shift);
bigint operator<<(const bigint& a, const bigint& shift);
bigint operator>>(const bigint& a, const bigint& shift);
bool operator>(const bigint&a, const bigint& b);
bool operator<(const bigint&a, const bigint& b);
bool operator>=(const bigint&a, const bigint& b);
bool operator<=(const bigint&a, const bigint& b);
bool operator==(const bigint&a, const bigint& b);
bool operator!=(const bigint&a, const bigint& b);


#endif