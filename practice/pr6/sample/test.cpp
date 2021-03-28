#ifndef __PROGTEST__
#include <cstring>
using namespace std;

class CLinkedSetTester;
#endif /* __PROGTEST__ */


class CLinkedSet
{
private:
    struct CNode
    {
        CNode * m_Next;
        const char * m_Value;

        const char * Value () const
        {
            return m_Value;
        }
    };

    CNode * m_Begin;
    size_t size;

public:
    CLinkedSet()
    {
        m_Begin = nullptr;
        size = 0;
    }

    // copy constructor

    // operator=

    // destructor
    ~CLinkedSet()
    {
        for( size_t i = size ; i > 0 ; i-- )
        {
            CNode * last_node = m_Begin;
            for ( size_t j = 1 ; j < i ; j++ )
            {
                last_node = last_node->m_Next;
            }
            delete last_node->m_Next;
            delete last_node->m_Value;
        }
        m_Begin = nullptr;
    }

    bool Insert ( const char * value )
    {

        if ( m_Begin == nullptr )
        {
            m_Begin->m_Value = value;
            m_Begin->m_Next = nullptr;
            size++;
        }
        else
        {
            CNode * last_node = m_Begin;
            while ( last_node->m_Next != nullptr )
            {
                if ( strcmp( last_node->m_Value , value ) )
                    return false;
                last_node = last_node->m_Next;
            }
            if ( strcmp( last_node->m_Value , value ) )
                return false;
            
            last_node->m_Next->m_Value = value;
            last_node->m_Next->m_Next = nullptr;
            size ++;
        }
        return true;

    }
    
    bool Remove ( const char * value )
    {
        if ( m_Begin == nullptr )
            return false;
        
        if ( strcmp (m_Begin->m_Value , value ) )
        {
            if ( m_Begin->m_Next == nullptr )
            {
                delete m_Begin->m_Value;
                m_Begin = nullptr;
                size--;
                return true;
            }
            else
            {
                CNode * tmp = m_Begin->m_Next;
                delete m_Begin->m_Value;
                m_Begin = tmp;
                size--;
                return true;
            }
        }
        else
        {
            CNode * check = m_Begin;
            for ( size_t i = 1 ; i < size ; i++ )
            {
                if ( strcmp ( check->m_Next->m_Value , value ) )
                {
                    CNode * tmp = check->m_Next->m_Next;
                    delete check->m_Next->m_Value;
                    check->m_Next = tmp;
                    size--;
                    return true;
                }
                check = check->m_Next;
            }
        }
        return false;
    }

    bool Empty () const
    {
        return m_Begin == nullptr;
    }

    size_t Size () const
    {
        return size;
    }

    bool Contains ( const char * value ) const
    {
        if ( m_Begin == nullptr )
            return false;
        
        CNode * check = m_Begin;
        for ( size_t i = 1 ; i <= size ; i++ )
        {
            if ( strcmp(check->m_Value , value) )
                return true;
        }
        return false;
    }

    friend class ::CLinkedSetTester;
};

#ifndef __PROGTEST__
#include <cassert>

struct CLinkedSetTester
{
    static void test0()
    {
        CLinkedSet x0;
        assert( x0 . Insert( "Jerabek Michal" ) );
        assert( x0 . Insert( "Pavlik Ales" ) );
        assert( x0 . Insert( "Dusek Zikmund" ) );
        assert( x0 . Size() == 3 );
        assert( x0 . Contains( "Dusek Zikmund" ) );
        assert( !x0 . Contains( "Pavlik Jan" ) );
        assert( x0 . Remove( "Jerabek Michal" ) );
        assert( !x0 . Remove( "Pavlik Jan" ) );
        assert( !x0 . Empty() );
    }

    static void test1()
    {
        CLinkedSet x0;
        assert( x0 . Insert( "Jerabek Michal" ) );
        assert( x0 . Insert( "Pavlik Ales" ) );
        assert( x0 . Insert( "Dusek Zikmund" ) );
        assert( x0 . Size() == 3 );
        assert( x0 . Contains( "Dusek Zikmund" ) );
        assert( !x0 . Contains( "Pavlik Jan" ) );
        assert( x0 . Remove( "Jerabek Michal" ) );
        assert( !x0 . Remove( "Pavlik Jan" ) );
        CLinkedSet x1 ( x0 );
        assert( x0 . Insert( "Vodickova Saskie" ) );
        assert( x1 . Insert( "Kadlecova Kvetslava" ) );
        assert( x0 . Size() == 3 );
        assert( x1 . Size() == 3 );
        assert( x0 . Contains( "Vodickova Saskie" ) );
        assert( !x1 . Contains( "Vodickova Saskie" ) );
        assert( !x0 . Contains( "Kadlecova Kvetslava" ) );
        assert( x1 . Contains( "Kadlecova Kvetslava" ) );
    }

    static void test2()
    {
        CLinkedSet x0;
        CLinkedSet x1;
        assert( x0 . Insert( "Jerabek Michal" ) );
        assert( x0 . Insert( "Pavlik Ales" ) );
        assert( x0 . Insert( "Dusek Zikmund" ) );
        assert( x0 . Size() == 3 );
        assert( x0 . Contains( "Dusek Zikmund" ) );
        assert( !x0 . Contains( "Pavlik Jan" ) );
        assert( x0 . Remove( "Jerabek Michal" ) );
        assert( !x0 . Remove( "Pavlik Jan" ) );
        x1 = x0;
        assert( x0 . Insert( "Vodickova Saskie" ) );
        assert( x1 . Insert( "Kadlecova Kvetslava" ) );
        assert( x0 . Size() == 3 );
        assert( x1 . Size() == 3 );
        assert( x0 . Contains( "Vodickova Saskie" ) );
        assert( !x1 . Contains( "Vodickova Saskie" ) );
        assert( !x0 . Contains( "Kadlecova Kvetslava" ) );
        assert( x1 . Contains( "Kadlecova Kvetslava" ) );
    }

};

int main ()
{
    CLinkedSetTester::test0();
    //CLinkedSetTester::test1();
    //CLinkedSetTester::test2();
    return 0;
}
#endif /* __PROGTEST__ */
