#ifndef BOARD_H__
#define BOARD_H__

#include <vector>
#include <ostream>

#include "Enums.h"

class Move;

class Board
{
public:
    static const int WIN_SIZE = 5;

    Board(int cell_size, int cells_per_row);
    ~Board();

    Board(const Board& other);

    BoardEntry& get_value(int cell, int entry);
    BoardEntry get_value(int cell, int entry) const;

    BoardEntry& get_value_absolute(int x, int y);
    BoardEntry get_value_absolute(int x, int y) const;

    void set_value(int cell, int entry, BoardEntry value);
    void set_value_absolute(int x, int y, BoardEntry value);

    WinStatus apply_move(const Move& move, PlayerColor color);

    int board_size() const;
    int cells_per_row() const {return m_cells_per_row;}
    int cell_size() const {return m_cell_size;}
    int cell_count() const {return m_cells_per_row*m_cells_per_row;}

    void cell_to_absolute_pos(int cell, int entry, int& x, int& y) const;

    void rotate_cell(int cell, RotationDirection dir);

    WinStatus check_for_wins() const;

    friend std::ostream& operator <<(std::ostream& stream, const Board& board);

protected:

    int m_cell_size;
    int m_cells_per_row;
    std::vector<BoardEntry> m_board;
private:

    void rotate_cell_left(int cell);
    void rotate_cell_right(int cell);

    void check_run_next(int x, int y, int& run_len, BoardEntry& run_type, bool& white_win, bool& black_win) const;
};

std::ostream& operator <<(std::ostream& stream, BoardEntry entry);
std::ostream& operator <<(std::ostream& stream, WinStatus value);

#endif
    
