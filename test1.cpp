#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <climits>
#include <cstdint>
#include <cassert>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include <algorithm>
#include <memory>
#include <functional>
#include <stdexcept>
using namespace std;
#endif /* __PROGTEST__ */


class CBigInt
{
public:
    CBigInt();
    explicit CBigInt(const int &num);
    explicit CBigInt(const string &num);
    CBigInt(const CBigInt &num);

    CBigInt & operator = (const int &num);
    CBigInt & operator = (const string &num);
    CBigInt & operator = (const CBigInt &num);
    CBigInt operator - ();
    template <class T>
    CBigInt & operator +=(const T &num);
    template <class T>
    CBigInt & operator*=(const T &num);

    friend ostringstream & operator<<(ostringstream & os, const CBigInt & num);
    friend ostream & operator<<(ostream & os, const CBigInt & num);
    friend bool AreEqual(const CBigInt & num1, const CBigInt & num2);
    friend bool IsGreater(const CBigInt & num1, const CBigInt & num2);
    template <class T, class U>
    friend CBigInt operator + (const T &num1, const U &num2);
    template <class T, class U>
    friend CBigInt operator - (const T &num1, const U &num2);
    template <class T, class U>
    friend CBigInt operator * (const T &num1, const U &num2);

    // copying/assignment/destruction
    // operator *, any combination {CBigInt/int/string} * {CBigInt/int/string}
    // operator *=, any of {CBigInt/int/string}
private:
    bool sign;
    unsigned long size;
    string number;
};

bool IsNumber(const string& num)
{
    for (unsigned long i = num.length() - 1; i > 0; i--)
    {
        if (num[i] < '0' || num[i] > '9')
            return false;
    }
    return ((num[0] >= '0' && num[0] <= '9') || num[0] == '+' || num[0] == '-');
}

ostringstream & operator<<(ostringstream & os, const CBigInt & num)
{
    os << (num.sign ? "" : "-") << num.number;
    return os;
}
ostream & operator<<(ostream & os, const CBigInt & num)
{
    os << (num.sign ? "" : "-") << num.number;
    return os;
}

bool operator>>(istringstream & is, CBigInt & num)
{
    string temp;
    char current;
    current = (char) is.get();

    while (current == ' ')
    {
        current = (char) is.get();
    }
    if (current != '+' && current != '-' && (current < '0' || current > '9'))
    {
        is.setstate(std::ios::failbit);
        return false;
    }
    if (current == '+' || current == '-')
    {
        temp += current;
        current = (char) is.get();
    }
    if (current < '1' || current > '9')
    {
        is.setstate(std::ios::failbit);
        return false;
    }
    while (current >= '0' && current <= '9')
    {
        temp += current;
        current = (char) is.get();
    }
    is.unget();

    try {
        num = CBigInt(temp);
    }
    catch (invalid_argument &e)
    {
        is.setstate(std::ios::failbit);
        return false;
    }
    return true;
}

CBigInt::CBigInt()
{
    sign = true;
    number = "0";
    size = 1;
}

CBigInt::CBigInt(const int &num)
{
    sign = (num >= 0);
    if (sign)
        number = to_string(num);
    else
        number = to_string(-num);
    size = number.size();
}

CBigInt::CBigInt(const string& num)
{
    if (!IsNumber(num))
        throw invalid_argument(num);

    bool write = false;
    size = 0;

    sign = num[0] != '-';

    for (char i : num)
    {
        if (i != '0' && i != '+' && i != '-')
            write = true;
        if (write)
        {
            number += i;
            size++;
        }
    }
    if (!write)
    {
        number = "0";
        sign = true;
        size = 1;
    }
}

CBigInt::CBigInt(const CBigInt &num)
{
    sign = num.sign;
    size = num.size;
    for (unsigned long i = 0; i < size; i++)
        number += num.number[i];
}

CBigInt & CBigInt::operator=(const int & num)
{
    num < 0 ? (sign = false) : (sign = true);
    number = to_string(num);
    size = number.size();
    return *this;
}

CBigInt & CBigInt::operator=(const string & num)
{
    if (!IsNumber(num))
        throw invalid_argument(num);

    bool write = false;
    size = 0;

    sign = num[0] != '-';
    number = "";

    for (char i : num)
    {
        if (i != '0' && i != '+' && i != '-')
            write = true;
        if (write)
        {
            number += i;
            size++;
        }
    }
    if (!write)
    {
        number = "0";
        sign = true;
        size = 1;
    }
    return *this;
}

CBigInt & CBigInt::operator = (const CBigInt &num)
= default;

bool AreEqual(const CBigInt & num1, const CBigInt & num2)
{
    if (num1.sign != num2.sign)
        return false;

    return num1.number == num2.number;
}
bool IsGreater(const CBigInt & num1, const CBigInt & num2)
{
    if (AreEqual(num1, num2))
        return false;

    if (num1.sign > num2.sign)
        return true;
    else if (num1.sign < num2.sign)
        return false;

    if (num1.sign)
    {
        if (num1.size > num2.size)
            return true;
        else if (num1.size < num2.size)
            return false;
    }
    else
    {
        if (num1.size < num2.size)
            return true;
        else if (num1.size > num2.size)
            return false;
    }

    if (num1.sign)
    {
        for (int i = 0; true; i++)
        {
            if (num1.number[i] != num2.number[i])
                return num1.number[i] > num2.number[i];
        }
    } else
        for (int i = 0; true; i++)
        {
            if (num1.number[i] != num2.number[i])
                return num1.number[i] < num2.number[i];
        }
}

template <class T, class U>
bool operator == (const T & num1, const U & num2)
{
    CBigInt temp1(num1);
    CBigInt temp2(num2);
    return AreEqual(temp1, temp2);
}
template <class T, class U>
bool operator != (const T & num1, const U & num2)
{
    CBigInt temp1(num1);
    CBigInt temp2(num2);
    return !AreEqual(temp1, temp2);
}
template <class T, class U>
bool operator > (const T & num1, const U & num2)
{
    CBigInt temp1(num1);
    CBigInt temp2(num2);
    return IsGreater(temp1, temp2);
}
template <class T, class U>
bool operator >= (const T & num1, const U & num2)
{
    return num1 > num2 || num1 == num2;
}
template <class T, class U>
bool operator < (const T & num1, const U & num2)
{
    return !(num1 >= num2);
}
template <class T, class U>
bool operator <= (const T & num1, const U & num2)
{
    return !(num1 > num2);
}

template <class T, class U>
CBigInt operator + (const T &num1, const U &num2)
{
    CBigInt temp1(num1);
    CBigInt temp2(num2);
    if (temp1.sign && !temp2.sign)
        return temp1 - (-temp2);
    if (!temp1.sign && temp2.sign)
        return temp2 - (-temp1);
    if (!temp1.sign && !temp2.sign)
        return -((-temp2) + (-temp1));

    if (temp2.size > temp1.size)
        return temp2 + temp1;

    string reversed_result;
    bool transmission = false;
    for (unsigned long i = 0; i < temp1.size; i++)
    {
        int temp;
        if (temp2.size - 1 >= i)
            temp = temp1.number[temp1.size - 1 - i] + temp2.number[temp2.size - 1 - i] - 96 + transmission;
        else {
            temp = temp1.number[temp1.size - 1 - i] - 48 + transmission;
        }

        if (temp >= 10)
        {
            temp -= 10;
            transmission = true;
        }
        else
            transmission = false;

        reversed_result += (char) (temp + 48);
    }
    if (transmission)
        reversed_result += '1';

    string result;
    unsigned long size = reversed_result.size();
    for (unsigned long i = 0; i < size; i++)
    {
        result += reversed_result[size - 1 - i];
    }
    return CBigInt(result);
}

CBigInt CBigInt::operator - ()
{
    CBigInt res(*this);
    res.sign = !res.sign;
    return res;
}

template <class T, class U>
CBigInt operator - (const T &num1, const U &num2)
{
    CBigInt temp1(num1);
    CBigInt temp2(num2);
    if (temp1 == temp2)
        return CBigInt();
    if (temp1.sign && !temp2.sign)
        return temp1 + (-temp2);
    if (!temp1.sign && temp2.sign)
        return -(-temp1 + temp2);
    if (!temp1.sign && !temp2.sign)
        return (-temp2) - (-temp1);

    if (temp2 > temp1)
        return -(temp2 - temp1);

    string reversed_result;
    bool transmission = false;
    for (unsigned long i = 0; i < temp1.size; i++)
    {
        int temp;
        if (temp2.size - 1 >= i)
            temp = temp1.number[temp1.size - 1 - i] - temp2.number[temp2.size - 1 - i] - transmission;
        else
            temp = temp1.number[temp1.size - 1 - i] - 48 - transmission;

        if (temp < 0)
        {
            temp += 10;
            transmission = true;
        }
        else
            transmission = false;

        reversed_result += (char) (temp + 48);
    }

    string result;
    unsigned long size = reversed_result.size();
    for (unsigned long i = 0; i < size; i++)
    {
        result += reversed_result[size - 1 - i];
    }
    return CBigInt(result);
}

template <class T>
CBigInt & CBigInt::operator+=(const T &num)
{
    *this = *this + num;
    return *this;
}

template <class T, class U>
CBigInt operator * (const T &num1, const U &num2)
{
    CBigInt temp1(num1);
    CBigInt temp2(num2);
    if (temp2.size > temp1.size)
        return temp2 * temp1;

    auto * array = new CBigInt[temp2.size];
    for (unsigned long i = 0; i < temp2.size; i++)
    {
        int transmission = 0;
        string reversed_result;
        for (unsigned long j = 0; j < temp1.size; j++)
        {
            int temp = (temp1.number[temp1.size - 1 - j] - 48) * (temp2.number[temp2.size - 1 - i] - 48) + transmission;
            transmission = 0;
            while (temp >= 10)
            {
                temp -= 10;
                transmission++;
            }
            reversed_result += (char) (temp + 48);
        }
        if (transmission > 0)
        {
            reversed_result += to_string(transmission);
        }

        string result;
        unsigned long size = reversed_result.size();
        for (unsigned long k = 0; k < size; k++)
        {
            result += reversed_result[size - 1 - k];
        }
        for (unsigned long l = 0; l < i; l++)
            result += '0';
        array[i] = CBigInt(result);
    }
    CBigInt result;
    for (unsigned long i = 0; i < temp2.size; i++)
    {
        result += array[i];
    }
    if (result.number != "0")
        result.sign = !(temp1.sign ^ temp2.sign);
    else
        result.sign = true;
    result.size = result.number.size();
    delete [] array;
    return result;
}

template <class T>
CBigInt & CBigInt::operator*=(const T &num)
{
    *this = *this * num;
    return *this;
}
