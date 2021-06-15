#pragma once

#include <fstream>

class CItem
{
    protected:
        int m_Quantity;
        int m_Price;
        int m_Weight;
    public:
        CItem( int quantity , int price, int weight );

        /**
         * Getter for quanity
         * 
         * @returns quantity of the item in players inventory
         */
        int GetQuantity() const;

        /**
         * Getter for item price
         * 
         * @returns price the item can be sold for
         */
        int GetPrice() const;

        /**
         * Getter for weight
         * 
         * @returns weight of the item
         */
        int GetWeight() const;

        /**
         * Getter for the weight all the items of this type are
         * 
         * @returns m_Weight*m_Quantity
         */
        int GetActualWeight() const;

        /**
         * Adds specified number of items to inventory
         * 
         * @param[in] quantity quantity of the item to add
         */
        void AddQuantity( const int quantity );

        /**
         * Destroys half of the items in players inventory
         * 
         * @returns new actual weight
         */
        int Die();

        /**
         * Resets quantity to 0
         */
        void Reset();

        /**
         * Saves quantity of an item into a file
         * 
         * @param[out] data filestream to output to
         */
        void Save( std::ofstream & data );

        /**
         * Prints how many items were added to standard output
         * 
         * @param[in] quantity how much to print out
         */
        virtual void PrintAdded( int quantity ) = 0;

        /**
         * Prints itself to the standard output
         */
        virtual void Print() = 0;
        
};