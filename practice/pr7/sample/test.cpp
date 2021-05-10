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
#include <set>
#include <map>
#include <deque>
#include <queue>
#include <stack>
#include <cassert>
using namespace std;
#endif /* __PROGTEST__ */

class CTimeStamp
{
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
  
  private:
  tuple<int,int,int,int,int,int> m_Time;
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

int findNumber( const CContact & Contact , const int PhoneNumber , CTimeStamp & from )
{
  if ( Contact.getFirstPerson() == Contact.getSecondPerson())
      return -1;
    else if ( (Contact.getFirstPerson() == PhoneNumber) && 
              ( Contact.getTime().getTime() >= from.getTime() ) )
    {
      from = Contact.getTime();
      return Contact.getSecondPerson();
    }
      
    else if ( (Contact.getSecondPerson() == PhoneNumber) && 
              ( Contact.getTime().getTime() >= from.getTime() ) )
    {
      from = Contact.getTime();
      return Contact.getFirstPerson();
    }
    else 
      return -1;
}

int cmp ( pair<int,CTimeStamp> & Contact1 , pair<int,CTimeStamp> & Contact2 )
{

}

void CheckAndInsert ( vector<pair<int,CTimeStamp>> & Contacts , int save_num , deque<pair<int,CTimeStamp>> & to_search ,CTimeStamp & time )
  {
    auto it = lower_bound(Contacts.begin(),Contacts.end(),save_num);
    if ( it != Contacts.end() && *it == save_num )
      return;
    else 
    {
      Contacts.insert(it,save_num);
      to_search.push_back(make_pair(save_num,time));
    }
      
    return;
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
  public:
    private:
  vector<CContact> m_Contacts;

  public:
  CEFaceMask() {}

  CEFaceMask & addContact ( const CContact & Contact )
  {
    m_Contacts.push_back(Contact);
    return *this;
  }
    // getInfectedGroup ( phone, timeStamp )
  vector<int> getInfectedGroup ( const int phone , const CTimeStamp & time  ) const
  {
    vector<pair<int,CTimeStamp>> infected;
    deque<pair<int,CTimeStamp>> to_search;

    to_search.push_back(make_pair(phone,time));
    infected.push_back(make_pair(phone,time));

    while ( ! to_search.empty() )
    {
      int search = to_search.front().first;
      CTimeStamp timestamp = to_search.front().second;
      to_search.pop_front();

      for ( auto Contact : m_Contacts )
      {
        int save_num=findNumber(Contact,search,timestamp);
        if ( save_num != -1 )
          CheckAndInsert(infected,save_num,to_search,timestamp);
      }
    }

    return infected;
  }
    // getSuperSpreaders ()
  vector<int> getSuperSpreaders (void) const
  {
    map<int,vector<int>> each_contact;
    for ( auto Contact : m_Contacts )
    {
      int first = Contact.getFirstPerson();
      int second = Contact.getSecondPerson();
      if ( first == second )
        continue;
      
      CheckAndInsert( each_contact[first] , second);
      CheckAndInsert( each_contact[second], first);
    }
    vector<int> SuperSpreaders;
    auto it = each_contact.begin();
    size_t max_contacts = 0;
    while ( it != each_contact.end() )
    {
      if ( it->second.size() == max_contacts )
      {
        SuperSpreaders.push_back(it->first);
      }
      else if ( it->second.size() > max_contacts )
      {
        SuperSpreaders.clear();
        SuperSpreaders.push_back(it->first);
        max_contacts = it->second.size();
      }
      it++;
    }
    return SuperSpreaders;
  }
    // TODO
};

#ifndef __PROGTEST__
int main ()
{
  CEFaceMask test;

  test . addContact ( CContact ( CTimeStamp ( 2021, 1, 10, 12, 40, 10 ), 111111111, 222222222 ) );
  test . addContact ( CContact ( CTimeStamp ( 2021, 1, 12, 12, 40, 10 ), 333333333, 222222222 ) )
       . addContact ( CContact ( CTimeStamp ( 2021, 2, 14, 15, 30, 28 ), 222222222, 444444444 ) );
  test . addContact ( CContact ( CTimeStamp ( 2021, 2, 15, 18, 0, 0 ), 555555555, 444444444 ) );
  assert ( test . getSuperSpreaders ( ) == (vector<int> {222222222}) );
  test . addContact ( CContact ( CTimeStamp ( 2021, 3, 20, 18, 0, 0 ), 444444444, 666666666 ) );
  assert ( test . getInfectedGroup ( 222222222, CTimeStamp ( 2021, 1, 11, 17, 17, 17 ) ) == (vector<int> {222222222, 333333333, 444444444, 555555555, 666666666}) );
  test . addContact ( CContact ( CTimeStamp ( 2021, 3, 25, 0, 0, 0 ), 111111111, 666666666 ) );
  assert ( test . getSuperSpreaders ( ) == (vector<int> {222222222, 444444444}) );
  assert ( test . getInfectedGroup ( 111111111, CTimeStamp ( 2021, 1, 1, 0, 0, 0 ) ) == (vector<int> {111111111, 222222222, 333333333, 444444444, 555555555, 666666666}) );
  assert ( test . getInfectedGroup ( 111111111, CTimeStamp ( 2021, 3, 26, 0, 0, 0 ) ) == (vector<int> {111111111}) );
  assert ( test . getInfectedGroup ( 222222222, CTimeStamp ( 2021, 2, 15, 17, 17, 17 ) ) == (vector<int> {222222222}) );
  assert ( test . getInfectedGroup ( 222222222, CTimeStamp ( 2021, 1, 11, 17, 17, 17 ) ) == (vector<int> {111111111, 222222222, 333333333, 444444444, 555555555, 666666666}) );
  return 0;
}
#endif /* __PROGTEST__ */
