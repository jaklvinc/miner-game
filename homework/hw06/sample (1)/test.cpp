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
    virtual ostream & Print( ostream & os , const string & prefix ) const = 0;
    friend ostream & operator << ( ostream & os , const CRec & out )
    {
      return out.Print(os,"");
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
    ostream & Print( ostream & os , const string & prefix ) const override
    {
      return os << prefix <<  m_Name << " " << "A" << " " << m_IP;
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
    ostream & Print( ostream & os, const string & prefix) const override
    {
      return os << prefix << m_Name << " " << "AAAA" << " " << m_IP;
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
    ostream & Print( ostream & os , const string & prefix ) const override
    {
      return os << prefix << m_Name << " " << "MX" << " " << m_NevimCoToJe << " " << m_Domain;
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

class CRecCNAME : public CRec
{
  public:
    CRecCNAME( const string & name , const string & domain )
    : CRec(name) , m_Domain(domain) {};
    string Type( void ) const  override
    {
      return "CNAME";
    }
    ostream & Print( ostream & os , const string & prefix ) const override
    {
      return os << prefix << m_Name << " " << "CNAME" << " " << m_Domain;
    }
    bool Equals( const CRec & cmp ) const override
    {
      if ( cmp.Name() != this->Name() )
        return false;
      return false;
    }
    private:
      string m_Domain;
};

class CRecSPF : public CRec
{
  public:
    CRecSPF( const string & name )
    : CRec(name) {};
    string Type( void ) const  override
    {
      return "SPF";
    }
    CRecSPF & Add( string toAdd )
    {
      m_Addresses.push_back(toAdd);
      return *this;
    }
    ostream & Print( ostream & os , const string & prefix) const override
    {
      os << prefix << m_Name << " " << "SPF" << " ";
      for ( size_t i = 0 ; i < m_Addresses.size()-1 ; i++ )
      {
        os << m_Addresses[i] << ", ";
      }
      os << m_Addresses[m_Addresses.size()-1];
      return os;
    }
    
    bool Equals( const CRec & cmp ) const override
    {
      if ( cmp.Type() != this->Type() )
        return false;
      if ( cmp.Name() != this->Name() )
        return false;
      return true;
    }
  private:
    vector<string> m_Addresses;
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

class CZone : public CRec
{
  public:
    int m_Odsazeni;
    // constructor(s)
    CZone( const string & name , const int & odsazeni = 0) :
    CRec(name), m_Odsazeni(odsazeni) {};

    bool Add( const CRec & rec )
    {
      for ( auto i : m_Rec )
      {
        if ( (rec.Type() == "Z" || rec.Type() == "CNAME" || i->Type() == "Z" || i->Type() == "CNAME") && rec.Name() == i->Name() )
          return false;
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
      else if (rec.Type() == "CNAME" )
      {
        CRecCNAME recA = (CRecCNAME&)rec;
        m_Rec.push_back(make_shared<CRecCNAME>(recA));
      }
      else if (rec.Type() == "SPF" )
      {
        CRecSPF recA = (CRecSPF&)rec;
        m_Rec.push_back(make_shared<CRecSPF>(recA));
      }
      else if (rec.Type() == "Z" )
      {
        CZone recA = (CZone&)rec;
        recA.m_Odsazeni+=this->m_Odsazeni+1;
        m_Rec.push_back(make_shared<CZone>(recA));
      }
      
      return true;
    }


    string Type( void ) const
    {
      return "Z";
    }

    bool Equals( const CRec & cmp ) const override
    {
      if ( cmp.Name() != this->Name() )
        return false;
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
    SOut Search( const string & find ) const
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

    ostream & Print ( ostream & is , const string & prefix ) const override
    {
      is << prefix << m_Name;

      for ( size_t i = 0 ; i < m_Rec.size() ; i++ )
      { 
        is << endl;
        if ( i != m_Rec.size()-1 )
        {
          if ( m_Rec[i]->Type() == "Z" )
            m_Rec[i]->Print(is,"    +- ");
          else
            m_Rec[i]->Print(is," +- ");
        }
        else
        {
         if ( m_Rec[i]->Type() == "Z" )
            m_Rec[i]->Print(is,"    \\- ");
          else
            m_Rec[i]->Print(is," \\- ");
        }
      }
      if ( m_Odsazeni == 0)
        is << endl;
      return is;
    }

    private:
      //string m_Name;

      vector <shared_ptr<CRec>> m_Rec;
};


/*class CZone
{
  public:
    // constructor(s)
    // destructor (if needed)
    // operator = (if needed)
    // Name ()
    // Type ()
    // Add ()
    // Del ()
    // Search ()
    // operator <<
    // todo
};*/
#ifndef __PROGTEST__
int main ( void )
{
  ostringstream oss;

  CZone z0 ( "fit" );
  assert ( z0 . Add ( CRecA ( "progtest", CIPv4 ( "147.32.232.142" ) ) ) == true );
  assert ( z0 . Add ( CRecAAAA ( "progtest", CIPv6 ( "2001:718:2:2902:0:1:2:3" ) ) ) == true );
  assert ( z0 . Add ( CRecA ( "courses", CIPv4 ( "147.32.232.158" ) ) ) == true );
  assert ( z0 . Add ( CRecA ( "courses", CIPv4 ( "147.32.232.160" ) ) ) == true );
  assert ( z0 . Add ( CRecA ( "courses", CIPv4 ( "147.32.232.159" ) ) ) == true );
  assert ( z0 . Add ( CRecCNAME ( "pririz", "sto.fit.cvut.cz." ) ) == true );
  assert ( z0 . Add ( CRecSPF ( "courses" ) . Add ( "ip4:147.32.232.128/25" ) . Add ( "ip4:147.32.232.64/26" ) ) == true );
  assert ( z0 . Add ( CRecAAAA ( "progtest", CIPv6 ( "2001:718:2:2902:1:2:3:4" ) ) ) == true );
  assert ( z0 . Add ( CRecMX ( "courses", "relay.fit.cvut.cz.", 0 ) ) == true );
  assert ( z0 . Add ( CRecMX ( "courses", "relay2.fit.cvut.cz.", 10 ) ) == true );
  oss . str ( "" );
  oss << z0;
  cout << z0;
  assert ( oss . str () == 
    "fit\n"
    " +- progtest A 147.32.232.142\n"
    " +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
    " +- courses A 147.32.232.158\n"
    " +- courses A 147.32.232.160\n"
    " +- courses A 147.32.232.159\n"
    " +- pririz CNAME sto.fit.cvut.cz.\n"
    " +- courses SPF ip4:147.32.232.128/25, ip4:147.32.232.64/26\n"
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
    "fit\n"
    " +- progtest A 147.32.232.142\n"
    " +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
    " +- courses A 147.32.232.158\n"
    " +- courses A 147.32.232.159\n"
    " +- pririz CNAME sto.fit.cvut.cz.\n"
    " +- courses SPF ip4:147.32.232.128/25, ip4:147.32.232.64/26\n"
    " +- progtest AAAA 2001:718:2:2902:1:2:3:4\n"
    " +- courses MX 0 relay.fit.cvut.cz.\n"
    " +- courses MX 10 relay2.fit.cvut.cz.\n"
    " \\- courses A 147.32.232.122\n" );
  assert ( z0 . Search ( "courses" ) . Count () == 6 );
  oss . str ( "" );
  oss << z0 . Search ( "courses" );
  assert ( oss . str () == 
    "courses A 147.32.232.158\n"
    "courses A 147.32.232.159\n"
    "courses SPF ip4:147.32.232.128/25, ip4:147.32.232.64/26\n"
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
  assert ( oss . str () == "courses SPF ip4:147.32.232.128/25, ip4:147.32.232.64/26" );
  assert ( z0 . Search ( "courses" ) [ 2 ] . Name () == "courses" );
  assert ( z0 . Search ( "courses" ) [ 2 ] . Type () == "SPF" );
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
  CZone z1 ( "fit2" );
  z1 . Add ( z0 . Search ( "progtest" ) [ 2 ] );
  z1 . Add ( z0 . Search ( "progtest" ) [ 0 ] );
  z1 . Add ( z0 . Search ( "progtest" ) [ 1 ] );
  z1 . Add ( z0 . Search ( "courses" ) [ 2 ] );
  oss . str ( "" );
  oss << z1;
  assert ( oss . str () == 
    "fit2\n"
    " +- progtest AAAA 2001:718:2:2902:1:2:3:4\n"
    " +- progtest A 147.32.232.142\n"
    " +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
    " \\- courses SPF ip4:147.32.232.128/25, ip4:147.32.232.64/26\n" );
  dynamic_cast<const CRecA &> ( z1 . Search ( "progtest" ) [ 1 ] );
  
  CZone z10 ( "fit" );
  assert ( z10 . Add ( CRecA ( "progtest", CIPv4 ( "147.32.232.142" ) ) ) == true );
  assert ( z10 . Add ( CRecAAAA ( "progtest", CIPv6 ( "2001:718:2:2902:0:1:2:3" ) ) ) == true );
  assert ( z10 . Add ( CRecA ( "progtest", CIPv4 ( "147.32.232.144" ) ) ) == true );
  assert ( z10 . Add ( CRecMX ( "progtest", "relay.fit.cvut.cz.", 10 ) ) == true );
  assert ( z10 . Add ( CRecA ( "progtest", CIPv4 ( "147.32.232.142" ) ) ) == false );
  assert ( z10 . Del ( CRecA ( "progtest", CIPv4 ( "147.32.232.140" ) ) ) == false );
  assert ( z10 . Del ( CRecA ( "progtest", CIPv4 ( "147.32.232.142" ) ) ) == true );
  assert ( z10 . Del ( CRecA ( "progtest", CIPv4 ( "147.32.232.142" ) ) ) == false );
  assert ( z10 . Add ( CRecMX ( "progtest", "relay.fit.cvut.cz.", 20 ) ) == true );
  assert ( z10 . Add ( CRecMX ( "progtest", "relay.fit.cvut.cz.", 10 ) ) == false );
  assert ( z10 . Add ( CRecCNAME ( "pririz", "sto.fit.cvut.cz." ) ) == true );
  assert ( z10 . Add ( CRecCNAME ( "pririz", "stojedna.fit.cvut.cz." ) ) == false );
  assert ( z10 . Add ( CRecA ( "pririz", CIPv4 ( "147.32.232.111" ) ) ) == false );
  assert ( z10 . Add ( CRecCNAME ( "progtest", "progtestbak.fit.cvut.cz." ) ) == false );
  assert ( z10 . Add ( CZone ( "test" ) ) == true );
  assert ( z10 . Add ( CZone ( "pririz" ) ) == false );
  assert ( z10 . Add ( CRecA ( "test", CIPv4 ( "147.32.232.232" ) ) ) == false );
  oss . str ( "" );
  oss << z10;
  cout << z10;
  /*assert ( oss . str () == 
    "fit\n"
    " +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
    " +- progtest A 147.32.232.144\n"
    " +- progtest MX 10 relay.fit.cvut.cz.\n"
    " +- progtest MX 20 relay.fit.cvut.cz.\n"
    " +- pririz CNAME sto.fit.cvut.cz.\n"
    " \\- test\n" );*/
  assert ( z10 . Search ( "progtest" ) . Count () == 4 );
  oss . str ( "" );
  oss << z10 . Search ( "progtest" );
  assert ( oss . str () == 
    "progtest AAAA 2001:718:2:2902:0:1:2:3\n"
    "progtest A 147.32.232.144\n"
    "progtest MX 10 relay.fit.cvut.cz.\n"
    "progtest MX 20 relay.fit.cvut.cz.\n" );
  assert ( z10 . Search ( "courses" ) . Count () == 0 );
  oss . str ( "" );
  oss << z10 . Search ( "courses" );
  assert ( oss . str () == "" );

  CZone z20 ( "<ROOT ZONE>" );
  CZone z21 ( "cz" );
  CZone z22 ( "cvut" );
  CZone z23 ( "fit" );
  assert ( z23 . Add ( CRecA ( "progtest", CIPv4 ( "147.32.232.142" ) ) ) == true );
  assert ( z23 . Add ( CRecAAAA ( "progtest", CIPv6 ( "2001:718:2:2902:0:1:2:3" ) ) ) == true );
  assert ( z23 . Add ( CRecA ( "courses", CIPv4 ( "147.32.232.158" ) ) ) == true );
  assert ( z23 . Add ( CRecA ( "courses", CIPv4 ( "147.32.232.160" ) ) ) == true );
  assert ( z23 . Add ( CRecA ( "courses", CIPv4 ( "147.32.232.159" ) ) ) == true );
  assert ( z23 . Add ( CRecCNAME ( "pririz", "sto.fit.cvut.cz." ) ) == true );
  assert ( z23 . Add ( CRecSPF ( "courses" ) . Add ( "ip4:147.32.232.128/25" ) . Add ( "ip4:147.32.232.64/26" ) ) == true );
  CZone z24 ( "fel" );
  assert ( z24 . Add ( CRecA ( "www", CIPv4 ( "147.32.80.2" ) ) ) == true );
  assert ( z24 . Add ( CRecAAAA ( "www", CIPv6 ( "1:2:3:4:5:6:7:8" ) ) ) == true );
  assert ( z22 . Add ( z23 ) == true );
  assert ( z22 . Add ( z24 ) == true );
  assert ( z21 . Add ( z22 ) == true );
  assert ( z20 . Add ( z21 ) == true );
  assert ( z23 . Add ( CRecA ( "www", CIPv4 ( "147.32.90.1" ) ) ) == true );
  oss . str ( "" );
  oss << z20;
  cout << z20;
  assert ( oss . str () == 
    "<ROOT ZONE>\n"
    " \\- cz\n"
    "    \\- cvut\n"
    "       +- fit\n"
    "       |  +- progtest A 147.32.232.142\n"
    "       |  +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
    "       |  +- courses A 147.32.232.158\n"
    "       |  +- courses A 147.32.232.160\n"
    "       |  +- courses A 147.32.232.159\n"
    "       |  +- pririz CNAME sto.fit.cvut.cz.\n"
    "       |  \\- courses SPF ip4:147.32.232.128/25, ip4:147.32.232.64/26\n"
    "       \\- fel\n"
    "          +- www A 147.32.80.2\n"
    "          \\- www AAAA 1:2:3:4:5:6:7:8\n" );
  oss . str ( "" );
  oss << z21;
  assert ( oss . str () == 
    "cz\n"
    " \\- cvut\n"
    "    +- fit\n"
    "    |  +- progtest A 147.32.232.142\n"
    "    |  +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
    "    |  +- courses A 147.32.232.158\n"
    "    |  +- courses A 147.32.232.160\n"
    "    |  +- courses A 147.32.232.159\n"
    "    |  +- pririz CNAME sto.fit.cvut.cz.\n"
    "    |  \\- courses SPF ip4:147.32.232.128/25, ip4:147.32.232.64/26\n"
    "    \\- fel\n"
    "       +- www A 147.32.80.2\n"
    "       \\- www AAAA 1:2:3:4:5:6:7:8\n" );
  oss . str ( "" );
  oss << z22;
  assert ( oss . str () == 
    "cvut\n"
    " +- fit\n"
    " |  +- progtest A 147.32.232.142\n"
    " |  +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
    " |  +- courses A 147.32.232.158\n"
    " |  +- courses A 147.32.232.160\n"
    " |  +- courses A 147.32.232.159\n"
    " |  +- pririz CNAME sto.fit.cvut.cz.\n"
    " |  \\- courses SPF ip4:147.32.232.128/25, ip4:147.32.232.64/26\n"
    " \\- fel\n"
    "    +- www A 147.32.80.2\n"
    "    \\- www AAAA 1:2:3:4:5:6:7:8\n" );
  oss . str ( "" );
  oss << z23;
  assert ( oss . str () == 
    "fit\n"
    " +- progtest A 147.32.232.142\n"
    " +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
    " +- courses A 147.32.232.158\n"
    " +- courses A 147.32.232.160\n"
    " +- courses A 147.32.232.159\n"
    " +- pririz CNAME sto.fit.cvut.cz.\n"
    " +- courses SPF ip4:147.32.232.128/25, ip4:147.32.232.64/26\n"
    " \\- www A 147.32.90.1\n" );
  oss . str ( "" );
  oss << z24;
  assert ( oss . str () == 
    "fel\n"
    " +- www A 147.32.80.2\n"
    " \\- www AAAA 1:2:3:4:5:6:7:8\n" );
  assert ( z20 . Search ( "progtest.fit.cvut.cz" ) . Count () == 2 );
  oss . str ( "" );
  oss << z20 . Search ( "progtest.fit.cvut.cz" );
  assert ( oss . str () == 
    "progtest A 147.32.232.142\n"
    "progtest AAAA 2001:718:2:2902:0:1:2:3\n" );
  assert ( z20 . Search ( "fit.cvut.cz" ) . Count () == 1 );
  oss . str ( "" );
  oss << z20 . Search ( "fit.cvut.cz" );
  assert ( oss . str () == 
    "fit\n"
    " +- progtest A 147.32.232.142\n"
    " +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
    " +- courses A 147.32.232.158\n"
    " +- courses A 147.32.232.160\n"
    " +- courses A 147.32.232.159\n"
    " +- pririz CNAME sto.fit.cvut.cz.\n"
    " \\- courses SPF ip4:147.32.232.128/25, ip4:147.32.232.64/26\n" );
  assert ( dynamic_cast<CZone &> ( z20 . Search ( "fit.cvut.cz" ) [0] ) . Add ( z20 . Search ( "fel.cvut.cz" ) [0] ) == true );
  oss . str ( "" );
  oss << z20;
  assert ( oss . str () == 
    "<ROOT ZONE>\n"
    " \\- cz\n"
    "    \\- cvut\n"
    "       +- fit\n"
    "       |  +- progtest A 147.32.232.142\n"
    "       |  +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
    "       |  +- courses A 147.32.232.158\n"
    "       |  +- courses A 147.32.232.160\n"
    "       |  +- courses A 147.32.232.159\n"
    "       |  +- pririz CNAME sto.fit.cvut.cz.\n"
    "       |  +- courses SPF ip4:147.32.232.128/25, ip4:147.32.232.64/26\n"
    "       |  \\- fel\n"
    "       |     +- www A 147.32.80.2\n"
    "       |     \\- www AAAA 1:2:3:4:5:6:7:8\n"
    "       \\- fel\n"
    "          +- www A 147.32.80.2\n"
    "          \\- www AAAA 1:2:3:4:5:6:7:8\n" );
  assert ( dynamic_cast<CZone &> ( z20 . Search ( "fit.cvut.cz" ) [0] ) . Add ( z20 . Search ( "cz" ) [0] ) == true );
  oss . str ( "" );
  oss << z20;
  assert ( oss . str () == 
    "<ROOT ZONE>\n"
    " \\- cz\n"
    "    \\- cvut\n"
    "       +- fit\n"
    "       |  +- progtest A 147.32.232.142\n"
    "       |  +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
    "       |  +- courses A 147.32.232.158\n"
    "       |  +- courses A 147.32.232.160\n"
    "       |  +- courses A 147.32.232.159\n"
    "       |  +- pririz CNAME sto.fit.cvut.cz.\n"
    "       |  +- courses SPF ip4:147.32.232.128/25, ip4:147.32.232.64/26\n"
    "       |  +- fel\n"
    "       |  |  +- www A 147.32.80.2\n"
    "       |  |  \\- www AAAA 1:2:3:4:5:6:7:8\n"
    "       |  \\- cz\n"
    "       |     \\- cvut\n"
    "       |        +- fit\n"
    "       |        |  +- progtest A 147.32.232.142\n"
    "       |        |  +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
    "       |        |  +- courses A 147.32.232.158\n"
    "       |        |  +- courses A 147.32.232.160\n"
    "       |        |  +- courses A 147.32.232.159\n"
    "       |        |  +- pririz CNAME sto.fit.cvut.cz.\n"
    "       |        |  +- courses SPF ip4:147.32.232.128/25, ip4:147.32.232.64/26\n"
    "       |        |  \\- fel\n"
    "       |        |     +- www A 147.32.80.2\n"
    "       |        |     \\- www AAAA 1:2:3:4:5:6:7:8\n"
    "       |        \\- fel\n"
    "       |           +- www A 147.32.80.2\n"
    "       |           \\- www AAAA 1:2:3:4:5:6:7:8\n"
    "       \\- fel\n"
    "          +- www A 147.32.80.2\n"
    "          \\- www AAAA 1:2:3:4:5:6:7:8\n" );
  assert ( dynamic_cast<CZone &> ( z20 . Search ( "fit.cvut.cz.fit.cvut.cz" ) [0] ) . Add ( z20 . Search ( "cz" ) [0] ) == true );
  oss . str ( "" );
  oss << z20;
  assert ( oss . str () == 
    "<ROOT ZONE>\n"
    " \\- cz\n"
    "    \\- cvut\n"
    "       +- fit\n"
    "       |  +- progtest A 147.32.232.142\n"
    "       |  +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
    "       |  +- courses A 147.32.232.158\n"
    "       |  +- courses A 147.32.232.160\n"
    "       |  +- courses A 147.32.232.159\n"
    "       |  +- pririz CNAME sto.fit.cvut.cz.\n"
    "       |  +- courses SPF ip4:147.32.232.128/25, ip4:147.32.232.64/26\n"
    "       |  +- fel\n"
    "       |  |  +- www A 147.32.80.2\n"
    "       |  |  \\- www AAAA 1:2:3:4:5:6:7:8\n"
    "       |  \\- cz\n"
    "       |     \\- cvut\n"
    "       |        +- fit\n"
    "       |        |  +- progtest A 147.32.232.142\n"
    "       |        |  +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
    "       |        |  +- courses A 147.32.232.158\n"
    "       |        |  +- courses A 147.32.232.160\n"
    "       |        |  +- courses A 147.32.232.159\n"
    "       |        |  +- pririz CNAME sto.fit.cvut.cz.\n"
    "       |        |  +- courses SPF ip4:147.32.232.128/25, ip4:147.32.232.64/26\n"
    "       |        |  +- fel\n"
    "       |        |  |  +- www A 147.32.80.2\n"
    "       |        |  |  \\- www AAAA 1:2:3:4:5:6:7:8\n"
    "       |        |  \\- cz\n"
    "       |        |     \\- cvut\n"
    "       |        |        +- fit\n"
    "       |        |        |  +- progtest A 147.32.232.142\n"
    "       |        |        |  +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
    "       |        |        |  +- courses A 147.32.232.158\n"
    "       |        |        |  +- courses A 147.32.232.160\n"
    "       |        |        |  +- courses A 147.32.232.159\n"
    "       |        |        |  +- pririz CNAME sto.fit.cvut.cz.\n"
    "       |        |        |  +- courses SPF ip4:147.32.232.128/25, ip4:147.32.232.64/26\n"
    "       |        |        |  +- fel\n"
    "       |        |        |  |  +- www A 147.32.80.2\n"
    "       |        |        |  |  \\- www AAAA 1:2:3:4:5:6:7:8\n"
    "       |        |        |  \\- cz\n"
    "       |        |        |     \\- cvut\n"
    "       |        |        |        +- fit\n"
    "       |        |        |        |  +- progtest A 147.32.232.142\n"
    "       |        |        |        |  +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
    "       |        |        |        |  +- courses A 147.32.232.158\n"
    "       |        |        |        |  +- courses A 147.32.232.160\n"
    "       |        |        |        |  +- courses A 147.32.232.159\n"
    "       |        |        |        |  +- pririz CNAME sto.fit.cvut.cz.\n"
    "       |        |        |        |  +- courses SPF ip4:147.32.232.128/25, ip4:147.32.232.64/26\n"
    "       |        |        |        |  \\- fel\n"
    "       |        |        |        |     +- www A 147.32.80.2\n"
    "       |        |        |        |     \\- www AAAA 1:2:3:4:5:6:7:8\n"
    "       |        |        |        \\- fel\n"
    "       |        |        |           +- www A 147.32.80.2\n"
    "       |        |        |           \\- www AAAA 1:2:3:4:5:6:7:8\n"
    "       |        |        \\- fel\n"
    "       |        |           +- www A 147.32.80.2\n"
    "       |        |           \\- www AAAA 1:2:3:4:5:6:7:8\n"
    "       |        \\- fel\n"
    "       |           +- www A 147.32.80.2\n"
    "       |           \\- www AAAA 1:2:3:4:5:6:7:8\n"
    "       \\- fel\n"
    "          +- www A 147.32.80.2\n"
    "          \\- www AAAA 1:2:3:4:5:6:7:8\n" );
  assert ( dynamic_cast<CZone &> ( z20 . Search ( "fit.cvut.cz.fit.cvut.cz" ) [0] ) . Del ( CZone ( "fel" ) ) == true );
  oss . str ( "" );
  oss << z20;
  assert ( oss . str () == 
    "<ROOT ZONE>\n"
    " \\- cz\n"
    "    \\- cvut\n"
    "       +- fit\n"
    "       |  +- progtest A 147.32.232.142\n"
    "       |  +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
    "       |  +- courses A 147.32.232.158\n"
    "       |  +- courses A 147.32.232.160\n"
    "       |  +- courses A 147.32.232.159\n"
    "       |  +- pririz CNAME sto.fit.cvut.cz.\n"
    "       |  +- courses SPF ip4:147.32.232.128/25, ip4:147.32.232.64/26\n"
    "       |  +- fel\n"
    "       |  |  +- www A 147.32.80.2\n"
    "       |  |  \\- www AAAA 1:2:3:4:5:6:7:8\n"
    "       |  \\- cz\n"
    "       |     \\- cvut\n"
    "       |        +- fit\n"
    "       |        |  +- progtest A 147.32.232.142\n"
    "       |        |  +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
    "       |        |  +- courses A 147.32.232.158\n"
    "       |        |  +- courses A 147.32.232.160\n"
    "       |        |  +- courses A 147.32.232.159\n"
    "       |        |  +- pririz CNAME sto.fit.cvut.cz.\n"
    "       |        |  +- courses SPF ip4:147.32.232.128/25, ip4:147.32.232.64/26\n"
    "       |        |  \\- cz\n"
    "       |        |     \\- cvut\n"
    "       |        |        +- fit\n"
    "       |        |        |  +- progtest A 147.32.232.142\n"
    "       |        |        |  +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
    "       |        |        |  +- courses A 147.32.232.158\n"
    "       |        |        |  +- courses A 147.32.232.160\n"
    "       |        |        |  +- courses A 147.32.232.159\n"
    "       |        |        |  +- pririz CNAME sto.fit.cvut.cz.\n"
    "       |        |        |  +- courses SPF ip4:147.32.232.128/25, ip4:147.32.232.64/26\n"
    "       |        |        |  +- fel\n"
    "       |        |        |  |  +- www A 147.32.80.2\n"
    "       |        |        |  |  \\- www AAAA 1:2:3:4:5:6:7:8\n"
    "       |        |        |  \\- cz\n"
    "       |        |        |     \\- cvut\n"
    "       |        |        |        +- fit\n"
    "       |        |        |        |  +- progtest A 147.32.232.142\n"
    "       |        |        |        |  +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
    "       |        |        |        |  +- courses A 147.32.232.158\n"
    "       |        |        |        |  +- courses A 147.32.232.160\n"
    "       |        |        |        |  +- courses A 147.32.232.159\n"
    "       |        |        |        |  +- pririz CNAME sto.fit.cvut.cz.\n"
    "       |        |        |        |  +- courses SPF ip4:147.32.232.128/25, ip4:147.32.232.64/26\n"
    "       |        |        |        |  \\- fel\n"
    "       |        |        |        |     +- www A 147.32.80.2\n"
    "       |        |        |        |     \\- www AAAA 1:2:3:4:5:6:7:8\n"
    "       |        |        |        \\- fel\n"
    "       |        |        |           +- www A 147.32.80.2\n"
    "       |        |        |           \\- www AAAA 1:2:3:4:5:6:7:8\n"
    "       |        |        \\- fel\n"
    "       |        |           +- www A 147.32.80.2\n"
    "       |        |           \\- www AAAA 1:2:3:4:5:6:7:8\n"
    "       |        \\- fel\n"
    "       |           +- www A 147.32.80.2\n"
    "       |           \\- www AAAA 1:2:3:4:5:6:7:8\n"
    "       \\- fel\n"
    "          +- www A 147.32.80.2\n"
    "          \\- www AAAA 1:2:3:4:5:6:7:8\n" );
  CZone z25 ( z20 );
z22 = z20;
  assert ( z20 . Add ( CZone ( "sk" ) ) == true );
  assert ( z25 . Add ( CZone ( "au" ) ) == true );
  assert ( z22 . Add ( CZone ( "de" ) ) == true );
  oss . str ( "" );
  oss << z20;
  assert ( oss . str () == 
    "<ROOT ZONE>\n"
    " +- cz\n"
    " |  \\- cvut\n"
    " |     +- fit\n"
    " |     |  +- progtest A 147.32.232.142\n"
    " |     |  +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
    " |     |  +- courses A 147.32.232.158\n"
    " |     |  +- courses A 147.32.232.160\n"
    " |     |  +- courses A 147.32.232.159\n"
    " |     |  +- pririz CNAME sto.fit.cvut.cz.\n"
    " |     |  +- courses SPF ip4:147.32.232.128/25, ip4:147.32.232.64/26\n"
    " |     |  +- fel\n"
    " |     |  |  +- www A 147.32.80.2\n"
    " |     |  |  \\- www AAAA 1:2:3:4:5:6:7:8\n"
    " |     |  \\- cz\n"
    " |     |     \\- cvut\n"
    " |     |        +- fit\n"
    " |     |        |  +- progtest A 147.32.232.142\n"
    " |     |        |  +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
    " |     |        |  +- courses A 147.32.232.158\n"
    " |     |        |  +- courses A 147.32.232.160\n"
    " |     |        |  +- courses A 147.32.232.159\n"
    " |     |        |  +- pririz CNAME sto.fit.cvut.cz.\n"
    " |     |        |  +- courses SPF ip4:147.32.232.128/25, ip4:147.32.232.64/26\n"
    " |     |        |  \\- cz\n"
    " |     |        |     \\- cvut\n"
    " |     |        |        +- fit\n"
    " |     |        |        |  +- progtest A 147.32.232.142\n"
    " |     |        |        |  +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
    " |     |        |        |  +- courses A 147.32.232.158\n"
    " |     |        |        |  +- courses A 147.32.232.160\n"
    " |     |        |        |  +- courses A 147.32.232.159\n"
    " |     |        |        |  +- pririz CNAME sto.fit.cvut.cz.\n"
    " |     |        |        |  +- courses SPF ip4:147.32.232.128/25, ip4:147.32.232.64/26\n"
    " |     |        |        |  +- fel\n"
    " |     |        |        |  |  +- www A 147.32.80.2\n"
    " |     |        |        |  |  \\- www AAAA 1:2:3:4:5:6:7:8\n"
    " |     |        |        |  \\- cz\n"
    " |     |        |        |     \\- cvut\n"
    " |     |        |        |        +- fit\n"
    " |     |        |        |        |  +- progtest A 147.32.232.142\n"
    " |     |        |        |        |  +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
    " |     |        |        |        |  +- courses A 147.32.232.158\n"
    " |     |        |        |        |  +- courses A 147.32.232.160\n"
    " |     |        |        |        |  +- courses A 147.32.232.159\n"
    " |     |        |        |        |  +- pririz CNAME sto.fit.cvut.cz.\n"
    " |     |        |        |        |  +- courses SPF ip4:147.32.232.128/25, ip4:147.32.232.64/26\n"
    " |     |        |        |        |  \\- fel\n"
    " |     |        |        |        |     +- www A 147.32.80.2\n"
    " |     |        |        |        |     \\- www AAAA 1:2:3:4:5:6:7:8\n"
    " |     |        |        |        \\- fel\n"
    " |     |        |        |           +- www A 147.32.80.2\n"
    " |     |        |        |           \\- www AAAA 1:2:3:4:5:6:7:8\n"
    " |     |        |        \\- fel\n"
    " |     |        |           +- www A 147.32.80.2\n"
    " |     |        |           \\- www AAAA 1:2:3:4:5:6:7:8\n"
    " |     |        \\- fel\n"
    " |     |           +- www A 147.32.80.2\n"
    " |     |           \\- www AAAA 1:2:3:4:5:6:7:8\n"
    " |     \\- fel\n"
    " |        +- www A 147.32.80.2\n"
    " |        \\- www AAAA 1:2:3:4:5:6:7:8\n"
    " \\- sk\n" );
  oss . str ( "" );
  oss << z22;
  assert ( oss . str () == 
    "<ROOT ZONE>\n"
    " +- cz\n"
    " |  \\- cvut\n"
    " |     +- fit\n"
    " |     |  +- progtest A 147.32.232.142\n"
    " |     |  +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
    " |     |  +- courses A 147.32.232.158\n"
    " |     |  +- courses A 147.32.232.160\n"
    " |     |  +- courses A 147.32.232.159\n"
    " |     |  +- pririz CNAME sto.fit.cvut.cz.\n"
    " |     |  +- courses SPF ip4:147.32.232.128/25, ip4:147.32.232.64/26\n"
    " |     |  +- fel\n"
    " |     |  |  +- www A 147.32.80.2\n"
    " |     |  |  \\- www AAAA 1:2:3:4:5:6:7:8\n"
    " |     |  \\- cz\n"
    " |     |     \\- cvut\n"
    " |     |        +- fit\n"
    " |     |        |  +- progtest A 147.32.232.142\n"
    " |     |        |  +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
    " |     |        |  +- courses A 147.32.232.158\n"
    " |     |        |  +- courses A 147.32.232.160\n"
    " |     |        |  +- courses A 147.32.232.159\n"
    " |     |        |  +- pririz CNAME sto.fit.cvut.cz.\n"
    " |     |        |  +- courses SPF ip4:147.32.232.128/25, ip4:147.32.232.64/26\n"
    " |     |        |  \\- cz\n"
    " |     |        |     \\- cvut\n"
    " |     |        |        +- fit\n"
    " |     |        |        |  +- progtest A 147.32.232.142\n"
    " |     |        |        |  +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
    " |     |        |        |  +- courses A 147.32.232.158\n"
    " |     |        |        |  +- courses A 147.32.232.160\n"
    " |     |        |        |  +- courses A 147.32.232.159\n"
    " |     |        |        |  +- pririz CNAME sto.fit.cvut.cz.\n"
    " |     |        |        |  +- courses SPF ip4:147.32.232.128/25, ip4:147.32.232.64/26\n"
    " |     |        |        |  +- fel\n"
    " |     |        |        |  |  +- www A 147.32.80.2\n"
    " |     |        |        |  |  \\- www AAAA 1:2:3:4:5:6:7:8\n"
    " |     |        |        |  \\- cz\n"
    " |     |        |        |     \\- cvut\n"
    " |     |        |        |        +- fit\n"
    " |     |        |        |        |  +- progtest A 147.32.232.142\n"
    " |     |        |        |        |  +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
    " |     |        |        |        |  +- courses A 147.32.232.158\n"
    " |     |        |        |        |  +- courses A 147.32.232.160\n"
    " |     |        |        |        |  +- courses A 147.32.232.159\n"
    " |     |        |        |        |  +- pririz CNAME sto.fit.cvut.cz.\n"
    " |     |        |        |        |  +- courses SPF ip4:147.32.232.128/25, ip4:147.32.232.64/26\n"
    " |     |        |        |        |  \\- fel\n"
    " |     |        |        |        |     +- www A 147.32.80.2\n"
    " |     |        |        |        |     \\- www AAAA 1:2:3:4:5:6:7:8\n"
    " |     |        |        |        \\- fel\n"
    " |     |        |        |           +- www A 147.32.80.2\n"
    " |     |        |        |           \\- www AAAA 1:2:3:4:5:6:7:8\n"
    " |     |        |        \\- fel\n"
    " |     |        |           +- www A 147.32.80.2\n"
    " |     |        |           \\- www AAAA 1:2:3:4:5:6:7:8\n"
    " |     |        \\- fel\n"
    " |     |           +- www A 147.32.80.2\n"
    " |     |           \\- www AAAA 1:2:3:4:5:6:7:8\n"
    " |     \\- fel\n"
    " |        +- www A 147.32.80.2\n"
    " |        \\- www AAAA 1:2:3:4:5:6:7:8\n"
    " \\- de\n" );
  oss . str ( "" );
  oss << z25;
  assert ( oss . str () == 
    "<ROOT ZONE>\n"
    " +- cz\n"
    " |  \\- cvut\n"
    " |     +- fit\n"
    " |     |  +- progtest A 147.32.232.142\n"
    " |     |  +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
    " |     |  +- courses A 147.32.232.158\n"
    " |     |  +- courses A 147.32.232.160\n"
    " |     |  +- courses A 147.32.232.159\n"
    " |     |  +- pririz CNAME sto.fit.cvut.cz.\n"
    " |     |  +- courses SPF ip4:147.32.232.128/25, ip4:147.32.232.64/26\n"
    " |     |  +- fel\n"
    " |     |  |  +- www A 147.32.80.2\n"
    " |     |  |  \\- www AAAA 1:2:3:4:5:6:7:8\n"
    " |     |  \\- cz\n"
    " |     |     \\- cvut\n"
    " |     |        +- fit\n"
    " |     |        |  +- progtest A 147.32.232.142\n"
    " |     |        |  +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
    " |     |        |  +- courses A 147.32.232.158\n"
    " |     |        |  +- courses A 147.32.232.160\n"
    " |     |        |  +- courses A 147.32.232.159\n"
    " |     |        |  +- pririz CNAME sto.fit.cvut.cz.\n"
    " |     |        |  +- courses SPF ip4:147.32.232.128/25, ip4:147.32.232.64/26\n"
    " |     |        |  \\- cz\n"
    " |     |        |     \\- cvut\n"
    " |     |        |        +- fit\n"
    " |     |        |        |  +- progtest A 147.32.232.142\n"
    " |     |        |        |  +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
    " |     |        |        |  +- courses A 147.32.232.158\n"
    " |     |        |        |  +- courses A 147.32.232.160\n"
    " |     |        |        |  +- courses A 147.32.232.159\n"
    " |     |        |        |  +- pririz CNAME sto.fit.cvut.cz.\n"
    " |     |        |        |  +- courses SPF ip4:147.32.232.128/25, ip4:147.32.232.64/26\n"
    " |     |        |        |  +- fel\n"
    " |     |        |        |  |  +- www A 147.32.80.2\n"
    " |     |        |        |  |  \\- www AAAA 1:2:3:4:5:6:7:8\n"
    " |     |        |        |  \\- cz\n"
    " |     |        |        |     \\- cvut\n"
    " |     |        |        |        +- fit\n"
    " |     |        |        |        |  +- progtest A 147.32.232.142\n"
    " |     |        |        |        |  +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
    " |     |        |        |        |  +- courses A 147.32.232.158\n"
    " |     |        |        |        |  +- courses A 147.32.232.160\n"
    " |     |        |        |        |  +- courses A 147.32.232.159\n"
    " |     |        |        |        |  +- pririz CNAME sto.fit.cvut.cz.\n"
    " |     |        |        |        |  +- courses SPF ip4:147.32.232.128/25, ip4:147.32.232.64/26\n"
    " |     |        |        |        |  \\- fel\n"
    " |     |        |        |        |     +- www A 147.32.80.2\n"
    " |     |        |        |        |     \\- www AAAA 1:2:3:4:5:6:7:8\n"
    " |     |        |        |        \\- fel\n"
    " |     |        |        |           +- www A 147.32.80.2\n"
    " |     |        |        |           \\- www AAAA 1:2:3:4:5:6:7:8\n"
    " |     |        |        \\- fel\n"
    " |     |        |           +- www A 147.32.80.2\n"
    " |     |        |           \\- www AAAA 1:2:3:4:5:6:7:8\n"
    " |     |        \\- fel\n"
    " |     |           +- www A 147.32.80.2\n"
    " |     |           \\- www AAAA 1:2:3:4:5:6:7:8\n"
    " |     \\- fel\n"
    " |        +- www A 147.32.80.2\n"
    " |        \\- www AAAA 1:2:3:4:5:6:7:8\n"
    " \\- au\n" );
  
  return 0;
}
#endif /* __PROGTEST__ */
