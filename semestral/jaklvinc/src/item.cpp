
#include "item.h"

CItem::CItem( int quantity , int price, int weight )
: m_Quantity(quantity) , m_Price(price) , m_Weight(weight)
{}

int CItem::GetQuantity() const
{
    return m_Quantity;
}

int CItem::GetPrice() const
{
    return m_Price;
}

int CItem::GetWeight() const
{
    return m_Weight;
}

int CItem::GetActualWeight() const
{
    return m_Weight*m_Quantity;
}

void CItem::AddQuantity( const int quantity )
{
    m_Quantity+=quantity;
}

int CItem::Die()
{
    m_Quantity/=2;
    return GetActualWeight();
}

void CItem::Save( std::ofstream & data )
{
    data << m_Quantity;
}

void CItem::Reset()
{
    m_Quantity=0;
}