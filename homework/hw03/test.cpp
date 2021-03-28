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

pair<double,double> complex_multiply ( pair<double,double> first , pair<double,double> second )
{
  return make_pair( (first.first * second.first)-(first.second * second.second) , (first.first * second.second) + (first.second * second.first) );
}

pair<double,double> complex_plus ( pair<double,double> first , pair<double,double> second )
{
  return make_pair( (first.first + second.first) , (first.second + second.second) );
}

pair<double,double> complex_minus ( pair<double,double> first , pair<double,double> second )
{
  return make_pair( (first.first - second.first) , (first.second - second.second) );
}


// inspired by https://www.geeksforgeeks.org/fast-fourier-transformation-poynomial-multiplication/
vector<pair<double,double>> FFT ( const vector<pair<double,double>> & transform_me , bool reverse )
{
  int size = transform_me.size();
  if (size == 1)
    return transform_me;
  
  // omega = e^((2*pi*i)/n)
  vector<pair<double,double>> omega(size);
  if ( !reverse )
  {
    for ( int i = 0 ; i < size ; i++ )
    {
      double alpha = -2 * M_PI * i / size;
      omega[i].first=cos(alpha); 
      omega[i].second=sin(alpha);
    } 
  }
  else
  {
    for ( int i = 0 ; i < size ; i++ )
    {
      double alpha = -2 * M_PI * i / size;
      omega[i].first=cos(alpha); 
      omega[i].second=-sin(alpha);
    } 
  }
  

  vector<pair<double,double>> Pe(size / 2) , Po(size / 2);
  for (int i = 0; i < size / 2; i++) 
  {
    Pe[i] = transform_me[i * 2];
    Po[i] = transform_me[i * 2 + 1];
  }

  vector<pair<double,double>> Ye = FFT ( Pe , reverse );
  vector<pair<double,double>> Yo = FFT ( Po , reverse );

  vector<pair<double,double>> Y(size);

  for (int k = 0; k < size / 2; k++) {
        Y[k] = complex_plus ( Ye[k] , complex_multiply( omega[k] , Yo[k]) ) ;
        Y[k + size / 2] = complex_minus ( Ye[k] , complex_multiply (omega[k] , Yo[k]) );
    }
  return Y;
}


class CPolynomial
{
  public:
    // default constructor
    CPolynomial()
    {
      m_Indeces.push_back(0);
    }
    // polynomial multiplication
    CPolynomial multiply( const CPolynomial & first , const CPolynomial & second ) const
    {
      vector<pair<double,double>> complex_first;
      vector<pair<double,double>> complex_second;

      for ( auto i : first.m_Indeces )
        complex_first.push_back(make_pair(i,0));
      
      for ( auto i : second.m_Indeces )
        complex_second.push_back(make_pair(i,0));

      size_t new_degree = first.Degree() + second.Degree() + 1 ;
      while ( (new_degree & (new_degree-1)) != 0)
        new_degree++;

      while ( complex_first.size() < new_degree )
        complex_first.push_back(make_pair(0,0));
      
      while ( complex_second.size() < new_degree )
        complex_second.push_back(make_pair(0,0));
      
      vector<pair<double,double>> values_first = FFT ( complex_first , false ) ;
      vector<pair<double,double>> values_second = FFT ( complex_second , false ) ;
      vector<pair<double,double>> values_combined(values_first.size());
      for ( size_t i = 0 ; i < values_combined.size() ; i++ )
        values_combined[i] = complex_multiply ( values_first[i] , values_second[i] );
      
      vector<pair<double,double>> result = FFT ( values_combined , true ) ;
     //for ( size_t i = 0 ; i < result.size() ; i++ )
      //  result[i].first /= result.size();

      CPolynomial ret;
      ret.m_Indeces.resize(result.size());
      for ( size_t i = 0 ; i < result.size() ; i++ )
        ret.m_Indeces[i] = result[i].first / result.size();
        //ret.m_Indeces[i] = abs(result[i].first / result.size()) > 0.1 ? result[i].first / result.size() : 0 ;
      
      return ret;
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
            if (abs(first.m_Indeces[i]) > 0.1 )
              return false;
          }
        }
        else
        {
          for ( size_t i = first.m_Indeces.size() ; i < second.m_Indeces.size() ; i++)
          {
            if (abs(second.m_Indeces[i]) > 0.1 )
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
      NewIndeces.m_Indeces.resize(max(this->m_Indeces.size(),polyn.m_Indeces.size()));
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
      NewIndeces.m_Indeces.resize(max(this->m_Indeces.size(),polyn.m_Indeces.size()));
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
      return multiply(*this,polyn);
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
        this->m_Indeces.resize(position+1);
        for ( size_t i = old_size+1 ; i < this->m_Indeces.size() ; i++ )
          this->m_Indeces[i]=0;
      }
      return this->m_Indeces[position];
    }

    double operator [] (size_t position) const
    {
      return this->m_Indeces[position];
    } 
    // operator ()
    double operator ()( const double x ) const 
    {
      double result = 0;
      for ( size_t i = 0 ; i < this->m_Indeces.size() ; i++ )
      {
        result+=(this->m_Indeces[i]*pow(x,i));
      }
      return result;
    }
    // Degree (), returns unsigned value
    unsigned int Degree ( void ) const
    {
      size_t degree = 0;
      for ( size_t i = 0 ; i < this->m_Indeces.size() ; i++ )
      {
        if ( abs(this->m_Indeces[i]) > 0.1 )
          degree = i;
      }
      return degree;
    }
    // << operator
    friend ostream &operator << ( ostream & out , const CPolynomial & Polyn )
    {
      bool only_zeros= true;
      for ( auto i : Polyn.m_Indeces )
      {
        if ( abs(i) >= 0.1 )
        {
          only_zeros = false;
        }
      }
      if ( !only_zeros )
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
                out << "x^" << i ;
              else
                out << Polyn.m_Indeces[i] << "*x^" << i ;
            }
            else 
            {
              if ( Polyn.m_Indeces[i] == -1 )
                out << "- " << "x^" << i ;
              else
                out << "- " << -Polyn.m_Indeces[i] << "*x^" << i ;
            }
            first_index = false;
          }
          else
          {
            if ( Polyn.m_Indeces[i] > 0 )
            {
              if ( Polyn.m_Indeces[i] == 1 )
                out << " + x^" << i;
              else
                out << " + " << Polyn.m_Indeces[i] << "*x^" << i;
            }
            else 
            {
              if ( Polyn.m_Indeces[i] == -1 )
                out << " - " << "x^" << i;
              else
                out << " - " << -Polyn.m_Indeces[i] << "*x^" << i;
            }
          }
        }
      }
      if ( abs(Polyn.m_Indeces[0]) > 0.1 )
      {
        if ( Polyn.m_Indeces[0] > 0 )
        {
          out << " + " << Polyn.m_Indeces[0];
        }
        else
        {
          out << " - " << -Polyn.m_Indeces[0];
        }
      }
      }
      else
      out << 0;
      return out;
    }
  
  private:
    vector<double> m_Indeces;
    vector<double> m_Complex_Indeces;

    
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
  CPolynomial a, b, c , x , y;
  ostringstream out;

  y[0] = 2;
  y[1] = 4;
  y[2] = 9;
  x = x * y;
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
