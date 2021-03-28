#ifndef __PROGTEST__
#include <iostream>
#include <iomanip>
#include <string>
#include <stdexcept>
using namespace std;

class CTimeTester;
#endif /* __PROGTEST__ */

#define DAY_SEC 86400

class CTime
{
private:
    int m_Hour;
    int m_Minute;
    int m_Second;

public:
    // constructor, destructor
    CTime() {}
    CTime(int hour, int minute)
    {
        if ( hour > 23 || hour < 0 || minute > 59 || minute < 0 )
            throw invalid_argument("Hour/minute not valid!");
        m_Hour = hour;
        m_Minute = minute;
        m_Second = 0;
    }

    CTime(int hour, int minute , int second )
    {
        if ( hour > 23 || hour < 0 || minute > 59 || minute < 0 || second > 59 || second < 0 )
            throw invalid_argument("Hour/minute/second not valid!");
        m_Hour = hour;
        m_Minute = minute;
        m_Second = second;
    }

    // arithmetic operators
    friend CTime add ( const CTime & time , const int plus_sec );

    friend int convert ( const CTime & time );

    CTime operator + ( const int & sec ) const
    {
        return add(*this,sec);
    }

    friend CTime operator + ( const int & sec , const CTime & time )
    {
        return add(time,sec);
    }

    CTime operator += ( const int & sec )
    {
        *this = add(*this,sec);
        return *this;
    }

    CTime operator - ( const int & sec ) const
    {
        return add(*this,-sec);
    }

    CTime operator -= ( const int & sec )
    {
        *this = add(*this,-sec);
        return *this;
    }

    int operator - ( const CTime time ) const
    {
        int first_time=convert(*this);
        int second_time=convert(time);
        int first_try = (first_time-second_time);
        if (first_try < 0)
            first_try+=DAY_SEC;
        
        int second_try = (second_time-first_time);
        if (second_try < 0)
            second_try+=DAY_SEC;
        return min(first_try,second_try);
    }

    CTime & operator ++ ( void )
    {
        *this = add( *this , 1 );
        return *this;
    }

    CTime operator ++ ( int nic )
    {
        CTime to_return = *this;
        *this = add( *this , 1 );
        return to_return; 
    }

    CTime & operator -- ( void )
    {
        *this = add( *this , -1 );
        return *this;
    }

    CTime operator -- ( int nic )
    {
        CTime to_return = *this;
        *this = add( *this , -1 );
        return to_return; 
    }

    // comparison operators

    friend int cmp( const CTime & first , const CTime & second );

    bool operator == ( const CTime & time ) const
    {
        return convert(*this) == convert( time );
    }

    bool operator != ( const CTime & time ) const
    {
        return convert(*this) != convert( time );
    }

    bool operator > ( const CTime & time ) const
    {
        return convert(*this) > convert( time );
    }

    bool operator >= ( const CTime & time ) const
    {
        return convert(*this) >= convert( time );
    }

    bool operator < ( const CTime & time ) const
    {
        return convert(*this) < convert( time );
    }

    bool operator <= ( const CTime & time ) const
    {
        return convert(*this) <= convert( time );
    }

    // in/out operators

    friend ostream &operator << ( ostream & out , const CTime & time )
    {
        out << setw(2) << setfill(' ') << time.m_Hour << ":";
        out << setw(2) << setfill('0') << time.m_Minute << ":";
        out << setw(2) << setfill('0') << time.m_Second;
        return out;
    }

    friend istream &operator >> ( istream &in , CTime & time )
    {
        int hour, minute, second;
        char div1 ,div2;
        in >> hour >> noskipws >> div1 >> minute >> div2 >> second;
        
        if (hour > 23 || hour < 0 || 
            minute > 59 || minute < 0 ||
            second > 59 || second < 0 ||
            div1 != ':' || div2 != ':'||
            !in.eof())
            in.setstate(ios::failbit);
        
        time.m_Hour=hour;
        time.m_Minute=minute;
        time.m_Second=second;

        return in;
    }
    friend class ::CTimeTester;
};

CTime add ( const CTime & time , const int plus_sec )
{
    CTime new_time;
    new_time = time;

    new_time.m_Second+=plus_sec;

    while (new_time.m_Second > 59 )
    {
        new_time.m_Minute++;
        new_time.m_Second-=60;
        if ( new_time.m_Minute > 59 )
        {
            new_time.m_Hour++;
            new_time.m_Minute-=60;
            new_time.m_Hour%=24;
        }
    }

    while (new_time.m_Second < 0)
    {
        new_time.m_Minute--;
        new_time.m_Second+=60;
        if ( new_time.m_Minute < 0 )
        {
            new_time.m_Hour--;
            new_time.m_Minute+=60;
            if ( new_time.m_Hour < 0)
                new_time.m_Hour+=24;
        }
    }

    return new_time;
}

int convert ( const CTime & time )
{
    int new_int = 0;
    new_int+=time.m_Second;
    new_int+=(time.m_Minute*60);
    new_int+=(time.m_Hour*3600);
    return new_int;
}

/*int cmp ( const CTime & first , const CTime & second ) // 0 if equal , 1 if first biger , -1 if second bigger
{
    int first_sec = convert(first);
    int second_sec = convert(second);
    if ( first_sec == second_sec )
}*/


#ifndef __PROGTEST__
struct CTimeTester
{
    static bool test ( const CTime & time, int hour, int minute, int second )
    {
        return time.m_Hour == hour 
            && time.m_Minute == minute
            && time.m_Second == second;
    }
};

#include <cassert>
#include <sstream>

int main ()
{
    CTime a { 12, 30 };
    assert( CTimeTester::test( a, 12, 30, 0 ) );

    CTime b;
    std::istringstream input { "13:30:00" };
    assert( static_cast<bool>( input >> b ) );
    assert( CTimeTester::test( b, 13, 30, 0 ) );

    assert( b - a == 3600 );

    assert( CTimeTester::test( a + 60, 12, 31, 0 ) );
    assert( CTimeTester::test( a - 60, 12, 29, 0 ) );

    assert( a < b );
    assert( a <= b );
    assert( a != b );
    assert( !( a > b ) );
    assert( !( a >= b ) );
    assert( !( a == b ) );

    while ( ++a != b );
    assert( a == b );

    std::ostringstream output;
    assert( static_cast<bool>( output << a ) );
    assert( output.str() == "13:30:00" );

    assert( a++ == b++ );
    assert( a == b );

    assert( --a == --b );
    assert( a == b );

    assert( a-- == b-- );
    assert( a == b );

    return 0;
}
#endif /* __PROGTEST__ */
