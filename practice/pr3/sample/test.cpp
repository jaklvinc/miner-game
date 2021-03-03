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

struct Contact
{
    string m_Name;
    string m_Surname;
    string m_Number;
};

bool report ( const string & fileName, ostream & out )
{
  ifstream file;
  file.open(fileName);
  if (!file)
  {
    return false;
  }
  vector <Contact> Phonebook;
  string Name;
  string Surname;
  string Number;
  string Catch_me;

  string dataline;
  
  while ( getline(file,dataline) )
  {
    if ( dataline == "" )
      break;
    stringstream iss(dataline);
    iss >> Name >> Surname >> Number >> Catch_me;
    
    if ((Name == "") || 
        (Surname == "") || 
        (Number == "") || 
        (Catch_me != "") || 
        (Number.length() != 9) ||
        (Number[0] == '0'))
    {
      //cout << "*" << endl;
      return false;
    }

    for ( auto i : Number )
    {
      if (!isdigit(i))
      {
        //cout << "*" << endl;
        return false;
      }
    }

    Contact contact;
    contact.m_Name=Name;
    contact.m_Surname=Surname;
    contact.m_Number=Number;
    Phonebook.push_back(contact);
  }


  
  while ( getline(file,dataline) )
  {
    /*getline(file,dataline);
    if (dataline == "")
      break;*/
    Name = dataline;
    int Times_found=0;
    for (auto & Contact : Phonebook)
    {
      if (Contact.m_Name == Name || Contact.m_Surname == Name )
      {
        Times_found++;
        out << Contact.m_Name << " " << Contact.m_Surname << " " << Contact.m_Number << "\n";
      }
    }
    out << "-> " << Times_found << "\n";
  }


  file.close();
  return true;
}

#ifndef __PROGTEST__
int main ()
{
  ostringstream oss;
  oss . str ( "" );
  report( "tests/test_me_in.txt", oss );
  oss . str ("");
  assert ( report( "tests/test0_in.txt", oss ) == true );
  assert ( oss . str () ==
    "John Christescu 258452362\n"
    "John Harmson 861647702\n"
    "-> 2\n"
    "-> 0\n"
    "Josh Dakhov 264112084\n"
    "Dakhov Speechley 865216101\n"
    "-> 2\n"
    "John Harmson 861647702\n"
    "-> 1\n" );
  oss . str ( "" );
  assert ( report( "tests/test1_in.txt", oss ) == false );
  return 0;
}
#endif /* __PROGTEST__ */
