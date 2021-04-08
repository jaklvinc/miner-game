#ifndef __PROGTEST__
#include <cassert>
#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <set>
#include <list>
#include <map>
#include <algorithm>
using namespace std;

class CResult
{
  public:
                   CResult     ( const string & name,
                                 unsigned int   studentID,
                                 const string & test,
                                 int            result )
      : m_Name ( name ),
        m_StudentID ( studentID ),
        m_Test ( test ),
        m_Result ( result )
    {
    }
    bool           operator == ( const CResult& other ) const
    {
      return m_Name == other . m_Name 
             && m_StudentID == other . m_StudentID
             && m_Test == other . m_Test
             && m_Result == other . m_Result;
    }
    string         m_Name;
    unsigned int   m_StudentID;
    string         m_Test;
    int            m_Result;
 };
#endif /* __PROGTEST__ */\

struct SStudent
{
  public:
  SStudent( int ID , string Name )
  : m_ID(ID) , m_Name(Name) {};

  bool operator < ( const int cmp ) const
  {
    return m_ID < cmp;
  }

  bool operator < ( const SStudent & cmp ) const
  {
    return m_ID < cmp.m_ID;
  }

  int m_ID;
  string m_Name;
};

class CExam
{
  public:
    static const int SORT_NONE   = 0;
    static const int SORT_ID     = 1;
    static const int SORT_NAME   = 2;
    static const int SORT_RESULT = 3;

    bool           Load        ( istream      & cardMap );
    bool           Register    ( const string & cardID,
                                 const string & test );
    bool           Assess      ( unsigned int   studentID,
                                 const string & test,
                                 int            result );
    list<CResult>  ListTest    ( const string & testName,
                                 int            sortBy ) const;
    set<unsigned int> ListMissing ( const string & testName ) const;

  private:
    map<string,unsigned int> m_LoadedCards;
    vector<SStudent> m_LoadedStudents;
    map<string,set<unsigned int>> m_GradedStudents;
    map<string,set<unsigned int>> m_Tests;
    map<string,list<CResult>> m_GradesByAddition;
    map<string,list<CResult>> m_GradesByGrade;
    map<string,list<CResult>> m_GradesByName;
    map<string,list<CResult>> m_GradesByID;
};

bool cmpStudentID( const SStudent & first , const SStudent & second )
{
  return first.m_ID < second.m_ID;
}

bool CExam::Load ( istream & cardMap )
{
  string sStudent;
  map<string,unsigned int> LoadedCards;
  vector<SStudent> LoadedStudents;
  while ( getline(cardMap,sStudent) )
  {
    stringstream ssStudent(sStudent);
    string sID;
    string Name;
    string sCards;
    if( 
    !getline(ssStudent,sID,':') ||
    !getline(ssStudent,Name,':') ||
    !getline(ssStudent,sCards,':') )
    {
      return false;
    }
    int ID = stoi(sID);
    string Card;
    stringstream ssCards(sCards);

    while ( getline(ssCards,Card,',') )
    {
      if ( Card[0] == ' ' )
        Card.erase(Card.begin());
      if ( m_LoadedCards.find(Card) != m_LoadedCards.end() || LoadedCards.find(Card) != LoadedCards.end() )
      {
        return false;
      }
      else
      {
        LoadedCards.emplace(Card,ID);
      }
    }
    
    SStudent Student(ID,Name);
    auto itMS = lower_bound(m_LoadedStudents.begin(),m_LoadedStudents.end(),Student,cmpStudentID);
    auto itS = lower_bound(LoadedStudents.begin(),LoadedStudents.end(),Student,cmpStudentID);
    if (  ( itMS != m_LoadedStudents.end() && itMS->m_ID == Student.m_ID ) || 
          ( itS != LoadedStudents.end() && itS->m_ID == Student.m_ID  ) )
    {
      return false;
    }
    else 
    {
      LoadedStudents.insert(itS,Student);
    }
  }
  m_LoadedCards.merge(LoadedCards);
  for ( auto i : LoadedStudents )
  {
    auto it = lower_bound(m_LoadedStudents.begin(),m_LoadedStudents.end(),i,cmpStudentID);
    m_LoadedStudents.insert(it,i);
  }
  return true;
}

bool CExam::Register ( const string & cardID , const string & test)
{
  auto itCards = m_LoadedCards.find(cardID);
  if ( itCards == m_LoadedCards.end() )
  {
    return false;
  }

  auto itTest = lower_bound(m_Tests[test].begin(),m_Tests[test].end(),itCards->second);
  if ( itTest != m_Tests[test].end() && *itTest == itCards->second )
  {
    return false;
  }
  m_Tests[test].insert(itTest,itCards->second);

  return true;
}

void AddToMapList ( list<CResult> & List , const CResult & Grade , const char gl )
{
  bool set = false;
  for ( auto i = List.begin() ; i != List.end() ; i++ )
  {
    if ( gl == 'g' )
    {
      if ( Grade.m_Result > i->m_Result )
      {
        set = true;
        List.insert(i,Grade);
        break;
      }
    }
    else if ( gl == 'l' )
    {
      if ( Grade.m_Result < i->m_Result )
      {
        set = true;
        List.insert(i,Grade);
        break;
      }
    }
  }

  if ( List.empty() || ! set )
    List.push_back(Grade);
  
  return;
}

bool CExam::Assess ( unsigned int studentID , const string & test , int result)
{
  auto itTest = lower_bound(m_Tests[test].begin(),m_Tests[test].end(),studentID);
  if ( itTest == m_Tests[test].end() || *itTest != studentID )
  {
    return false;
  }
  auto itStudent = lower_bound(m_LoadedStudents.begin(),m_LoadedStudents.end(),studentID);
  CResult Grade(itStudent->m_Name,itStudent->m_ID,test,result);

  for ( auto i : m_GradesByAddition[test] )
  {
    if ( i.m_StudentID == Grade.m_StudentID )
    {
      return false;
    }
  }
  m_GradesByAddition[test].push_back(Grade);

  AddToMapList(m_GradesByGrade[test],Grade,'g');
  AddToMapList(m_GradesByName[test],Grade,'l');
  AddToMapList(m_GradesByID[test],Grade,'g');
  m_GradedStudents[test].insert(studentID);
  return true;
}

list<CResult> CExam::ListTest ( const string & testName, int sortBy ) const
{
  switch (sortBy)
  {
    case SORT_NONE: 
      return m_GradesByAddition.find(testName)->second;
    case SORT_RESULT:
      return m_GradesByGrade.find(testName)->second;
    case SORT_ID:
      return m_GradesByID.find(testName)->second;
    case SORT_NAME:
      return m_GradesByName.find(testName)->second;
    default:
      return m_GradesByAddition.find(testName)->second;
  }
}

set<unsigned int> CExam::ListMissing ( const string & testName ) const
{
  set<unsigned int> result;
  set_difference( m_Tests.find(testName)->second.begin() , m_Tests.find(testName)->second.end() , 
                  m_GradedStudents.find(testName)->second.begin() , m_GradedStudents.find(testName)->second.end(),
                  inserter(result,result.end()) );
  return result;
}


#ifndef __PROGTEST__
int main ( void )
{
  istringstream iss;
  CExam         m;
  iss . clear ();
  iss . str ( "123456:Smith John:er34252456hjsd2451451, 1234151asdfe5123416, asdjklfhq3458235\n"
      "654321:Nowak Jane: 62wtsergtsdfg34\n"
      "456789:Nowak Jane: okjer834d34\n"
      "987:West Peter Thomas:sdswertcvsgncse\n" );
  assert ( m . Load ( iss ) );

  assert ( m . Register ( "62wtsergtsdfg34", "PA2 - #1" ) );
  assert ( m . Register ( "62wtsergtsdfg34", "PA2 - #2" ) );
  assert ( m . Register ( "er34252456hjsd2451451", "PA2 - #1" ) );
  assert ( m . Register ( "er34252456hjsd2451451", "PA2 - #3" ) );
  assert ( m . Register ( "sdswertcvsgncse", "PA2 - #1" ) );
  assert ( !m . Register ( "1234151asdfe5123416", "PA2 - #1" ) );
  assert ( !m . Register ( "aaaaaaaaaaaa", "PA2 - #1" ) );
  assert ( m . Assess ( 123456, "PA2 - #1", 50 ) );
  assert ( m . Assess ( 654321, "PA2 - #1", 30 ) );
  assert ( m . Assess ( 654321, "PA2 - #2", 40 ) );
  assert ( m . Assess ( 987, "PA2 - #1", 100 ) );
  assert ( !m . Assess ( 654321, "PA2 - #1", 35 ) );
  assert ( !m . Assess ( 654321, "PA2 - #3", 35 ) );
  assert ( !m . Assess ( 999999, "PA2 - #1", 35 ) );
  assert ( m . ListTest ( "PA2 - #1", CExam::SORT_RESULT ) == (list<CResult>
  {
    CResult ( "West Peter Thomas", 987, "PA2 - #1", 100 ),
    CResult ( "Smith John", 123456, "PA2 - #1", 50 ),
    CResult ( "Nowak Jane", 654321, "PA2 - #1", 30 )
  } ) );
  assert ( m . ListTest ( "PA2 - #1", CExam::SORT_NAME ) == (list<CResult>
  {
    CResult ( "Nowak Jane", 654321, "PA2 - #1", 30 ),
    CResult ( "Smith John", 123456, "PA2 - #1", 50 ),
    CResult ( "West Peter Thomas", 987, "PA2 - #1", 100 )
  } ) );
  assert ( m . ListTest ( "PA2 - #1", CExam::SORT_NONE ) == (list<CResult>
  {
    CResult ( "Smith John", 123456, "PA2 - #1", 50 ),
    CResult ( "Nowak Jane", 654321, "PA2 - #1", 30 ),
    CResult ( "West Peter Thomas", 987, "PA2 - #1", 100 )
  } ) );
  assert ( m . ListTest ( "PA2 - #1", CExam::SORT_ID ) == (list<CResult>
  {
    CResult ( "West Peter Thomas", 987, "PA2 - #1", 100 ),
    CResult ( "Smith John", 123456, "PA2 - #1", 50 ),
    CResult ( "Nowak Jane", 654321, "PA2 - #1", 30 )
  } ) );
  assert ( m . ListMissing ( "PA2 - #3" ) == (set<unsigned int>{ 123456 }) );
  iss . clear ();
  iss . str ( "888:Watson Joe:25234sdfgwer52, 234523uio, asdf234235we, 234234234\n" );
  assert ( m . Load ( iss ) );

  assert ( m . Register ( "234523uio", "PA2 - #1" ) );
  assert ( m . Assess ( 888, "PA2 - #1", 75 ) );
  iss . clear ();
  iss . str ( "555:Gates Bill:ui2345234sdf\n"
      "888:Watson Joe:2345234634\n" );
  assert ( !m . Load ( iss ) );

  assert ( !m . Register ( "ui2345234sdf", "PA2 - #1" ) );
  iss . clear ();
  iss . str ( "555:Gates Bill:ui2345234sdf\n"
      "666:Watson Thomas:okjer834d34\n" );
  assert ( !m . Load ( iss ) );

  assert ( !m . Register ( "ui2345234sdf", "PA2 - #3" ) );
  iss . clear ();
  iss . str ( "555:Gates Bill:ui2345234sdf\n"
      "666:Watson Thomas:jer834d3sdf4\n" );
  assert ( m . Load ( iss ) );

  assert ( m . Register ( "ui2345234sdf", "PA2 - #3" ) );
  assert ( m . ListMissing ( "PA2 - #3" ) == (set<unsigned int>{ 555, 123456 }) ); 

  return 0;
}
#endif /* __PROGTEST__ */
