#ifndef __PROGTEST__
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <array>
#include <cassert>
using namespace std;
#endif /* __PROGTEST__ */



class CTimeStamp
{
  private:
  tuple<int,int,int,int,int,int> m_Time;

  public:
  /*CTimeStamp( int Year , int Month , int Day , int Hour , int Minute , int Second )
    : m_Year(Year), m_Month(Month), m_Day(Day), m_Hour(Hour), m_Minute(Minute), m_Second(Second) 
    {}*/
  CTimeStamp( int Year , int Month , int Day , int Hour , int Minute , int Second )
    {
      m_Time = make_tuple(Year,Month,Day,Hour,Minute,Second);
    }
  
  tuple<int,int,int,int,int,int> getTime(void) const
  {
    return m_Time;
  }
};

class CContact
{
  protected:
  CTimeStamp m_Time;
  int m_FirstPerson;
  int m_SecondPerson;

  public:
  CContact( CTimeStamp Time , int FirstPerson , int SecondPerson )
  : m_Time(Time), m_FirstPerson(FirstPerson), m_SecondPerson(SecondPerson)
  {}

  CTimeStamp getTime (void) const
  {
    return m_Time;
  }

  int getFirstPerson (void) const
  {
    return m_FirstPerson;
  }

  int getSecondPerson (void) const
  {
    return m_SecondPerson;
  }  

};

int findNumber( const CContact & Contact , const int PhoneNumber )
  {
    if ( Contact.getFirstPerson() == Contact.getSecondPerson())
      return -1;
    else if ( Contact.getFirstPerson() == PhoneNumber )
      return Contact.getSecondPerson();
    else if ( Contact.getSecondPerson() == PhoneNumber )
      return Contact.getFirstPerson();
    else 
      return -1;
  }

int findNumber( const CContact & Contact , const int PhoneNumber , const CTimeStamp & from ,  const CTimeStamp & to )
{
  if ( Contact.getFirstPerson() == Contact.getSecondPerson())
      return -1;
    else if ( (Contact.getFirstPerson() == PhoneNumber) && 
              ( Contact.getTime().getTime() >= from.getTime() ) && 
              (Contact.getTime().getTime() <= to.getTime() ) )
      return Contact.getSecondPerson();
    else if ( (Contact.getSecondPerson() == PhoneNumber) && 
              ( Contact.getTime().getTime() >= from.getTime() ) && 
              (Contact.getTime().getTime() <= to.getTime() ) )
      return Contact.getFirstPerson();
    else 
      return -1;
}

  void CheckAndInsert ( vector<int> & Contacts , int save_num )
  {
    bool is_in = false;
    for ( auto Contact : Contacts)
    {
      if ( Contact == save_num )
      {
        is_in = true;
        break;
      }
    }
    if (!is_in)
      Contacts.push_back(save_num);
    return;
  }

class CEFaceMask
{
  private:
  vector<CContact> m_Contacts;

  public:
  CEFaceMask() {}

  CEFaceMask & addContact ( const CContact & Contact )
  {
    m_Contacts.push_back(Contact);
    return *this;
  }

  

  vector<int> listContacts( const int PhoneNumber) const
  {
    vector<int> FoundContacts;
    for ( auto Contact : m_Contacts)
    {
      int save_num;
      save_num=findNumber(Contact,PhoneNumber);
      if ( save_num != -1 )
        CheckAndInsert(FoundContacts,save_num);
    }
    return FoundContacts;
  }

  vector<int> listContacts ( const int PhoneNumber , const CTimeStamp & from , const CTimeStamp & to ) const
  {
    vector<int> FoundContacts;
    for ( auto Contact : m_Contacts )
    {
      int save_num;
      save_num=findNumber(Contact,PhoneNumber,from,to);
      if ( save_num != -1 )
        CheckAndInsert(FoundContacts,save_num);
    }
    return FoundContacts;
  }

};



#ifndef __PROGTEST__
int main ()
{
  CEFaceMask test;

  test . addContact ( CContact ( CTimeStamp ( 2021, 1, 10, 12, 40, 10 ), 123456789, 999888777 ) );
  test . addContact ( CContact ( CTimeStamp ( 2021, 1, 12, 12, 40, 10 ), 123456789, 111222333 ) )
       . addContact ( CContact ( CTimeStamp ( 2021, 2, 5, 15, 30, 28 ), 999888777, 555000222 ) );
  test . addContact ( CContact ( CTimeStamp ( 2021, 2, 21, 18, 0, 0 ), 123456789, 999888777 ) );
  test . addContact ( CContact ( CTimeStamp ( 2021, 1, 5, 18, 0, 0 ), 123456789, 456456456 ) );
  test . addContact ( CContact ( CTimeStamp ( 2021, 2, 1, 0, 0, 0 ), 123456789, 123456789 ) );
  assert ( test . listContacts ( 123456789 ) == (vector<int> {999888777, 111222333, 456456456}) );
  assert ( test . listContacts ( 999888777 ) == (vector<int> {123456789, 555000222}) );
  assert ( test . listContacts ( 191919191 ) == (vector<int> {}) );
  assert ( test . listContacts ( 123456789, CTimeStamp ( 2021, 1, 5, 18, 0, 0 ), CTimeStamp ( 2021, 2, 21, 18, 0, 0 ) ) == (vector<int> {999888777, 111222333, 456456456}) );
  assert ( test . listContacts ( 123456789, CTimeStamp ( 2021, 1, 5, 18, 0, 1 ), CTimeStamp ( 2021, 2, 21, 17, 59, 59 ) ) == (vector<int> {999888777, 111222333}) );
  assert ( test . listContacts ( 123456789, CTimeStamp ( 2021, 1, 10, 12, 41, 9 ), CTimeStamp ( 2021, 2, 21, 17, 59, 59 ) ) == (vector<int> {111222333}) );
  return 0;
}
#endif /* __PROGTEST__ */
