#ifndef BOARD_H__
#define BOARD_H__

#include <vector>
#include <ostream>
#include <array>

#include "Enums.h"
#include "Move.h"

//Object representing the state of a pentago board.
class Board
{
public:
    static const int WIN_SIZE = 5;
    static const int CELL_SIZE = 3;
    static const int CELLS_PER_ROW = 2;
    static const int TOTAL_ENTRIES = CELL_SIZE*CELL_SIZE*CELLS_PER_ROW*CELLS_PER_ROW;
    static const int CELL_ENTRIES = CELL_SIZE*CELL_SIZE;

    //Construct a board with the given dimensions.
    //Board(3, 2) is the standard 6x6 board.
    Board();
    ~Board() {};

    Board(const Board& other);

    Board clone() const;

    //Get entry values
    BoardEntry& get_value(int cell, int entry);
    BoardEntry get_value(int cell, int entry) const;

    BoardEntry& get_value_absolute(int x, int y);
    BoardEntry get_value_absolute(int x, int y) const;

    bool is_cell_empty(int cell, int entry) const;
    bool is_cell_empty_absolute(int x, int y) const;

    void set_value(int cell, int entry, BoardEntry value);
    void set_value_absolute(int x, int y, BoardEntry value);

    //Apply a move to the board. If a player wins after applying
    //the piece to the board but before twisting a cell,
    //the win status will be returned and the twist will not be
    //applied.
    WinStatus apply_move(const Move& move, PlayerColor color);

    void apply_move_no_check(const Move& move, PlayerColor color);

    //Return board dimensions
    int board_size() const {return CELL_SIZE*CELLS_PER_ROW;}
    int cells_per_row() const {return CELLS_PER_ROW;}
    int cell_size() const {return CELL_SIZE;}
    int entries_per_cell() const {return CELL_SIZE*CELL_SIZE;}
    int cell_count() const {return CELLS_PER_ROW*CELLS_PER_ROW;}
    int total_entries() const {return TOTAL_ENTRIES;}

    void cell_to_absolute_pos(int cell, int entry, int& x, int& y) const;

    void rotate_cell(int cell, RotationDirection dir);

    //Check for victory/loss conditions.
    WinStatus check_for_wins() const;
    bool check_full() const;

    friend std::ostream& operator <<(std::ostream& stream, const Board& board);

protected:

    std::array<BoardEntry, TOTAL_ENTRIES> m_board;
private:

    void rotate_cell_left(int cell);
    void rotate_cell_right(int cell);

    void check_run_next(int x, int y, int& run_len, BoardEntry& run_type, bool& white_win, bool& black_win) const;
};

std::ostream& operator <<(std::ostream& stream, BoardEntry entry);
std::ostream& operator <<(std::ostream& stream, WinStatus value);

//Functions inlined for considerable performance improvement

inline BoardEntry& Board::get_value(int cell, int entry)
{
    int x;
    int y;
    cell_to_absolute_pos(cell, entry, x, y);

    return m_board[x + y * board_size()];

}
 
inline BoardEntry Board::get_value(int cell, int entry) const
{
    int x;
    int y;
    cell_to_absolute_pos(cell, entry, x, y);

    return m_board[x + y * board_size()];
}
 
inline BoardEntry& Board::get_value_absolute(int x, int y)
{
    return m_board[x + y*board_size()]; 
}
 
inline BoardEntry Board::get_value_absolute(int x, int y) const
{
    return m_board[x + y*board_size()]; 
}
 
inline bool Board::is_cell_empty(int cell, int entry) const
{
    return get_value(cell, entry) == EmptyEntry; 
}
 
inline bool Board::is_cell_empty_absolute(int x, int y) const
{
    return get_value_absolute(x, y) == EmptyEntry; 
}

inline Board::Board() { 
    m_board.fill(EmptyEntry); 
}
 
inline Board::Board(const Board& other):
    m_board(other.m_board)
{ 
}
 
inline Board Board::clone() const
{
    return Board(*this);  
}
 
inline void Board::apply_move_no_check(const Move& move, PlayerColor color)
{
    set_value(move.play_cell(), move.play_index(), player_color_to_board_entry(color));
    rotate_cell(move.rotate_cell(), move.rotation_direction()); 
}
 
inline void Board::cell_to_absolute_pos(int cell, int entry, int& x, int& y) const
{
    int cell_start_x = (cell % cells_per_row()) * cell_size();
    int cell_start_y = (cell / cells_per_row()) * cell_size();

    int cell_offset_x = entry % cell_size();
    int cell_offset_y = entry / cell_size();

    x = cell_start_x + cell_offset_x;
    y = cell_start_y + cell_offset_y;
 
}

#endif
    
