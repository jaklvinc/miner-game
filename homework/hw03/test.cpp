#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <climits>
#include <cmath>
#include <cfloat>
#include <cassert>
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#if defined ( __cplusplus ) && __cplusplus > 199711L /* C++ 11 */
#include <memory>
#endif /* C++ 11 */
using namespace std;
#endif /* __PROGTEST__ */

bool cmp_double( double first , double second )
{
  return ( abs(first-second) < 0.1 );
}

class CPolynomial
{
  public:
    // default constructor
    CPolynomial()
    {
      m_Indeces.push_back(0);
    }
    // cmp function
    bool cmp_Polyn (const CPolynomial & first , const CPolynomial & second ) const
    {
      if ( first.m_Indeces.size() != second.m_Indeces.size() )
      {
        if ( first.m_Indeces.size() > second.m_Indeces.size() )
        {
          for ( size_t i = second.m_Indeces.size() ; i < first.m_Indeces.size() ; i++)
          {
            if (abs(first.m_Indeces[i]) < 0.1 )
              return false;
          }
        }
        else
        {
          for ( size_t i = first.m_Indeces.size() ; i < second.m_Indeces.size() ; i++)
          {
            if (abs(second.m_Indeces[i]) < 0.1 )
              return false;
          }
        }
      }

      size_t smaller_size = min ( first.m_Indeces.size() , second.m_Indeces.size() );
      for ( size_t i = 0 ; i < smaller_size ; i++ )
      {
        if ( ! cmp_double ( first.m_Indeces[i] , second.m_Indeces[i] ) )
          return false;
      }

      return true;
    }
    // operator +
    CPolynomial operator + (const CPolynomial & polyn ) const
    {
      CPolynomial NewIndeces;
      NewIndeces.m_Indeces.reserve(max(this->m_Indeces.size(),polyn.m_Indeces.size()));
      for ( size_t i = 0 ; i < NewIndeces.m_Indeces.size() ; i++ )
        NewIndeces.m_Indeces[i] = 0;
      
      for ( size_t i = 0 ; i < this->m_Indeces.size() ; i++ )
      {
        NewIndeces.m_Indeces[i]+=this->m_Indeces[i];
      }
      for ( size_t i = 0 ; i < polyn.m_Indeces.size() ; i++ )
      {
        NewIndeces.m_Indeces[i]+=polyn.m_Indeces[i];
      }

      return NewIndeces;
    }
    // operatnr -
    CPolynomial operator - (const CPolynomial & polyn) const 
    {
      CPolynomial NewIndeces;
      NewIndeces.m_Indeces.reserve(max(this->m_Indeces.size(),polyn.m_Indeces.size()));
      for ( size_t i = 0 ; i < NewIndeces.m_Indeces.size() ; i++ )
        NewIndeces.m_Indeces[i] = 0;
      
      for ( size_t i = 0 ; i < this->m_Indeces.size() ; i++ )
      {
        NewIndeces.m_Indeces[i]+=this->m_Indeces[i];
      }
      for ( size_t i = 0 ; i < polyn.m_Indeces.size() ; i++ )
      {
        NewIndeces.m_Indeces[i]-=polyn.m_Indeces[i];
      }

      return NewIndeces;
    }
    // operator * (polynomial, double)
    CPolynomial operator * (const CPolynomial & polyn) const
    {
      CPolynomial NewIndeces;
      NewIndeces.m_Indeces.reserve(max(this->m_Indeces.size(),polyn.m_Indeces.size()));
      for ( size_t i = 0 ; i < NewIndeces.m_Indeces.size() ; i++ )
        NewIndeces.m_Indeces[i] = 0;
      
      for ( size_t i = 0 ; i < this->m_Indeces.size() ; i++ )
      {
        for ( size_t j = 0 ; j < polyn.m_Indeces.size() ; j++ )
        {
          NewIndeces.m_Indeces[i+j] = this->m_Indeces[i] * polyn.m_Indeces[j];
        }
      }
      
      return NewIndeces;
    }

    CPolynomial operator * (const double multip) const
    {
      CPolynomial NewIndeces;
      NewIndeces.m_Indeces=this->m_Indeces;
      
      for ( size_t i = 0 ; i < this->m_Indeces.size() ; i++ )
      {
        NewIndeces.m_Indeces[i]*=multip;
      }
      return NewIndeces;
    }
    // operator ==
    bool operator == ( const CPolynomial & polyn ) const
    {
      return cmp_Polyn(*this,polyn);
    }
    // operator !=
    bool operator != (const CPolynomial & polyn ) const
    {
      return !cmp_Polyn(*this,polyn);
    }
    // operator []
    double& operator []( size_t position )
    {
      size_t old_size = this->m_Indeces.size()-1;
      if ( position > old_size )
      {
        this->m_Indeces.resize(position);
        for ( size_t i = old_size ; i < this->m_Indeces.size() ; i++ )
          this->m_Indeces[i]=0;
      }
      return this->m_Indeces[position];
    } 
    // operator ()
    double operator ()( int x ) const 
    {
      double result = 0;
      for ( size_t i = 0 ; i < this->m_Indeces.size() ; i++ )
      {
        result+=(this->m_Indeces[i]*pow(x,i));
      }
      return result;
    }
    // Degree (), returns unsigned value
    unsigned int Degree ( void )
    {
      size_t degree;
      for ( size_t i = 0 ; i < this->m_Indeces[i] ; i++ )
        if ( m_Indeces[i] > 0.1 )
          degree = i;
        
      return degree;
    }
    // << operator
    friend ostream &operator << ( ostream & out , const CPolynomial & Polyn )
    {
      bool first_index = true;
      for ( size_t i = Polyn.m_Indeces.size()-1 ; i > 0 ; i--)
      {
        if (abs(Polyn.m_Indeces[i]) > 0.1 )
        {
          if ( first_index )
          {
            if ( Polyn.m_Indeces[i] > 0 )
            {
              if ( Polyn.m_Indeces[i] == 1 )
                out << "x^" << i << " ";
              else
                out << Polyn.m_Indeces[i] << "*x^" << i << " ";
            }
            else 
            {
              if ( Polyn.m_Indeces[i] == -1 )
                out << "- " << "x^" << i << " ";
              else
                out << "- " << -Polyn.m_Indeces[i] << "x^" << i << " ";
            }
          }
          else
          {

          }
        }
      }
      return out;
    }
  
  private:
    vector<double> m_Indeces;

    
};

#ifndef __PROGTEST__
bool               smallDiff                               ( double            a,
                                                             double            b )
{
  return ( abs(a - b) < 0.1 );
}

bool               dumpMatch                               ( const CPolynomial & x,
                                                             const vector<double> & ref )
{
  CPolynomial PolRef;
  for ( size_t i = 0 ; i < ref.size() ; i++ )
    PolRef[i]=ref[i];
  return ( x==PolRef );
}

int                main                                    ( void )
{
  CPolynomial a, b, c;
  ostringstream out;

  a[0] = -10;
  a[1] = 3.5;
  a[3] = 1;
  assert ( smallDiff ( a ( 2 ), 5 ) );
  out . str ("");
  out << a;
  assert ( out . str () == "x^3 + 3.5*x^1 - 10" );
  a = a * -2;
  assert ( a . Degree () == 3
           && dumpMatch ( a, vector<double>{ 20.0, -7.0, -0.0, -2.0 } ) );

  out . str ("");
  out << a;
  assert ( out . str () == "- 2*x^3 - 7*x^1 + 20" );
  out . str ("");
  out << b;
  assert ( out . str () == "0" );
  b[5] = -1;
  out . str ("");
  out << b;
  assert ( out . str () == "- x^5" );
  c = a + b;
  assert ( c . Degree () == 5
           && dumpMatch ( c, vector<double>{ 20.0, -7.0, 0.0, -2.0, 0.0, -1.0 } ) );

  out . str ("");
  out << c;
  assert ( out . str () == "- x^5 - 2*x^3 - 7*x^1 + 20" );
  c = a - b;
  assert ( c . Degree () == 5
           && dumpMatch ( c, vector<double>{ 20.0, -7.0, -0.0, -2.0, -0.0, 1.0 } ) );

  out . str ("");
  out << c;
  assert ( out . str () == "x^5 - 2*x^3 - 7*x^1 + 20" );
  c = a * b;
  assert ( c . Degree () == 8
           && dumpMatch ( c, vector<double>{ -0.0, -0.0, 0.0, -0.0, 0.0, -20.0, 7.0, -0.0, 2.0 } ) );

  out . str ("");
  out << c;
  assert ( out . str () == "2*x^8 + 7*x^6 - 20*x^5" );
  assert ( a != b );
  b[5] = 0;
  assert ( !(a == b) );
  a = a * 0;
  assert ( a . Degree () == 0
           && dumpMatch ( a, vector<double>{ 0.0 } ) );

  assert ( a == b );
  return 0;
}
#endif /* __PROGTEST__ */
