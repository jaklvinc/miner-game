#pragma once

class CItem
{
protected:
    char m_Type;
    int m_Weight;

public:
    CItem();
    CItem(char type, int weight);

    char GetType() const;
    int GetWeight() const;

    bool operator<(const CItem &item) const;
    bool operator<(const char &type) const;
    bool operator==(const char &type) const;
};