#include "item.h"

CItem::CItem(){}

CItem::CItem(char type, int weight) : m_Type(type), m_Weight(weight){}

char CItem::GetType() const
{
    return m_Type;
}

int CItem::GetWeight() const
{
    return m_Weight;
}

bool CItem::operator < ( const CItem & item ) const
{
    return this->m_Type < item.GetType();
}

bool CItem::operator < ( const char & type ) const
{
    return this->m_Type < type;
}

bool CItem::operator == ( const char & type ) const
{
    return this->m_Type == type;
}