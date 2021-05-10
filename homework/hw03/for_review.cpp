/*
* This is a program that can create and store polynomials.
* It can also perform different actions with polynomials such as addition and mutiplication.
* 
* Polynomials are stored as a class CPolynomial and the class uses operator overloading for the sake of easier use
* of the user of the class. 
*
* Each polynomial is stored from the lowest to the highest power and can be shown using the overloaded << operator.
*/

/*
*Sometimes i don't use {} after if else statements. This is for beter code orientation and at places, where
* the code isn't likely to be changed
*/

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
#if defined(__cplusplus) && __cplusplus > 199711L /* C++ 11 */
#include <memory>
#endif /* C++ 11 */
using namespace std;
#endif /* __PROGTEST__ */

// Compares two doubles with 0.1 accuracy
bool Cmp_Double(double first, double second)
{
  return (abs(first - second) < 0.1);
}

class CPolynomial
{
public:
  // default constructor
  CPolynomial()
  {
    m_Indeces.push_back(0);
  }

  //compares two Polynomial and returns if they are equal
  bool Cmp_Polyn(const CPolynomial &first, const CPolynomial &second) const
  {
    if (first.m_Indeces.size() != second.m_Indeces.size())
    {
      if (first.m_Indeces.size() > second.m_Indeces.size())
      {
        if (!Check_Diff_Size(first, second.m_Indeces.size()))
        {
          return false;
        }
      }
      else
      {
        if (!Check_Diff_Size(second, first.m_Indeces.size()))
        {
          return false;
        }
      }
    }
    size_t smaller_size = min(first.m_Indeces.size(), second.m_Indeces.size());
    for (size_t i = 0; i < smaller_size; i++)
    {
      if (!Cmp_Double(first.m_Indeces[i], second.m_Indeces[i]))
      {
        return false;
      }
    }
    return true;
  }

  // adds or substracts two polynomials
  CPolynomial add(const CPolynomial &first, const CPolynomial &second, char sign) const
  {
    CPolynomial NewIndeces;
    NewIndeces.m_Indeces.resize(max(first.m_Indeces.size(), second.m_Indeces.size()), 0);

    for (size_t i = 0; i < first.m_Indeces.size(); i++)
    {
      NewIndeces.m_Indeces[i] += first.m_Indeces[i];
    }

    for (size_t i = 0; i < second.m_Indeces.size(); i++)
    {
      if (sign == '+')
        NewIndeces.m_Indeces[i] += second.m_Indeces[i];
      else
        NewIndeces.m_Indeces[i] -= second.m_Indeces[i];
    }
    return NewIndeces;
  }

  // multiplies two polynomials
  CPolynomial multiply(const CPolynomial &first, const CPolynomial &second) const
  {
    CPolynomial NewIndeces;
    NewIndeces.m_Indeces.resize(first.m_Indeces.size() + second.m_Indeces.size(), 0);

    for (size_t i = 0; i < first.m_Indeces.size(); i++)
    {
      for (size_t j = 0; j < second.m_Indeces.size(); j++)
      {
        NewIndeces.m_Indeces[i + j] += first.m_Indeces[i] * second.m_Indeces[j];
      }
    }
    return NewIndeces;
  }

  /*-----------------------------------------------------------------------------------------------
    *----------------------OPERATOR OVERLOADING STARTS HERE------------------------------------------
    *------------------------------------------------------------------------------------------------
    */

  // operator +
  CPolynomial operator+(const CPolynomial &polyn) const
  {
    return add(*this, polyn, '+');
  }
  // operator -
  CPolynomial operator-(const CPolynomial &polyn) const
  {
    return add(*this, polyn, '-');
  }
  // operator * (polynomial, polynomial)
  CPolynomial operator*(const CPolynomial &polyn) const
  {
    return multiply(*this, polyn);
  }
  // operator * (polynomial , double)
  CPolynomial operator*(const double multip) const
  {
    CPolynomial NewIndeces;
    NewIndeces.m_Indeces = this->m_Indeces;

    for (size_t i = 0; i < this->m_Indeces.size(); i++)
    {
      NewIndeces.m_Indeces[i] *= multip;
    }
    return NewIndeces;
  }
  // operator ==
  bool operator==(const CPolynomial &polyn) const
  {
    return Cmp_Polyn(*this, polyn);
  }
  // operator !=
  bool operator!=(const CPolynomial &polyn) const
  {
    return !Cmp_Polyn(*this, polyn);
  }
  // operator []
  double &operator[](size_t position)
  {
    size_t old_size = this->m_Indeces.size() - 1;
    if (position > old_size)
    {
      this->m_Indeces.resize(position + 1, 0);
    }
    return this->m_Indeces[position];
  }
  // operator [] for read only
  double operator[](size_t position) const
  {
    return this->m_Indeces[position];
  }
  // operator () that returns value of the polyomial at a certain point
  double operator()(const double x) const
  {
    double result = 0;
    for (size_t i = 0; i < this->m_Indeces.size(); i++)
    {
      result += (this->m_Indeces[i] * pow(x, i));
    }
    return result;
  }
  // Degree (), returns unsigned value of the degree od the polynomial
  unsigned int Degree(void) const
  {
    size_t degree = 0;
    for (size_t i = 0; i < this->m_Indeces.size(); i++)
    {
      if (abs(this->m_Indeces[i]) > 0.1)
      {
        degree = i;
      }
    }
    return degree;
  }
  // << operator
  friend ostream &operator<<(ostream &out, const CPolynomial &Polyn)
  {
    bool only_zeros = true;
    for (auto i : Polyn.m_Indeces)
    {
      if (abs(i) >= 0.1)
      {
        only_zeros = false;
      }
    }
    if (!only_zeros)
    {
      bool first_index = true;
      for (size_t i = Polyn.m_Indeces.size() - 1; i > 0; i--)
      {
        if (abs(Polyn.m_Indeces[i]) > 0.1) // only prints the value if its bigger than 0
        {
          if (Polyn.m_Indeces[i] > 0)
          {
            if (!first_index)
              out << " + ";
            
            if (Polyn.m_Indeces[i] == 1)
              out << "x^" << i;
            else
              out << Polyn.m_Indeces[i] << "*x^" << i;
          }
          else
          {
            if (!first_index)
              out << " ";
            
            if (Polyn.m_Indeces[i] == -1)
              out << "- "
                  << "x^" << i;
            else
              out << "- " << -Polyn.m_Indeces[i] << "*x^" << i;
          }
          first_index = false;
        }
      }
      if (abs(Polyn.m_Indeces[0]) > 0.1)
      {
        if (Polyn.m_Indeces[0] > 0)
          out << " + " << Polyn.m_Indeces[0];
        else
          out << " - " << -Polyn.m_Indeces[0];
      }
    }
    else // the polynomial is 0
    {
      out << 0;
    }
    return out;
  }

private:
  vector<double> m_Indeces; //multiplication indeces of each power

  //used inside the Cmp_Polyn function, when one polynomial is bigger in size, than another one
  bool Check_Diff_Size(const CPolynomial &bigger, int starting_idx) const
  {
    for (size_t i = starting_idx; i < bigger.m_Indeces.size(); i++) // checks if the remainding indexes are zero
    {
      if (abs(bigger.m_Indeces[i]) > 0.1)
        return false;
    }
    return true;
  }
};

#ifndef __PROGTEST__
bool smallDiff(double a,
               double b)
{
  return (abs(a - b) < 0.1);
}

bool dumpMatch(const CPolynomial &x,
               const vector<double> &ref)
{
  CPolynomial PolRef;
  for (size_t i = 0; i < ref.size(); i++)
    PolRef[i] = ref[i];
  return (x == PolRef);
}

int main(void)
{
  CPolynomial a, b, c;
  ostringstream out;

  a[0] = -10;
  a[1] = 3.5;
  a[3] = 1;
  assert(smallDiff(a(2), 5));
  out.str("");
  out << a;
  assert(out.str() == "x^3 + 3.5*x^1 - 10");
  a = a * -2;
  assert(a.Degree() == 3 && dumpMatch(a, vector<double>{20.0, -7.0, -0.0, -2.0}));

  out.str("");
  out << a;
  assert(out.str() == "- 2*x^3 - 7*x^1 + 20");
  out.str("");
  out << b;
  assert(out.str() == "0");
  b[5] = -1;
  out.str("");
  out << b;
  assert(out.str() == "- x^5");
  c = a + b;
  assert(c.Degree() == 5 && dumpMatch(c, vector<double>{20.0, -7.0, 0.0, -2.0, 0.0, -1.0}));

  out.str("");
  out << c;
  assert(out.str() == "- x^5 - 2*x^3 - 7*x^1 + 20");
  c = a - b;
  assert(c.Degree() == 5 && dumpMatch(c, vector<double>{20.0, -7.0, -0.0, -2.0, -0.0, 1.0}));

  out.str("");
  out << c;
  assert(out.str() == "x^5 - 2*x^3 - 7*x^1 + 20");
  c = a * b;
  assert(c.Degree() == 8 && dumpMatch(c, vector<double>{-0.0, -0.0, 0.0, -0.0, 0.0, -20.0, 7.0, -0.0, 2.0}));

  out.str("");
  out << c;
  assert(out.str() == "2*x^8 + 7*x^6 - 20*x^5");
  assert(a != b);
  b[5] = 0;
  assert(!(a == b));
  a = a * 0;
  assert(a.Degree() == 0 && dumpMatch(a, vector<double>{0.0}));

  assert(a == b);
  return 0;
}
#endif /* __PROGTEST__ */
