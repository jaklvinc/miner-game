#ifndef __PROGTEST__
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <string>
#include <memory>
#include <functional>
#include <vector>
#include <algorithm>
using namespace std;
#endif /* __PROGTEST__ */

class CPerson
{
  public:
  string m_Name;
  string m_Surname;
  vector<string> m_Cars;

  CPerson(){}

  CPerson(string name, string surname)
  : m_Name(name) , m_Surname(surname)
  {}

  int operator == ( const CPerson person ) const
  {
    return (m_Surname == person.m_Surname && m_Name == person.m_Name );
  }
};

class CCar
{
  public:
  string m_RZ;
  CPerson m_Owner;

  CCar(){}

  CCar(string RZ)
  : m_RZ(RZ)
  {}
};


class CCarList
{
  public:
    CCarList(){}
    CCarList(const vector<string> & vect);
    const string & RZ      ( void ) const;
    bool           AtEnd   ( void ) const;
    void           Next    ( void );
  private:
    vector<string>::const_iterator m_End;
    vector<string>::const_iterator m_CarIterator;
};

CCarList::CCarList(const vector<string> & vect)
{
  m_CarIterator=vect.begin();
  m_End=vect.end();
}

const string & CCarList::RZ ( void ) const
{ 
  return *m_CarIterator;
}

bool CCarList::AtEnd ( void ) const
{
  return ( m_CarIterator == m_End  );
}
  
void CCarList::Next ( void )
{
  m_CarIterator++;
  return;
}

class CPersonList
{
  public:
    CPersonList( const vector <CPerson> & people );
    const string & Name    ( void ) const;
    const string & Surname ( void ) const;
    bool           AtEnd   ( void ) const;
    void           Next    ( void );
  private:
    vector<CPerson>::const_iterator m_End;
    vector<CPerson>::const_iterator m_PersonIterator;
};

CPersonList::CPersonList(const vector <CPerson> & people )
{
  m_End=people.end();
  m_PersonIterator=people.begin();
}

const string & CPersonList::Name ( void ) const
{ 
  return m_PersonIterator->m_Name;
}

const string & CPersonList::Surname ( void ) const
{ 
  return m_PersonIterator->m_Surname;
}

bool CPersonList::AtEnd ( void ) const
{
  return ( m_PersonIterator == m_End  );
}
  
void CPersonList::Next ( void )
{
  m_PersonIterator++;
  return;
}

class CRegister
{
  public:
                CRegister  ( void );
               ~CRegister  ( void );
                CRegister  ( const CRegister & src ) = delete;
    CRegister & operator = ( const CRegister & src ) = delete;
    bool        AddCar     ( const string & rz,
                             const string & name,
                             const string & surname );
    bool        DelCar     ( const string & rz );
    bool        Transfer   ( const string & rz,
                             const string & nName,
                             const string & nSurname);
    CCarList    ListCars   ( const string & name,
                             const string & surname ) const;
    int         CountCars  ( const string & name,
                             const string & surname ) const;
    CPersonList ListPersons( void ) const;
  private:
    vector<CPerson> m_Owners;
    vector<CCar>    m_Cars;
};

//compare function used to search by name
bool cmpP( const CPerson &a , const CPerson &b)
{
  if (a.m_Surname == b.m_Surname)
  {
    return a.m_Name < b.m_Name;
  }
  else
  return a.m_Surname < b.m_Surname ;
}

//compare function used to search by registry number
bool cmpC( const CCar &a , const CCar &b )
{
  return a.m_RZ < b.m_RZ ;
}

CRegister::CRegister(){}

CRegister::~CRegister(){}

bool CRegister::AddCar ( const string & rz, const string & name, const string & surname )
{
  CCar NewCar(rz);
  auto itC = lower_bound(m_Cars.begin(),m_Cars.end(),NewCar,cmpC);
  if ( itC != m_Cars.end() && itC->m_RZ == NewCar.m_RZ )
    return false;
  
  CPerson NewPers(name,surname);
  auto itP = lower_bound(m_Owners.begin(),m_Owners.end(),NewPers,cmpP);
  if ( itP != m_Owners.end() && (*itP) == NewPers )
  {
    auto itCarInPers = lower_bound(itP->m_Cars.begin(),itP->m_Cars.end(),rz);
    itP->m_Cars.insert(itCarInPers,rz);
    NewCar.m_Owner=*itP;
  }
  else
  {
    NewPers.m_Cars.push_back(rz);
    m_Owners.insert(itP,NewPers);
    NewCar.m_Owner=NewPers;
  }

  m_Cars.insert(itC,NewCar);

  return true;
}

bool CRegister::DelCar ( const string & rz )
{
  CCar Delete(rz);

  auto itC = lower_bound(m_Cars.begin(),m_Cars.end(),Delete,cmpC);
  if ( itC == m_Cars.end() || itC->m_RZ != rz )
    return false;

  CPerson DeleteP = itC->m_Owner;
  
  auto itP = lower_bound(m_Owners.begin(),m_Owners.end(),DeleteP,cmpP);
  auto itCarInPer = lower_bound(itP->m_Cars.begin(),itP->m_Cars.end(),rz);
  

  itP->m_Cars.erase(itCarInPer);
  //cout << itP->m_Cars.size() << endl;
  //cout << itP->m_Name << " " << itP->m_Surname << endl;
  if ( itP->m_Cars.empty() )
    m_Owners.erase(itP);

  m_Cars.erase(itC);
  return true;

}

bool CRegister::Transfer ( const string & rz, const string & nName , const string & nSurname)
{
  CCar transfer(rz);

  auto itC = lower_bound(m_Cars.begin(),m_Cars.end(),transfer,cmpC);
  if ( itC == m_Cars.end() || itC->m_RZ != rz )
    return false;
  
  if ( itC->m_Owner.m_Name == nName && itC->m_Owner.m_Surname == nSurname )
    return false;

  DelCar(rz);
  AddCar(rz,nName,nSurname);
  return true;
}

CCarList CRegister::ListCars( const string & name, const string & surname ) const
{
  CPerson NewPerson(name,surname);
  auto it=lower_bound(m_Owners.begin(),m_Owners.end(),NewPerson,cmpP);
  if ( it == m_Owners.end() || !(*it == NewPerson) )
    return CCarList();
  CCarList carList(it->m_Cars);
  return carList;
}

int CRegister::CountCars ( const string & name, const string & surname ) const
{
  CPerson NewPerson(name,surname);
  auto it=lower_bound(m_Owners.begin(),m_Owners.end(),NewPerson,cmpP);
  if ( it == m_Owners.end() || !(*it == NewPerson) )
    return 0;
  return it->m_Cars.size();
}

CPersonList CRegister::ListPersons ( void ) const
{
  return CPersonList(m_Owners);
}

#ifndef __PROGTEST__
bool checkPerson           ( CRegister    & r,
                             const string & name,
                             const string & surname,
                             vector<string> result )
{
  for ( CCarList l = r . ListCars ( name, surname ); ! l . AtEnd (); l . Next () )
  {
    auto pos = find ( result . begin (), result . end (), l . RZ () ); 
    if ( pos == result . end () )
      return false;
    result . erase ( pos );
  }
  return result . size () == 0;
}
int main ( void )
{
  CRegister b1;
  assert ( b1 . AddCar ( "ABC-12-34", "John", "Smith" ) == true );
  assert ( b1 . AddCar ( "ABC-32-22", "John", "Hacker" ) == true );
  assert ( b1 . AddCar ( "XYZ-11-22", "Peter", "Smith" ) == true );
  assert ( b1 . CountCars ( "John", "Hacker" ) == 1 );
  assert ( checkPerson ( b1, "John", "Hacker", { "ABC-32-22" } ) );
  assert ( b1 . Transfer ( "XYZ-11-22", "John", "Hacker" ) == true );
  assert ( b1 . AddCar ( "XYZ-99-88", "John", "Smith" ) == true );
  assert ( b1 . CountCars ( "John", "Smith" ) == 2 );
  assert ( checkPerson ( b1, "John", "Smith", { "ABC-12-34", "XYZ-99-88" } ) );
  assert ( b1 . CountCars ( "John", "Hacker" ) == 2 );
  assert ( checkPerson ( b1, "John", "Hacker", { "ABC-32-22", "XYZ-11-22" } ) );
  assert ( b1 . CountCars ( "Peter", "Smith" ) == 0 );
  assert ( checkPerson ( b1, "Peter", "Smith", {  } ) );
  assert ( b1 . Transfer ( "XYZ-11-22", "Jane", "Black" ) == true );
  assert ( b1 . CountCars ( "Jane", "Black" ) == 1 );
  assert ( checkPerson ( b1, "Jane", "Black", { "XYZ-11-22" } ) );
  assert ( b1 . CountCars ( "John", "Smith" ) == 2 );
  assert ( checkPerson ( b1, "John", "Smith", { "ABC-12-34", "XYZ-99-88" } ) );
  assert ( b1 . DelCar ( "XYZ-11-22" ) == true );
  assert ( b1 . CountCars ( "Jane", "Black" ) == 0 );
  assert ( checkPerson ( b1, "Jane", "Black", {  } ) );
  assert ( b1 . AddCar ( "XYZ-11-22", "George", "White" ) == true );
  CPersonList i1 = b1 . ListPersons ();
  assert ( ! i1 . AtEnd () && i1 . Surname () == "Hacker" && i1 . Name () == "John" );
  i1 . Next ();
  assert ( ! i1 . AtEnd () && i1 . Surname () == "Smith" && i1 . Name () == "John" );
  i1 . Next ();
  assert ( ! i1 . AtEnd () && i1 . Surname () == "White" && i1 . Name () == "George" );
  i1 . Next ();
  assert ( i1 . AtEnd () );

  CRegister b2;
  assert ( b2 . AddCar ( "ABC-12-34", "John", "Smith" ) == true );
  assert ( b2 . AddCar ( "ABC-32-22", "John", "Hacker" ) == true );
  assert ( b2 . AddCar ( "XYZ-11-22", "Peter", "Smith" ) == true );
  assert ( b2 . AddCar ( "XYZ-11-22", "Jane", "Black" ) == false );
  assert ( b2 . DelCar ( "AAA-11-11" ) == false );
  assert ( b2 . Transfer ( "BBB-99-99", "John", "Smith" ) == false );
  assert ( b2 . Transfer ( "ABC-12-34", "John", "Smith" ) == false );
  assert ( b2 . CountCars ( "George", "White" ) == 0 );
  assert ( checkPerson ( b2, "George", "White", {  } ) );
  CPersonList i2 = b2 . ListPersons ();
  assert ( ! i2 . AtEnd () && i2 . Surname () == "Hacker" && i2 . Name () == "John" );
  i2 . Next ();
  assert ( ! i2 . AtEnd () && i2 . Surname () == "Smith" && i2 . Name () == "John" );
  i2 . Next ();
  assert ( ! i2 . AtEnd () && i2 . Surname () == "Smith" && i2 . Name () == "Peter" );
  i2 . Next ();
  assert ( i2 . AtEnd () );

  return 0;
}
#endif /* __PROGTEST__ */
