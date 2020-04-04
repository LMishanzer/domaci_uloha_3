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
#include "CBigInt.cpp"
using namespace std;

static bool equal ( const CBigInt & x, const char * val )
{
    ostringstream oss;
    oss << x;
    return oss . str () == val;
}

int main ()
{
    istringstream iss;
    iss.clear();
    iss.str("    -1-2fds34afdhkjsd");
//    for (int i = 0; i < 4; i++)
//        cout << (char) iss.get() << endl;
    CBigInt x, y, z;
    assert(iss >> x);
    assert(iss >> y);
    assert(!(iss >> z));
    assert(iss.fail());

    assert(equal(x, "-1"));
    assert(equal(y, "-2"));

    x = "0";
    y = "-0";
    assert(!(x > y));


//    cout << boolalpha << IsNumber("1000") << endl;
//    cout << IsNumber("1023") << endl;
//    cout << IsNumber("+1000") << endl;
//    cout << IsNumber("-1000") << endl;
//    cout << IsNumber("1-000") << endl;
//    cout << IsNumber("100+0") << endl;
//    cout << IsNumber("00001000") << endl;
//    cout << IsNumber("1000dsfs") << endl;
//    cout << IsNumber("sgse1000") << endl;
//    cout << IsNumber("e1000") << endl;


//    CBigInt x;
//    cin >> x;
//    CBigInt y;
//    cin >> y;
//    CBigInt z;
//
//    z = x * y;
//    for (int i = 0; i < 10000; i++)
//        c += a + b + c;

//    cout << "z: " << z << endl;

//    CBigInt d = a * b;
//    CBigInt e = a - b;
//
//    cout << d << endl;

//    cout << "a + b = " << a << endl;
//    cout << "a - b = " << e << endl;
//    cout << boolalpha << "a > b = " << (a > b) << endl;


//    cout << "a: " << a << endl;
//    cout << "d: " << d << endl;
//    cout << boolalpha << "a < d: " << (a < d) << endl;
//    cout << boolalpha << "a <= d: " << (a <= d) << endl;
//    cout << boolalpha << "a > d: " << (a > d) << endl;
//    cout << boolalpha << "a >= d: " << (a >= d) << endl;
//    cout << boolalpha << "b == a: " << (b == a) << endl;
//    cout << boolalpha << "a != d: " << (a != d) << endl;
//
//    cout << boolalpha << "a < b: " << (a < b) << endl;
//    cout << boolalpha << "a <= b: " << (a <= b) << endl;
//    cout << boolalpha << "a > b: " << (a > b) << endl;
//    cout << boolalpha << "a >= b: " << (a >= b) << endl;
//    cout << boolalpha << "b == a: " << (b == a) << endl;
//    cout << boolalpha << "a != b: " << (a != b) << endl;


    CBigInt a, b;
    istringstream is;
    a = 10;
    a += 20;
    assert (equal(a, "30") );
    a *= 5;
    assert (equal(a, "150") );
    b = a + 3;
    assert (equal(b, "153") );
    b = a * 7;
    assert (equal(b, "1050") );
    assert (equal(a, "150") );

    a = 10;
    a += -20;
    assert (equal(a, "-10") );
    a *= 5;
    assert (equal(a, "-50") );
    b = a + 73;
    assert (equal(b, "23") );
    b = a * -7;
    assert (equal(b, "350") );
    assert (equal(a, "-50") );

    a = "12345678901234567890";
    a += "-99999999999999999999";
    assert (equal(a, "-87654321098765432109") );
    a *= "54321987654321987654";
    assert (equal(a, "-4761556948575111126880627366067073182286") );
    a *= 0;
    assert (equal(a, "0") );
    a = 10;
    b = a + "400";
    assert (equal(b, "410") );
    b = a * "15";
    assert (equal(b, "150") );
    assert (equal(a, "10") );

    is . clear ();
    is . str ( " 1234" );
    assert ( is >> b );
    assert (equal(b, "1234") );
    is . clear ();
    is . str ( " 12 34" );
    assert ( is >> b );
    assert (equal(b, "12") );
    is . clear ();
    is . str ( "999z" );
    assert ( is >> b );
    assert (equal(b, "999") );
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
        assert (equal(a, "42") );
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
    is.fail();
    assert(!(is >> a));

    return 0;
}
