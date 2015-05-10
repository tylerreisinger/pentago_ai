#include "Enums.h"


BoardEntry player_color_to_board_entry(PlayerColor color)
{
    switch(color) {
        case WhitePlayer: return WhiteEntry;
        case BlackPlayer: return BlackEntry;
        default: return EmptyEntry;
    } 
}
 
