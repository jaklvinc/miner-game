
#include "item.h"

class CBone : public CItem
{
    public:
        CBone( int quantity );
        /**
         * Prints how many items were added to standard output
         * 
         * @param[in] quantity how much to print out
         */
        void PrintAdded( int quantity );

        /**
         * Prints itself to the standard output
         */
        void Print() override;
};