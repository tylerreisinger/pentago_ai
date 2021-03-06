#ifndef ENUMS_H__
#define ENUMS_H__

enum RotationDirection {
    RotateLeft,
    RotateRight
};

enum WinStatus {
    WhiteWin,
    BlackWin,
    Tie,
    NoWin
};

enum BoardEntry {
    EmptyEntry,
    WhiteEntry,
    BlackEntry
};

enum PlayerColor {
    WhitePlayer,
    BlackPlayer
};

BoardEntry player_color_to_board_entry(PlayerColor color);

BoardEntry board_entry_from_char(char ch);

//Return the color of the opponent, if the player's color is color.
PlayerColor opposing_color(PlayerColor color);

inline BoardEntry player_color_to_board_entry(PlayerColor color)
{
    switch(color) {
        case WhitePlayer: return WhiteEntry;
        case BlackPlayer: return BlackEntry;
        default: return EmptyEntry;
    } 
}

inline PlayerColor opposing_color(PlayerColor color)
{
    if(color == WhitePlayer) {
       return BlackPlayer;
    } else {
       return WhitePlayer;
    } 
}

#endif
