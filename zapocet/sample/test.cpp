#ifndef __PROGTEST__
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstdint>
#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <queue>
#include <stack>
#include <deque>
#include <algorithm>
#include <memory>
#include <functional>
#include <stdexcept>
using namespace std;

enum class ERel
{
  REL_NONE,
  REL_FATHER,
  REL_MOTHER,
  REL_SON,
  REL_DAUGHTER
};
#endif /* __PROGTEST__ */

bool CheckPrefix( const string & prefix , const string & checkWith )
{
  if ( prefix.size() > checkWith.size() )
    return false;
  
  auto result = mismatch(prefix.begin(),prefix.end(),checkWith.begin());
  if ( result.first == prefix.end() )
    return true;
  else 
    return false;
}

class CPerson
{
  public:
  CPerson( int id , string name ):
  m_Name(name) , m_ID(id) 
  {
  }

  int GetID ( void )
  {
    return m_ID;
  }

  virtual ostream & Print( ostream & os ) const
  {
    os << m_ID << ": " << m_Name << " ";
    return os;
  }
  friend ostream & operator << ( ostream & os , const CPerson & out )
  {
    return out.Print(os);
  }

  void PrintDescendants( ostream & os , string before = "" ) const
  {
    Print(os);
    os << endl;
    if ( m_Descendants.size() != 0 )
    {
      for ( size_t i = 0 ; i < (m_Descendants.size()-1) ; i++ )
      {
        os << before;
        os << " +- ";
        m_Descendants[i].lock().get()->PrintDescendants(os,before+" | ");
      }
      os << before;
      os << " \\- ";
      m_Descendants[m_Descendants.size()-1].lock().get()->PrintDescendants(os,before+"   ");
    }
  }

  void AddFather( shared_ptr<CPerson> father )
  {
    m_Father=father;
  }

  void AddMother( shared_ptr<CPerson> mother)
  {
    m_Mother=mother;
  }
  
  shared_ptr<CPerson> GetMother()
  {
    return m_Mother.lock();
  }

  shared_ptr<CPerson> GetFather()
  {
    return m_Father.lock();
  }

  virtual bool SameName( const string & prefix ) const = 0; 

  void AddDescendant ( shared_ptr<CPerson> descendant )
  {
    m_Descendants.emplace_back(descendant);
    return;
  }

  vector<weak_ptr<CPerson>> GetDescendants(void)
  {
    return m_Descendants;
  }

  protected:
    string m_Name;
    int m_ID;
    weak_ptr<CPerson> m_Mother;
    weak_ptr<CPerson> m_Father;
    vector<weak_ptr<CPerson>> m_Descendants;
};

class CMan : public CPerson
{
  public:
    CMan( int id , string name ):
    CPerson(id,name){}
    // GetID ()
    ostream & Print ( ostream & os ) const override
    {
      CPerson::Print(os);
      os << "(man)";
      return os;
    }

    bool SameName ( const string & prefix ) const
    {
      return CheckPrefix(prefix,m_Name);
    }
};

class CWoman : public CPerson
{
  public:
    CWoman( int id , string name ):
    CPerson(id,name){}
    // GetID ()
    ostream & Print ( ostream & os ) const override
    {
      CPerson::Print(os);
      if ( ! m_OldNames.empty() )
      {
        os << "[";
        for ( size_t i = 0 ; i < m_OldNames.size()-1 ; i++ )
        {
          os << m_OldNames[i] << ", ";
        }
        os << m_OldNames[m_OldNames.size()-1] << "] ";
      }
      os << "(woman)";
      return os;
    }
    
    bool SameName ( const string & prefix ) const
    {
      for ( auto oldName : m_OldNames )
      {
        if ( CheckPrefix(prefix,oldName) )
          return true;
      }
      if ( CheckPrefix(prefix,m_Name) )
        return true;
      else
        return false;
    }

    void Wedding ( string newName )
    {
      m_OldNames.emplace_back(m_Name);
      m_Name = newName;
    }
  private:
    vector<string> m_OldNames;
};

bool CmpByPerson( const shared_ptr<CPerson> first , const shared_ptr<CPerson> second )
{
  return first.get()->GetID() < second.get()->GetID();
}

bool CmpByID( const shared_ptr<CPerson> first , const int second )
{
  return first.get()->GetID() < second;
}

class CRegister 
{
  public:
    CRegister(){};
    bool                 Add           ( shared_ptr<CPerson> person, shared_ptr<CPerson> father, shared_ptr<CPerson> mother )
    {   
      
      auto inserted = m_Registry.insert(pair<int,shared_ptr<CPerson>>(person.get()->GetID(),person));
      if ( inserted.second == false )
      {
        return false;
      }

      if ( father != nullptr )
      {
        person.get()->AddFather(father);
        auto it = m_Registry.find(father.get()->GetID());
        it->second->AddDescendant(person);
      }

      if ( mother != nullptr )
      {
        person.get()->AddMother(mother);
        auto it = m_Registry.find(mother.get()->GetID());
        it->second->AddDescendant(person);
      }

      return true;
    }

    shared_ptr<CPerson>                  FindByID      ( int id ) const
    {
      auto it = m_Registry.find(id);
      if ( it == m_Registry.end() )
        return nullptr;
      else
        return it->second;
    }

    vector<shared_ptr<CPerson>>            FindByName    ( string prefix ) const
    {
      vector<shared_ptr<CPerson>> NameChecksOut;
      for ( auto person : m_Registry )
      {
        if ( person.second->SameName(prefix) )
          NameChecksOut.emplace_back(person.second);
      }
      return NameChecksOut;
    }

    list<pair<shared_ptr<CPerson>, ERel> > FindRelatives ( const int id1, const int id2 ) const
    {
      list<pair<shared_ptr<CPerson>,ERel>> relatives;
      /*queue<shared_ptr<CPerson>> waiting;
      map<int,shared_ptr<CPerson>> visited;

      shared_ptr<CPerson> firstPerson = FindByID(id1);
      visited.insert(pair<int,shared_ptr<CPerson>>(id1,firstPerson));
      shared_ptr<CPerson> secondPerson = FindByID(id2);
      
      for ( auto descendant : firstPerson.get()->GetDescendants())
      {
        visited.insert(pair<int,shared_ptr<CPerson>>(id1,firstPerson));
        waiting.push(descendant.lock());
      }

      auto mother=firstPerson.get()->GetMother();
      auto father=firstPerson.get()->GetFather();

      if ( mother != nullptr )
      {
        visited.insert(pair<int,shared_ptr<CPerson>>(mother->GetID(),mother));
        waiting.push(mother);
      }

      if ( father != nullptr )
      {
        visited.insert(pair<int,shared_ptr<CPerson>>(father->GetID(),father));
        waiting.push(father);
      }
      

      while ( !waiting.empty() )
      {

      }*/
      return relatives;
    }

  private:
   map<int,shared_ptr<CPerson>> m_Registry;
};

#ifndef __PROGTEST__
template <typename T_>
static bool        vectorMatch ( const vector<T_>     & res,
                                 const vector<string> & ref )
{
  vector<string> tmp;
  for ( const auto & x : res )
  {
    ostringstream oss;
    oss << *x;
    tmp . push_back ( oss . str () );
  }
  return tmp == ref;
}
template <typename T_>
static bool        listMatch ( const list<pair<T_, ERel> >     & res,
                               const list<pair<string, ERel> > & ref )
{
  list<pair<string, ERel> > tmp;
  for ( const auto & x : res )
  {
    ostringstream oss;
    oss << *x . first;
    tmp . emplace_back ( oss . str (), x . second );
  }
  return tmp == ref;
}
int main ( void )
{
  CRegister mytest;
  assert ( mytest.Add( make_shared<CMan> ( 0, "Honza Borec" ) , nullptr , nullptr  ) == true );
  for ( int i = 1 ; i < 100000 ; i++ )
    assert ( mytest.Add( make_shared<CMan> ( i, "Honza Borec" ) , mytest . FindByID ( i-1 ) , nullptr  ) == true );

  ostringstream oss;
  CRegister r;
  assert ( r . Add ( make_shared<CMan> ( 1, "Peterson George" ),
                     nullptr, nullptr ) == true );
  assert ( r . Add ( make_shared<CMan> ( 2, "Watson Paul" ),
                     nullptr, nullptr ) == true );
  assert ( r . Add ( make_shared<CMan> ( 10, "Smith Samuel" ),
                     nullptr, nullptr ) == true );
  assert ( r . Add ( make_shared<CWoman> ( 11, "Peterson Jane" ),
                     r . FindByID ( 1 ), nullptr ) == true );
  assert ( r . Add ( make_shared<CWoman> ( 12, "Peterson Sue" ),
                     r . FindByID ( 1 ), nullptr ) == true );
  assert ( r . Add ( make_shared<CMan> ( 13, "Pershing John" ),
                     nullptr, nullptr ) == true );
  assert ( r . Add ( make_shared<CMan> ( 14, "Pearce Joe" ),
                     nullptr, nullptr ) == true );
  assert ( r . Add ( make_shared<CMan> ( 15, "Peant Thomas" ),
                     nullptr, nullptr ) == true );
  assert ( r . Add ( make_shared<CMan> ( 100, "Smith John" ),
                     r . FindByID ( 10 ), r . FindByID ( 11 ) ) == true );
  assert ( r . Add ( make_shared<CMan> ( 101, "Smith Roger" ),
                     r . FindByID ( 10 ), r . FindByID ( 11 ) ) == true );
  assert ( r . Add ( make_shared<CMan> ( 102, "Smith Daniel" ),
                     r . FindByID ( 10 ), r . FindByID ( 11 ) ) == true );
  assert ( r . Add ( make_shared<CWoman> ( 103, "Smith Eve" ),
                     r . FindByID ( 10 ), r . FindByID ( 11 ) ) == true );
  assert ( r . Add ( make_shared<CWoman> ( 103, "Smith Jane" ),
                     r . FindByID ( 10 ), r . FindByID ( 11 ) ) == false );
  dynamic_cast<CWoman &> ( *r . FindByID ( 12 ) ) . Wedding ( "Smith Sue" );
  assert ( r . Add ( make_shared<CMan> ( 150, "Pershing Joe" ),
                     r . FindByID ( 13 ), r . FindByID ( 12 ) ) == true );
  dynamic_cast<CWoman &> ( *r . FindByID ( 12 ) ) . Wedding ( "Pearce Sue" );
  assert ( r . Add ( make_shared<CMan> ( 151, "Pearce Phillip" ),
                     r . FindByID ( 14 ), r . FindByID ( 12 ) ) == true );
  dynamic_cast<CWoman &> ( *r . FindByID ( 12 ) ) . Wedding ( "Peant Sue" );
  assert ( r . Add ( make_shared<CMan> ( 152, "Peant Harry" ),
                     r . FindByID ( 15 ), r . FindByID ( 12 ) ) == true );
  assert ( r . Add ( make_shared<CMan> ( 200, "Pershing Peter" ),
                     r . FindByID ( 150 ), r . FindByID ( 103 ) ) == true );
  assert ( r . Add ( make_shared<CWoman> ( 201, "Pershing Julia" ),
                     r . FindByID ( 150 ), r . FindByID ( 103 ) ) == true );
  assert ( r . Add ( make_shared<CWoman> ( 202, "Pershing Anne" ),
                     r . FindByID ( 150 ), r . FindByID ( 103 ) ) == true );
  assert ( vectorMatch ( r . FindByName ( "Peterson" ), vector<string>
           {
             "1: Peterson George (man)",
             "11: Peterson Jane (woman)",
             "12: Peant Sue [Peterson Sue, Smith Sue, Pearce Sue] (woman)"
           } ) );
  assert ( vectorMatch ( r . FindByName ( "Pe" ), vector<string>
           {
             "1: Peterson George (man)",
             "11: Peterson Jane (woman)",
             "12: Peant Sue [Peterson Sue, Smith Sue, Pearce Sue] (woman)",
             "13: Pershing John (man)",
             "14: Pearce Joe (man)",
             "15: Peant Thomas (man)",
             "150: Pershing Joe (man)",
             "151: Pearce Phillip (man)",
             "152: Peant Harry (man)",
             "200: Pershing Peter (man)",
             "201: Pershing Julia (woman)",
             "202: Pershing Anne (woman)"
           } ) );
  assert ( vectorMatch ( r . FindByName ( "Smith" ), vector<string>
           {
             "10: Smith Samuel (man)",
             "12: Peant Sue [Peterson Sue, Smith Sue, Pearce Sue] (woman)",
             "100: Smith John (man)",
             "101: Smith Roger (man)",
             "102: Smith Daniel (man)",
             "103: Smith Eve (woman)"
           } ) );
  assert ( r . FindByID ( 1 ) -> GetID () == 1 );
  oss . str ( "" );
  oss << * r . FindByID ( 1 );
  assert ( oss . str () == "1: Peterson George (man)" );
  oss . str ( "" );
  r . FindByID ( 1 ) -> PrintDescendants ( oss );
  //r . FindByID ( 1 ) -> PrintDescendants ( cout );
  //cout << flush;
  assert ( oss . str () ==
    "1: Peterson George (man)\n"
    " +- 11: Peterson Jane (woman)\n"
    " |  +- 100: Smith John (man)\n"
    " |  +- 101: Smith Roger (man)\n"
    " |  +- 102: Smith Daniel (man)\n"
    " |  \\- 103: Smith Eve (woman)\n"
    " |     +- 200: Pershing Peter (man)\n"
    " |     +- 201: Pershing Julia (woman)\n"
    " |     \\- 202: Pershing Anne (woman)\n"
    " \\- 12: Peant Sue [Peterson Sue, Smith Sue, Pearce Sue] (woman)\n"
    "    +- 150: Pershing Joe (man)\n"
    "    |  +- 200: Pershing Peter (man)\n"
    "    |  +- 201: Pershing Julia (woman)\n"
    "    |  \\- 202: Pershing Anne (woman)\n"
    "    +- 151: Pearce Phillip (man)\n"
    "    \\- 152: Peant Harry (man)\n" );
  assert ( r . FindByID ( 2 ) -> GetID () == 2 );
  oss . str ( "" );
  oss << * r . FindByID ( 2 );
  assert ( oss . str () == "2: Watson Paul (man)" );
  oss . str ( "" );
  r . FindByID ( 2 ) -> PrintDescendants ( oss );
  assert ( oss . str () ==
    "2: Watson Paul (man)\n" );
  assert ( listMatch ( r . FindRelatives ( 100, 1 ), list<pair<string, ERel> >
           {
             { "11: Peterson Jane (woman)", ERel::REL_MOTHER },
             { "1: Peterson George (man)", ERel::REL_FATHER }
           } ) );
  assert ( listMatch ( r . FindRelatives ( 100, 13 ), list<pair<string, ERel> >
           {
             { "10: Smith Samuel (man)", ERel::REL_FATHER },
             { "103: Smith Eve (woman)", ERel::REL_DAUGHTER },
             { "200: Pershing Peter (man)", ERel::REL_SON },
             { "150: Pershing Joe (man)", ERel::REL_FATHER },
             { "13: Pershing John (man)", ERel::REL_FATHER }
           } ) );
  assert ( listMatch ( r . FindRelatives ( 100, 2 ), list<pair<string, ERel> >
           {
           } ) );
  try
  {
    r . FindRelatives ( 100, 3 );
    assert ( "Missing an exception" == nullptr );
  }
  catch ( const invalid_argument & e )
  {
  }
  catch ( ... )
  {
    assert ( "An unexpected exception thrown" );
  }
  try
  {
    r . FindRelatives ( 100, 100 );
    assert ( "Missing an exception" == nullptr );
  }
  catch ( const invalid_argument & e )
  {
  }
  catch ( ... )
  {
    assert ( "An unexpected exception thrown" );
  } 
  //cout << "PRDEL" << endl << flush;
  return 0;
}
#endif /* __PROGTEST__ */
