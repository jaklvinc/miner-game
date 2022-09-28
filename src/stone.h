
#include "item.h"

class CStone : public CItem
{
    public:
        CStone( int quantity );
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