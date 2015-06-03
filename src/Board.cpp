#include "Board.h"

#include <cassert>

RotationDirection reverse_direction(RotationDirection direction) {
    if(direction == RotateLeft) {
        return RotateRight;
    } else {
        return RotateLeft;
    }
}

void Board::set_value(int cell, int entry, BoardEntry value)
{
    int x;
    int y; 

    cell_to_absolute_pos(cell, entry, x, y);

    m_board[x + y*board_size()] = value;
}
 
void Board::set_value_absolute(int x, int y, BoardEntry value)
{
    m_board[x + y*board_size()] = value;
}
 
WinStatus Board::apply_move(const Move& move, PlayerColor color)
{
    set_value(move.play_cell(), move.play_index(), player_color_to_board_entry(color));
    bool no_twist_win = false;

    WinStatus status = check_for_wins();
    if(status != NoWin) {
        no_twist_win = true;
    }
    rotate_cell(move.rotate_cell(), move.rotation_direction()); 
    WinStatus end_status = check_for_wins();
    if(no_twist_win) {
        if(check_for_wins() == Tie) {
            return Tie;
        }
        else {
            rotate_cell(move.rotate_cell(),
                   reverse_direction(move.rotation_direction()));
            return status;
        }
    }
    return end_status;
}
 
 
void Board::rotate_cell(int cell, RotationDirection dir)
{
    std::array<BoardEntry, CELL_ENTRIES> cell_copy;

    int cell_start_x;
    int cell_start_y;

    cell_to_absolute_pos(cell, 0, cell_start_x, cell_start_y);

    for(int y = 0; y < cell_size(); ++y) {
        int y_pos = y + cell_start_y;
        for(int x = 0; x < cell_size(); ++x) {
           int x_pos = x + cell_start_x;
            
           cell_copy[x + y*cell_size()] = m_board[x_pos + y_pos*board_size()];
        }
    } 

    for(int y = 0; y < cell_size(); ++y) {

        for(int x = 0; x < cell_size(); ++x) {

           int rotated_x;
           int rotated_y;
            
           if(dir == RotateLeft) {
               rotated_y = (cell_size()-1)-x;
               rotated_x = y;
           } else if (dir == RotateRight) {
               rotated_y = x;
               rotated_x = (cell_size()-1)-y;
           } else {
               assert(false);
               return;
           }

           int x_pos_rotated = rotated_x + cell_start_x;
           int y_pos_rotated = rotated_y + cell_start_y;
           m_board[x_pos_rotated + y_pos_rotated*board_size()] = cell_copy[x + y*cell_size()];
        }
    } 
}

WinStatus Board::check_for_wins() const
{
    bool white_win = false;
    bool black_win = false;

    int run_len = 0;
    BoardEntry run_type = EmptyEntry;

    //Look for horizontal wins 
    for(int y = 0; y < board_size(); ++y) {
        run_len = 0;
        run_type = EmptyEntry;

        for(int x = 0; x < board_size(); ++x) {
            check_run_next(x, y, run_len, run_type, white_win, black_win);
        }
    }

    //Look for vertical wins
    for(int x = 0; x < board_size(); ++x) {
        run_len = 0;
        run_type = EmptyEntry;

        for(int y = 0; y < board_size(); ++y) {
            check_run_next(x, y, run_len, run_type, white_win, black_win);
        }
    }

    for(int y = 0; y < board_size(); ++y) {
        for(int x = 0; x < board_size()-WIN_SIZE+1; ++x) {
            run_len = 0;
            run_type = EmptyEntry;
            for(int off = 0; off < board_size()-x; ++off) {
                check_run_next(x+off, y+off, run_len, run_type, white_win, black_win);
            } 
        }
    }

    for(int y = board_size()-2; y < board_size(); ++y) {
        for(int x = 0; x < board_size()-WIN_SIZE+1; ++x) {
            run_len = 0;
            run_type = EmptyEntry;
            for(int off = 0; off < board_size()-x; ++off) {
                check_run_next(x+off, y-off, run_len, run_type, white_win, black_win);
            } 
        }
    }


    /*
    //Look for x diagonal wins
    for(int x = 0; x < board_size()-WIN_SIZE+1; ++x) {
        run_len = 0;
        run_type = EmptyEntry;
        for(int off = 0; off < board_size()-x; ++off) {
            check_run_next(x+off, off, run_len, run_type, white_win, black_win);
        } 
    }
    
    //Look for y diagonal wins
    for(int y = 0; y < board_size()-WIN_SIZE+1; ++y) {
        run_len = 0;
        run_type = EmptyEntry;
        for(int off = 0; off < board_size()-y; ++off) {
            check_run_next(off, y+off, run_len, run_type, white_win, black_win);
        } 
    }
    */

    if(white_win && !black_win) {
        return WhiteWin;
    } else if(black_win && !white_win) {
        return BlackWin;
    } else if(black_win && white_win) {
        return Tie;
    } else {
        return NoWin;
    }
}

bool Board::check_full() const
{
    for(int i = 0; i < board_size(); ++i) {
        for(int j = 0; j < board_size(); ++j) {
            if(get_value_absolute(i, j) == EmptyEntry) {
                return false;
            }
        }
    }
    return true;
}
 
void Board::check_run_next(int x, int y, int& run_len, BoardEntry& run_type,
        bool& white_win, bool& black_win) const
{
        BoardEntry cell_value = get_value_absolute(x, y);
        if(cell_value == run_type && run_type != EmptyEntry) {
            run_len += 1;
        } else {
            run_len = 1;
            run_type = cell_value;
        }
        if(run_len >= WIN_SIZE) {
            if(run_type == WhiteEntry) {
                white_win = true;
            } else if(run_type == BlackEntry) {
                black_win = true;
            }
        }
}
 

void output_line_border(std::ostream& stream, const Board& board) {
    stream << "+";
    for(int i = 0; i < board.cells_per_row(); ++i) {
        for(int j = 0; j < board.cell_size()*2-1; ++j) {
            stream << "-";
        } 
        stream << "+";
    }
    stream << "\n";
}

void output_blank_line(std::ostream& stream, const Board& board) {
    for(int i = 0; i < board.board_size()*2; ++i) {
        if(i % (board.cell_size()*2) == 0) {
            stream << '|';
        } else {
            stream << ' ';
        }
    }
    stream << "|\n";
}

 

std::ostream& operator <<(std::ostream& stream, BoardEntry entry) {
    switch(entry) {
        case EmptyEntry: stream << '.'; break;
        case WhiteEntry: stream << 'w'; break;
        case BlackEntry: stream << 'b'; break;
        default: stream << '?';
    }
    return stream;
}

std::ostream& operator <<(std::ostream& stream, const Board& board) {

    for(int y = 0; y < board.board_size(); ++y) {
        if(y % board.cell_size() == 0) {
            output_line_border(stream, board);
        } else {
            output_blank_line(stream, board);
        }
        for(int x = 0; x < board.board_size(); ++x) {
            if(x % board.cell_size() == 0) {
                stream << '|';
            } else {
                stream << ' ';
            }
            stream << board.get_value_absolute(x, y);            
        }
        stream << "|\n";
    }
    output_line_border(stream, board);
    return stream;    
}

std::ostream& operator<<(std::ostream& stream, WinStatus value)
{
   switch(value) {
       case WhiteWin: stream << "white wins"; break;
       case BlackWin: stream << "black wins"; break;
       case Tie: stream << "tie"; break;
       case NoWin: stream << "no win"; break;
       default: stream << "invalid value"; break;
   } 

   return stream;
}
 
