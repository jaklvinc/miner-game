#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <memory>
#include <functional>
#include <stdexcept>
#include "ipaddress.h"
using namespace std;
#endif /* __PROGTEST__ */

class CRec
{
  public:
    CRec() {};
    CRec( string name):
    m_Name(name) {};

    string Name( void ) const
    {
      return m_Name;
    }
    virtual string Type( void ) const = 0;
    virtual ostream & Print( ostream & os ) const = 0;
    friend ostream & operator << ( ostream & os , const CRec & out )
    {
      return out.Print(os);
    }
    virtual bool Equals( const CRec & cmp ) const = 0;
    
  protected:
    string m_Name;
};

class CRecA : public CRec
{
  public:
    CRecA( const string & name , const CIPv4 & IP )
    : CRec(name) , m_IP(IP) {};
    string Type( void ) const override
    {
      return "A";
    }
    ostream & Print( ostream & os ) const override
    {
      return os << m_Name << " " << "A" << " " << m_IP;
    }
    CIPv4 GetIP( void ) const
    {
      return m_IP;
    }
    bool Equals( const CRec & cmp ) const override
    {
      if ( cmp.Type() != this->Type() )
        return false;
      if ( cmp.Name() != this->Name() )
        return false;
      CRecA cmpA = (CRecA&)cmp;
      if ( !(cmpA.GetIP()==this->GetIP()) )
        return false;
      return true;
    }
    
  private:
    CIPv4 m_IP;
};

class CRecAAAA : public CRec
{
  public:
    CRecAAAA( const string & name , const CIPv6 & IP )
    : CRec(name) , m_IP(IP) {};
    
    string Type( void ) const override
    {
      return "AAAA";
    }
    ostream & Print( ostream & os ) const override
    {
      return os << m_Name << " " << "AAAA" << " " << m_IP;
    }
    CIPv6 GetIP( void ) const
    {
      return m_IP;
    }
    bool Equals( const CRec & cmp ) const override
    {
      if ( cmp.Type() != this->Type() )
        return false;
      if ( cmp.Name() != this->Name() )
        return false;
      CRecAAAA cmpA = (CRecAAAA&)cmp;
      if ( !(cmpA.GetIP()==this->GetIP()) )
        return false;
      return true;
    }
  private:
    CIPv6 m_IP;
};

class CRecMX: public CRec
{
  public:
    CRecMX( const string & name , const string & domain , const int ID )
    : CRec(name) , m_Domain(domain) , m_NevimCoToJe(ID) {};
    
    string Type( void ) const  override
    {
      return "MX";
    }
    ostream & Print( ostream & os ) const override
    {
      return os << m_Name << " " << "MX" << " " << m_NevimCoToJe << " " << m_Domain;
    }
    string GetDomain( void ) const
    {
      return m_Domain;
    }
    int GetInt( void ) const
    {
      return m_NevimCoToJe;
    }
    bool Equals( const CRec & cmp ) const override
    {
      if ( cmp.Type() != this->Type() )
        return false;
      if ( cmp.Name() != this->Name() )
        return false;
      CRecMX cmpA = (CRecMX&)cmp;
      if ( !(cmpA.GetDomain()==this->GetDomain()) )
        return false;
      if ( !(cmpA.GetInt()==this->GetInt()) )
        return false;
      return true;
    }
    private:
      string m_Domain;
      int m_NevimCoToJe;
};

struct SOut
{
  SOut(){};

  size_t Count( void ) const
  {
    return m_Rec.size();
  }
  friend ostream & operator << ( ostream & os , const SOut & out )
  {
    for ( auto i : out.m_Rec )
    {
      os << *i << endl ;
    }
    return os;
  }

  CRec & operator [] ( size_t pos )
  {
    if ( pos >= m_Rec.size() )
      throw out_of_range(" ");
    return *m_Rec[pos];
  }

  CRec & operator [] ( size_t pos ) const
  {
    if ( pos >= m_Rec.size() )
      throw out_of_range(" ");
    return *m_Rec[pos];
  }

  vector<shared_ptr<CRec>> m_Rec;
};

class CZone
{
  public:
    // constructor(s)
    CZone( const string & name ) :
    m_Name (name) {};

    bool Add( const CRec & rec )
    {
      for ( auto i : m_Rec )
      {
        if ( i->Equals(rec) )
          return false;
      }
      if (rec.Type() == "A" )
      {
        CRecA recA = (CRecA&)rec;
        m_Rec.push_back(make_shared<CRecA>(recA));
      }
      else if ( rec.Type() == "AAAA")
      {
        CRecAAAA recA = (CRecAAAA&)rec;
        m_Rec.push_back(make_shared<CRecAAAA>(recA));
      }
      else if ( rec.Type() == "MX" )
      {
        CRecMX recA = (CRecMX&)rec;
        m_Rec.push_back(make_shared<CRecMX>(recA));
      }
      
      return true;
    }

    bool Del( const CRec & rec )
    {
      for ( auto i = m_Rec.begin() ; i!=m_Rec.end() ; i++ )
      {
        if ( (*i)->Equals(rec) )
        {
          m_Rec.erase(i);
          return true;
        }
      }
      return false;
    }
    // Search ()
    SOut Search( const string & find )
    {
      SOut ret;
      for ( auto i : m_Rec )
      {
        if ( i->Name() == find )
        {
          ret.m_Rec.emplace_back(i);
        }
      }
      return ret;
    }

    friend ostream & operator << ( ostream & is , const CZone & out )
    {
      is << out.m_Name << endl;

      for ( size_t i = 0 ; i < out.m_Rec.size() ; i++ )
      {
        if ( i != out.m_Rec.size()-1 )
          is << " +- ";
        else 
          is << " \\- ";
        is << *(out.m_Rec[i]) << endl;
      }
      return is;
    }

    private:
      string m_Name;
      vector <shared_ptr<CRec>> m_Rec;
};
#ifndef __PROGTEST__
int main ( void )
{
  ostringstream oss;

  CZone z0 ( "fit.cvut.cz" );
  assert ( z0 . Add ( CRecA ( "progtest", CIPv4 ( "147.32.232.142" ) ) ) == true );
  assert ( z0 . Add ( CRecAAAA ( "progtest", CIPv6 ( "2001:718:2:2902:0:1:2:3" ) ) ) == true );
  assert ( z0 . Add ( CRecA ( "courses", CIPv4 ( "147.32.232.158" ) ) ) == true );
  assert ( z0 . Add ( CRecA ( "courses", CIPv4 ( "147.32.232.160" ) ) ) == true );
  assert ( z0 . Add ( CRecA ( "courses", CIPv4 ( "147.32.232.159" ) ) ) == true );
  assert ( z0 . Add ( CRecAAAA ( "progtest", CIPv6 ( "2001:718:2:2902:1:2:3:4" ) ) ) == true );
  assert ( z0 . Add ( CRecMX ( "courses", "relay.fit.cvut.cz.", 0 ) ) == true );
  assert ( z0 . Add ( CRecMX ( "courses", "relay2.fit.cvut.cz.", 10 ) ) == true );
  oss . str ( "" );
  oss << z0;
  //cout << z0;
  assert ( oss . str () == 
    "fit.cvut.cz\n"
    " +- progtest A 147.32.232.142\n"
    " +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
    " +- courses A 147.32.232.158\n"
    " +- courses A 147.32.232.160\n"
    " +- courses A 147.32.232.159\n"
    " +- progtest AAAA 2001:718:2:2902:1:2:3:4\n"
    " +- courses MX 0 relay.fit.cvut.cz.\n"
    " \\- courses MX 10 relay2.fit.cvut.cz.\n" );
  assert ( z0 . Search ( "progtest" ) . Count () == 3 );
  oss . str ( "" );
  oss << z0 . Search ( "progtest" );
  
  assert ( oss . str () == 
    "progtest A 147.32.232.142\n"
    "progtest AAAA 2001:718:2:2902:0:1:2:3\n"
    "progtest AAAA 2001:718:2:2902:1:2:3:4\n" );
  assert ( z0 . Del ( CRecA ( "courses", CIPv4 ( "147.32.232.160" ) ) ) == true );
  assert ( z0 . Add ( CRecA ( "courses", CIPv4 ( "147.32.232.122" ) ) ) == true );
  oss . str ( "" );
  oss << z0;
  assert ( oss . str () == 
    "fit.cvut.cz\n"
    " +- progtest A 147.32.232.142\n"
    " +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
    " +- courses A 147.32.232.158\n"
    " +- courses A 147.32.232.159\n"
    " +- progtest AAAA 2001:718:2:2902:1:2:3:4\n"
    " +- courses MX 0 relay.fit.cvut.cz.\n"
    " +- courses MX 10 relay2.fit.cvut.cz.\n"
    " \\- courses A 147.32.232.122\n" );
  assert ( z0 . Search ( "courses" ) . Count () == 5 );
  oss . str ( "" );
  oss << z0 . Search ( "courses" );
  assert ( oss . str () == 
    "courses A 147.32.232.158\n"
    "courses A 147.32.232.159\n"
    "courses MX 0 relay.fit.cvut.cz.\n"
    "courses MX 10 relay2.fit.cvut.cz.\n"
    "courses A 147.32.232.122\n" );
  oss . str ( "" );
  oss << z0 . Search ( "courses" ) [ 0 ];
  assert ( oss . str () == "courses A 147.32.232.158" );
  assert ( z0 . Search ( "courses" ) [ 0 ] . Name () == "courses" );
  assert ( z0 . Search ( "courses" ) [ 0 ] . Type () == "A" );
  oss . str ( "" );
  oss << z0 . Search ( "courses" ) [ 1 ];
  assert ( oss . str () == "courses A 147.32.232.159" );
  assert ( z0 . Search ( "courses" ) [ 1 ] . Name () == "courses" );
  assert ( z0 . Search ( "courses" ) [ 1 ] . Type () == "A" );
  oss . str ( "" );
  oss << z0 . Search ( "courses" ) [ 2 ];
  assert ( oss . str () == "courses MX 0 relay.fit.cvut.cz." );
  assert ( z0 . Search ( "courses" ) [ 2 ] . Name () == "courses" );
  assert ( z0 . Search ( "courses" ) [ 2 ] . Type () == "MX" );
  try
  {
    oss . str ( "" );
    oss << z0 . Search ( "courses" ) [ 10 ];
    assert ( "No exception thrown!" == nullptr );
  }
  catch ( const out_of_range & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception thrown!" == nullptr );
  }
  dynamic_cast<const CRecAAAA &> ( z0 . Search ( "progtest" ) [ 1 ] );
  CZone z1 ( "fit2.cvut.cz" );
  z1 . Add ( z0 . Search ( "progtest" ) [ 2 ] );
  z1 . Add ( z0 . Search ( "progtest" ) [ 0 ] );
  z1 . Add ( z0 . Search ( "progtest" ) [ 1 ] );
  z1 . Add ( z0 . Search ( "courses" ) [ 2 ] );
  oss . str ( "" );
  oss << z1;
  assert ( oss . str () == 
    "fit2.cvut.cz\n"
    " +- progtest AAAA 2001:718:2:2902:1:2:3:4\n"
    " +- progtest A 147.32.232.142\n"
    " +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
    " \\- courses MX 0 relay.fit.cvut.cz.\n" );
  dynamic_cast<const CRecA &> ( z1 . Search ( "progtest" ) [ 1 ] );
  
  CZone z2 ( "fit.cvut.cz" );
  assert ( z2 . Add ( CRecA ( "progtest", CIPv4 ( "147.32.232.142" ) ) ) == true );
  assert ( z2 . Add ( CRecAAAA ( "progtest", CIPv6 ( "2001:718:2:2902:0:1:2:3" ) ) ) == true );
  assert ( z2 . Add ( CRecA ( "progtest", CIPv4 ( "147.32.232.144" ) ) ) == true );
  assert ( z2 . Add ( CRecMX ( "progtest", "relay.fit.cvut.cz.", 10 ) ) == true );
  assert ( z2 . Add ( CRecA ( "progtest", CIPv4 ( "147.32.232.142" ) ) ) == false );
  assert ( z2 . Del ( CRecA ( "progtest", CIPv4 ( "147.32.232.140" ) ) ) == false );
  assert ( z2 . Del ( CRecA ( "progtest", CIPv4 ( "147.32.232.142" ) ) ) == true );
  assert ( z2 . Del ( CRecA ( "progtest", CIPv4 ( "147.32.232.142" ) ) ) == false );
  assert ( z2 . Add ( CRecMX ( "progtest", "relay.fit.cvut.cz.", 20 ) ) == true );
  assert ( z2 . Add ( CRecMX ( "progtest", "relay.fit.cvut.cz.", 10 ) ) == false );
  oss . str ( "" ); 
  oss << z2;
  assert ( oss . str () == 
    "fit.cvut.cz\n"
    " +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
    " +- progtest A 147.32.232.144\n"
    " +- progtest MX 10 relay.fit.cvut.cz.\n"
    " \\- progtest MX 20 relay.fit.cvut.cz.\n" );
  assert ( z2 . Search ( "progtest" ) . Count () == 4 );
  oss . str ( "" );
  oss << z2 . Search ( "progtest" );
  assert ( oss . str () == 
    "progtest AAAA 2001:718:2:2902:0:1:2:3\n"
    "progtest A 147.32.232.144\n"
    "progtest MX 10 relay.fit.cvut.cz.\n"
    "progtest MX 20 relay.fit.cvut.cz.\n" );
  assert ( z2 . Search ( "courses" ) . Count () == 0 );
  oss . str ( "" );
  oss << z2 . Search ( "courses" );
  assert ( oss . str () == "" );
  try
  {
    dynamic_cast<const CRecMX &> ( z2 . Search ( "progtest" ) [ 0 ] );
    assert ( "Invalid type" == nullptr );
  }
  catch ( const bad_cast & e )
  {
  }

  CZone z4 ( "fit.cvut.cz" );
  assert ( z4 . Add ( CRecA ( "progtest", CIPv4 ( "147.32.232.142" ) ) ) == true );
  assert ( z4 . Add ( CRecA ( "courses", CIPv4 ( "147.32.232.158" ) ) ) == true );
  assert ( z4 . Add ( CRecA ( "courses", CIPv4 ( "147.32.232.160" ) ) ) == true );
  assert ( z4 . Add ( CRecA ( "courses", CIPv4 ( "147.32.232.159" ) ) ) == true );
  CZone z5 ( z4 );
  assert ( z4 . Add ( CRecAAAA ( "progtest", CIPv6 ( "2001:718:2:2902:0:1:2:3" ) ) ) == true );
  assert ( z4 . Add ( CRecAAAA ( "progtest", CIPv6 ( "2001:718:2:2902:1:2:3:4" ) ) ) == true );
  assert ( z5 . Del ( CRecA ( "progtest", CIPv4 ( "147.32.232.142" ) ) ) == true );
  oss . str ( "" );
  oss << z4;
  assert ( oss . str () == 
    "fit.cvut.cz\n"
    " +- progtest A 147.32.232.142\n"
    " +- courses A 147.32.232.158\n"
    " +- courses A 147.32.232.160\n"
    " +- courses A 147.32.232.159\n"
    " +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
    " \\- progtest AAAA 2001:718:2:2902:1:2:3:4\n" );
  oss . str ( "" );
  oss << z5;
  assert ( oss . str () == 
    "fit.cvut.cz\n"
    " +- courses A 147.32.232.158\n"
    " +- courses A 147.32.232.160\n"
    " \\- courses A 147.32.232.159\n" );
  z5 = z4;
  assert ( z4 . Add ( CRecMX ( "courses", "relay.fit.cvut.cz.", 0 ) ) == true );
  assert ( z4 . Add ( CRecMX ( "courses", "relay2.fit.cvut.cz.", 10 ) ) == true );
  oss . str ( "" );
  oss << z4;
  assert ( oss . str () == 
    "fit.cvut.cz\n"
    " +- progtest A 147.32.232.142\n"
    " +- courses A 147.32.232.158\n"
    " +- courses A 147.32.232.160\n"
    " +- courses A 147.32.232.159\n"
    " +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
    " +- progtest AAAA 2001:718:2:2902:1:2:3:4\n"
    " +- courses MX 0 relay.fit.cvut.cz.\n"
    " \\- courses MX 10 relay2.fit.cvut.cz.\n" );
  oss . str ( "" );
  oss << z5;
  assert ( oss . str () == 
    "fit.cvut.cz\n"
    " +- progtest A 147.32.232.142\n"
    " +- courses A 147.32.232.158\n"
    " +- courses A 147.32.232.160\n"
    " +- courses A 147.32.232.159\n"
    " +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
    " \\- progtest AAAA 2001:718:2:2902:1:2:3:4\n" ); 

  return 0;
}
#endif /* __PROGTEST__ */
