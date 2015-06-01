#include "Move.h"

#include <stdexcept>

Move::Move(int cell, int index, int rotate_cell, RotationDirection dir):
    m_cell(cell), m_index(index), m_rotate_cell(rotate_cell), m_direction(dir)
{
}

std::ostream& operator <<(std::ostream& stream, const Move& move) {
    
    stream << move.play_cell()+1 << "/" << move.play_index()+1 << " ";

    stream << move.rotate_cell()+1;


    if(move.rotation_direction() == RotateLeft) {
        stream << "L";
    } else {
        stream << "R";
    }

    return stream;
}

 
Move Move::read_from_stream(std::istream& stream)
{
    Move move; 

    char rotate_char;
    char sep;
    
    if(!(stream >> move.m_cell)) {
        stream.clear();
        stream.ignore(1000, '\n');
        throw std::runtime_error("Invalid move format");
    }
    move.m_cell -= 1;
    if(!(stream >> sep)) {
        stream.clear();
        stream.ignore(1000, '\n');
        throw std::runtime_error("Invalid move format");
    }
    if(!(stream >> move.m_index)) {
        stream.clear();
        stream.ignore(1000, '\n');
        throw std::runtime_error("Invalid move format");
    }
    move.m_index -= 1;

    if(!(stream >> move.m_rotate_cell)) {
        stream.clear();
        stream.ignore(1000, '\n');
        throw std::runtime_error("Invalid move format");
    }
    move.m_rotate_cell -= 1;
    if(!(stream >> rotate_char)) {
        stream.clear();
        stream.ignore(1000, '\n');
        throw std::runtime_error("Invalid move format");
    }

    if(rotate_char == 'R' || rotate_char == 'r') {
        move.m_direction = RotateRight;
    } else if(rotate_char == 'L' || rotate_char == 'l') {
        move.m_direction = RotateLeft;
    }

    //Ignore anything after the move up to and including the new line.
    stream.ignore(1000, '\n');

    return move;
}
 
Move Move::invalid_move()
{
    return Move(-1, -1, -1, RotateRight);
}
 
