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

    template <class T>
    CBigInt & operator +=(const T &num);
    template <class T>
    CBigInt & operator*=(const T &num);

    friend ostringstream & operator<<(ostringstream & os, const CBigInt & num);
//    friend ostream & operator<<(ostream & os, const CBigInt & num);

    template <class T>
    bool operator == (const T &num) const;
    template <class T>
    bool operator != (const T &num) const;
    template <class T>
    bool operator > (const T &num) const;
    template <class T>
    bool operator < (const T &num) const;
    template <class T>
    bool operator >= (const T &num) const;
    template <class T>
    bool operator <= (const T &num) const;

    template <class T>
    CBigInt operator + (const T &num) const;
    template <class T>
    CBigInt operator - (const T &num) const;
    template <class T>
    CBigInt operator * (const T &num) const;

    CBigInt operator - () const;

private:
    bool sign;
    unsigned long size;
    string number;

    bool AreEqual(const CBigInt & num) const;
    bool IsGreater(const CBigInt & num) const;
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
//ostream & operator<<(ostream & os, const CBigInt & num)
//{
//    os << (num.sign ? "" : "-") << num.number;
//    return os;
//}

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

bool CBigInt::AreEqual(const CBigInt & num) const
{
    if (sign != num.sign)
        return false;

    return number == num.number;
}
bool CBigInt::IsGreater(const CBigInt & num) const
{
    if (this->AreEqual(num))
        return false;

    if (sign > num.sign)
        return true;
    else if (sign < num.sign)
        return false;

    if (sign)
    {
        if (size > num.size)
            return true;
        else if (size < num.size)
            return false;
    }
    else
    {
        if (size < num.size)
            return true;
        else if (size > num.size)
            return false;
    }

    if (sign)
    {
        for (int i = 0; true; i++)
        {
            if (number[i] != num.number[i])
                return number[i] > num.number[i];
        }
    } else
        for (int i = 0; true; i++)
        {
            if (number[i] != num.number[i])
                return number[i] < num.number[i];
        }
}

template <class T>
bool CBigInt::operator == (const T & num) const
{
    return this->AreEqual(CBigInt(num));
}
template <class T>
bool CBigInt::operator != (const T & num) const
{
    CBigInt temp(num);
    return !this->AreEqual(temp);
}
template <class T>
bool CBigInt::operator > (const T & num) const
{
    return this->IsGreater(CBigInt(num));
}
template <class T>
bool CBigInt::operator >= (const T & num) const
{
    return *this > num || *this == num;
}
template <class T>
bool CBigInt::operator < (const T & num) const
{
    return !(*this >= num);
}
template <class T>
bool CBigInt::operator <= (const T & num) const
{
    return !(*this > num);
}

template <class T>
CBigInt CBigInt::operator + (const T &num) const
{
    CBigInt temp(num);
    if (sign && !temp.sign)
        return *this - (-temp);
    if (!sign && temp.sign)
        return temp - (-(*this));
    if (!sign && !temp.sign)
        return -((-temp) + (-(*this)));

    if (temp.size > size)
        return temp + (*this);

    string reversed_result;
    bool transmission = false;
    for (unsigned long i = 0; i < size; i++)
    {
        int current = 0;
        if (temp.size - 1 >= i)
            current = number[size - 1 - i] + temp.number[temp.size - 1 - i] - 96 + transmission;
        else {
            current = number[size - 1 - i] - 48 + transmission;
        }

        if (current >= 10)
        {
            current -= 10;
            transmission = true;
        }
        else
            transmission = false;

        reversed_result += (char) (current + 48);
    }

    if (transmission)
        reversed_result += '1';

    string result;
    unsigned long reversed_result_size = reversed_result.size();
    for (unsigned long i = 0; i < reversed_result_size; i++)
    {
        result += reversed_result[reversed_result_size - 1 - i];
    }
    return CBigInt(result);
}

CBigInt CBigInt::operator - () const
{
    CBigInt res(*this);
    res.sign = !res.sign;
    return res;
}

template <class T>
CBigInt CBigInt::operator - (const T &num) const
{
    CBigInt temp(num);
    if (*this == temp)
        return CBigInt();
    if (sign && !temp.sign)
        return *this + (-temp);
    if (!sign && temp.sign)
        return -(-(*this) + temp);
    if (!sign && !temp.sign)
        return (-temp) - (-(*this));

    if (temp > *this)
        return -(temp - *this);

    string reversed_result;
    bool transmission = false;
    for (unsigned long i = 0; i < size; i++)
    {
        int current_number;
        if (temp.size - 1 >= i)
            current_number = number[size - 1 - i] - temp.number[temp.size - 1 - i] - transmission;
        else
            current_number = number[size - 1 - i] - 48 - transmission;

        if (current_number < 0)
        {
            current_number += 10;
            transmission = true;
        }
        else
            transmission = false;

        reversed_result += (char) (current_number + 48);
    }

    string result;
    unsigned long reversed_result_size = reversed_result.size();
    for (unsigned long i = 0; i < reversed_result_size; i++)
    {
        result += reversed_result[reversed_result_size - 1 - i];
    }
    return CBigInt(result);
}

template <class T>
CBigInt & CBigInt::operator+=(const T &num)
{
    *this = *this + num;
    return *this;
}

template <class T>
CBigInt CBigInt::operator * (const T &num) const
{
    CBigInt temp(num);
    if (temp.size > size)
        return temp * (*this);

    auto * array = new CBigInt[temp.size];
    for (unsigned long i = 0; i < temp.size; i++)
    {
        int transmission = 0;
        string reversed_result;
        for (unsigned long j = 0; j < size; j++)
        {
            int current_number = (number[size - 1 - j] - 48) * (temp.number[temp.size - 1 - i] - 48) + transmission;
            transmission = 0;
            while (current_number >= 10)
            {
                current_number -= 10;
                transmission++;
            }
            reversed_result += (char) (current_number + 48);
        }
        if (transmission > 0)
        {
            reversed_result += to_string(transmission);
        }

        string result;
        unsigned long reversed_result_size = reversed_result.size();
        for (unsigned long k = 0; k < reversed_result_size; k++)
        {
            result += reversed_result[reversed_result_size - 1 - k];
        }
        for (unsigned long l = 0; l < i; l++)
            result += '0';
        array[i] = CBigInt(result);
    }
    CBigInt result;
    for (unsigned long i = 0; i < temp.size; i++)
    {
        result += array[i];
    }
    if (result.number != "0")
        result.sign = !(sign ^ temp.sign);
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

#ifndef __PROGTEST__
static bool equal ( const CBigInt & x, const char * val )
{
  ostringstream oss;
  oss << x;
  return oss . str () == val;
}
int main ()
{
  CBigInt a, b;
  istringstream is;
  a = 10;
  a += 20;
  assert ( equal ( a, "30" ) );
  a *= 5;
  assert ( equal ( a, "150" ) );
  b = a + 3;
  assert ( equal ( b, "153" ) );
  b = a * 7;
  assert ( equal ( b, "1050" ) );
  assert ( equal ( a, "150" ) );

  a = 10;
  a += -20;
  assert ( equal ( a, "-10" ) );
  a *= 5;
  assert ( equal ( a, "-50" ) );
  b = a + 73;
  assert ( equal ( b, "23" ) );
  b = a * -7;
  assert ( equal ( b, "350" ) );
  assert ( equal ( a, "-50" ) );

  a = "12345678901234567890";
  a += "-99999999999999999999";
  assert ( equal ( a, "-87654321098765432109" ) );
  a *= "54321987654321987654";
  assert ( equal ( a, "-4761556948575111126880627366067073182286" ) );
  a *= 0;
  assert ( equal ( a, "0" ) );
  a = 10;
  b = a + "400";
  assert ( equal ( b, "410" ) );
  b = a * "15";
  assert ( equal ( b, "150" ) );
  assert ( equal ( a, "10" ) );

  is . clear ();
  is . str ( " 1234" );
  assert ( is >> b );
  assert ( equal ( b, "1234" ) );
  is . clear ();
  is . str ( " 12 34" );
  assert ( is >> b );
  assert ( equal ( b, "12" ) );
  is . clear ();
  is . str ( "999z" );
  assert ( is >> b );
  assert ( equal ( b, "999" ) );
  is . clear ();
  is . str ( "abcd" );
  assert ( ! ( is >> b ) );
  is . clear ();
  is . str ( "- 758" );
  assert ( ! ( is >> b ) );
  a = 42;
  try
  {
    a = "-xyz";
    assert ( "missing an exception" == NULL );
  }
  catch ( const invalid_argument & e )
  {
    assert ( equal ( a, "42" ) );
  }

  a = "73786976294838206464";
  assert ( a < "1361129467683753853853498429727072845824" );
  assert ( a <= "1361129467683753853853498429727072845824" );
  assert ( ! ( a > "1361129467683753853853498429727072845824" ) );
  assert ( ! ( a >= "1361129467683753853853498429727072845824" ) );
  assert ( ! ( a == "1361129467683753853853498429727072845824" ) );
  assert ( a != "1361129467683753853853498429727072845824" );
  assert ( ! ( a < "73786976294838206464" ) );
  assert ( a <= "73786976294838206464" );
  assert ( ! ( a > "73786976294838206464" ) );
  assert ( a >= "73786976294838206464" );
  assert ( a == "73786976294838206464" );
  assert ( ! ( a != "73786976294838206464" ) );
  assert ( a < "73786976294838206465" );
  assert ( a <= "73786976294838206465" );
  assert ( ! ( a > "73786976294838206465" ) );
  assert ( ! ( a >= "73786976294838206465" ) );
  assert ( ! ( a == "73786976294838206465" ) );
  assert ( a != "73786976294838206465" );
  a = "2147483648";
  assert ( ! ( a < -2147483648 ) );
  assert ( ! ( a <= -2147483648 ) );
  assert ( a > -2147483648 );
  assert ( a >= -2147483648 );
  assert ( ! ( a == -2147483648 ) );
  assert ( a != -2147483648 );

  return 0;
}
#endif /* __PROGTEST__ */
