#include "Enums.h"

#include <stdexcept>


 
BoardEntry board_entry_from_char(char ch)
{
    if(ch == '.') {
        return EmptyEntry;
    } else if (ch == 'w') {
        return WhiteEntry;
    } else if (ch == 'b') {
        return BlackEntry;
    } else {
        throw std::runtime_error("Invalid entry character");
    }
}
 
 
