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
    explicit CBigInt(const long long &num);
    explicit CBigInt(const string &num);
    CBigInt(const CBigInt &num);

    CBigInt & operator = (const long long &num);
    CBigInt & operator = (const string &num);
    CBigInt & operator = (const CBigInt &num);
    CBigInt operator - () const;

    CBigInt & operator +=(const CBigInt &num);
    CBigInt & operator +=(const long long &num);
    CBigInt & operator +=(const string &num);

    CBigInt & operator*=(const CBigInt &num);
    CBigInt & operator*=(const long long &num);
    CBigInt & operator*=(const string &num);

    friend ostringstream & operator<<(ostringstream & os, const CBigInt & num);
    friend ostream & operator<<(ostream & os, const CBigInt & num);
    friend bool AreEqual(const CBigInt & num1, const CBigInt & num2);
    friend bool IsGreater(const CBigInt & num1, const CBigInt & num2);

    friend CBigInt operator + (const CBigInt &num1, const CBigInt &num2);

    friend CBigInt operator - (const CBigInt &num1, const CBigInt &num2);

    friend CBigInt operator * (const CBigInt &num1, const CBigInt &num2);

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

CBigInt::CBigInt(const long long &num)
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

CBigInt & CBigInt::operator=(const long long & num)
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

bool operator == (const CBigInt & num1, const CBigInt & num2)
{
    return AreEqual(num1, num2);
}
bool operator == (const CBigInt & num1, const long long & num2)
{
    CBigInt temp(num2);
    return AreEqual(num1, temp);
}
bool operator == (const CBigInt & num1, const string & num2)
{
    CBigInt temp(num2);
    return AreEqual(num1, temp);
}
bool operator == (const long long & num1, const CBigInt & num2)
{
    CBigInt temp(num1);
    return AreEqual(temp, num2);
}
bool operator == (const string &num1, const CBigInt & num2)
{
    CBigInt temp(num1);
    return AreEqual(temp, num2);
}

bool operator != (const CBigInt & num1, const CBigInt &num2)
{
    return !AreEqual(num1, num2);
}
bool operator != (const CBigInt & num1, const long long &num2)
{
    CBigInt temp(num2);
    return !AreEqual(num1, temp);
}
bool operator != (const CBigInt & num1, const string &num2)
{
    CBigInt temp(num2);
    return !AreEqual(num1, temp);
}
bool operator != (const long long & num1, const CBigInt &num2)
{
    CBigInt temp(num1);
    return !AreEqual(temp, num2);
}
bool operator != (const string & num1, const CBigInt &num2)
{
    CBigInt temp(num1);
    return !AreEqual(temp, num2);
}

bool operator > (const CBigInt & num1, const CBigInt & num2)
{
    return IsGreater(num1, num2);
}
bool operator > (const CBigInt & num1, const long long & num2)
{
    CBigInt temp(num2);
    return IsGreater(num1, temp);
}
bool operator > (const CBigInt & num1, const string & num2)
{
    CBigInt temp(num2);
    return IsGreater(num1, temp);
}
bool operator > (const long long & num1, const CBigInt & num2)
{
    CBigInt temp(num1);
    return IsGreater(temp, num2);
}
bool operator > (const string & num1, const CBigInt & num2)
{
    CBigInt temp(num1);
    return IsGreater(temp, num2);
}

bool operator >= (const CBigInt & num1, const CBigInt & num2)
{
    return num1 > num2 || num1 == num2;
}
bool operator >= (const CBigInt & num1, const long long & num2)
{
    return num1 > num2 || num1 == num2;
}
bool operator >= (const CBigInt & num1, const string & num2)
{
    return num1 > num2 || num1 == num2;
}
bool operator >= (const long long & num1, const CBigInt & num2)
{
    return num1 > num2 || num1 == num2;
}
bool operator >= (const string & num1, const CBigInt & num2)
{
    return num1 > num2 || num1 == num2;
}

bool operator < (const CBigInt & num1, const CBigInt & num2)
{
    return !(num1 >= num2);
}
bool operator < (const CBigInt & num1, const long long & num2)
{
    return !(num1 >= num2);
}
bool operator < (const CBigInt & num1, const string & num2)
{
    return !(num1 >= num2);
}
bool operator < (const long long & num1, const CBigInt & num2)
{
    return !(num1 >= num2);
}
bool operator < (const string & num1, const CBigInt & num2)
{
    return !(num1 >= num2);
}

bool operator <= (const CBigInt & num1, const CBigInt & num2)
{
    return !(num1 > num2);
}
bool operator <= (const CBigInt & num1, const long long & num2)
{
    return !(num1 > num2);
}
bool operator <= (const CBigInt & num1, const string & num2)
{
    return !(num1 > num2);
}
bool operator <= (const long long & num1, const CBigInt & num2)
{
    return !(num1 > num2);
}
bool operator <= (const string & num1, const CBigInt & num2)
{
    return !(num1 > num2);
}

CBigInt operator + (const CBigInt &num1, const CBigInt &num2)
{
    if (num1.sign && !num2.sign)
        return num1 - (-num2);
    if (!num1.sign && num2.sign)
        return num2 - (-num1);
    if (!num1.sign && !num2.sign)
        return -((-num2) + (-num1));

    if (num2.size > num1.size)
        return num2 + num1;

    string reversed_result;
    bool transmission = false;
    for (unsigned long i = 0; i < num1.size; i++)
    {
        int temp;
        if (num2.size - 1 >= i)
            temp = num1.number[num1.size - 1 - i] + num2.number[num2.size - 1 - i] - 96 + transmission;
        else {
            temp = num1.number[num1.size - 1 - i] - 48 + transmission;
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
CBigInt operator + (const CBigInt &num1, const long long &num2)
{
    CBigInt temp(num2);
    return num1 + temp;
}
CBigInt operator + (const CBigInt &num1, const string &num2)
{
    CBigInt temp(num2);
    return num1 + temp;
}
CBigInt operator + (const long long &num1, const CBigInt &num2)
{
    CBigInt temp(num1);
    return temp + num2;
}
CBigInt operator + (const string &num1, const CBigInt &num2)
{
    CBigInt temp(num1);
    return temp + num2;
}

CBigInt CBigInt::operator - () const
{
    CBigInt res(*this);
    res.sign = !res.sign;
    return res;
}

CBigInt operator - (const CBigInt &num1, const CBigInt &num2)
{
    if (num1 == num2)
        return CBigInt();
    if (num1.sign && !num2.sign)
        return num1 + (-num2);
    if (!num1.sign && num2.sign)
        return -(-num1 + num2);
    if (!num1.sign && !num2.sign)
        return (-num2) - (-num1);

    if (num2 > num1)
        return -(num2 - num1);

    string reversed_result;
    bool transmission = false;
    for (unsigned long i = 0; i < num1.size; i++)
    {
        int temp;
        if (num2.size - 1 >= i)
            temp = num1.number[num1.size - 1 - i] - num2.number[num2.size - 1 - i] - transmission;
        else
            temp = num1.number[num1.size - 1 - i] - 48 - transmission;

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
CBigInt operator - (const CBigInt &num1, const long long &num2)
{
    CBigInt temp(num2);
    return num1 + temp;
}
CBigInt operator - (const CBigInt &num1, const string &num2)
{
    CBigInt temp(num2);
    return num1 + temp;
}
CBigInt operator - (const long long &num1, const CBigInt &num2)
{
    CBigInt temp(num1);
    return temp + num2;
}
CBigInt operator - (const string &num1, const CBigInt &num2)
{
    CBigInt temp(num1);
    return temp + num2;
}

CBigInt & CBigInt::operator+=(const CBigInt &num)
{
    *this = *this + num;
    return *this;
}
CBigInt & CBigInt::operator+=(const long long &num)
{
    CBigInt temp(num);
    *this = *this + temp;
    return *this;
}
CBigInt & CBigInt::operator+=(const string &num)
{
    CBigInt temp(num);
    *this = *this + temp;
    return *this;
}

CBigInt operator * (const CBigInt &num1, const CBigInt &num2)
{
    if (num2.size > num1.size)
        return num2 * num1;

    auto * array = new CBigInt[num2.size];
    for (unsigned long i = 0; i < num2.size; i++)
    {
        int transmission = 0;
        string reversed_result;
        for (unsigned long j = 0; j < num1.size; j++)
        {
            int temp = (num1.number[num1.size - 1 - j] - 48) * (num2.number[num2.size - 1 - i] - 48) + transmission;
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
    for (unsigned long i = 0; i < num2.size; i++)
    {
        result += array[i];
    }
    if (result.number != "0")
        result.sign = !(num1.sign ^ num2.sign);
    else
        result.sign = true;
    result.size = result.number.size();
    delete [] array;
    return result;
}
CBigInt operator * (const CBigInt &num1, const long long &num2)
{
    CBigInt temp(num2);
    return num1 * temp;
}
CBigInt operator * (const CBigInt &num1, const string &num2)
{
    CBigInt temp(num2);
    return num1 * temp;
}
CBigInt operator * (const long long &num1, const CBigInt &num2)
{
    CBigInt temp(num1);
    return temp * num2;
}
CBigInt operator * (const string &num1, const CBigInt &num2)
{
    CBigInt temp(num1);
    return temp * num2;
}

CBigInt & CBigInt::operator*=(const CBigInt &num)
{
    *this = *this * num;
    return *this;
}
CBigInt & CBigInt::operator*=(const long long &num)
{
    CBigInt temp(num);
    *this = *this * temp;
    return *this;
}
CBigInt & CBigInt::operator*=(const string &num)
{
    CBigInt temp(num);
    *this = *this * temp;
    return *this;
}
