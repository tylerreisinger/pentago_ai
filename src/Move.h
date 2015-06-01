#ifndef MOVE_H__
#define MOVE_H__

#include "Board.h"

#include <ostream>
#include <istream>

class Move
{
public:
    Move(int cell, int index, int rotate_cell, RotationDirection dir);
    ~Move() {};

    Move(const Move& other);

    int play_cell() const {return m_cell;}
    int rotate_cell() const {return m_rotate_cell;}
    int play_index() const {return m_index;}
    RotationDirection rotation_direction() const {return m_direction;}

    bool is_invalid() const;

    bool operator ==(const Move& other) const;
    bool operator !=(const Move& other) const;

    static Move read_from_stream(std::istream& stream);

    static Move invalid_move();

    friend std::ostream& operator <<(std::ostream& stream, const Move& move);

protected:
private:
    Move() {};

    int m_cell;
    int m_index;
    int m_rotate_cell;
    RotationDirection m_direction;
};

inline bool Move::is_invalid() const
{
    return m_cell == -1; 
}

inline Move::Move(const Move& other):
    m_cell(other.m_cell), m_index(other.m_index), m_rotate_cell(other.m_rotate_cell),
    m_direction(other.m_direction)
{
}


inline bool Move::operator==(const Move& other) const
{
    return (m_cell == other.m_cell && m_index == other.m_index
            && m_rotate_cell == other.m_rotate_cell && m_direction == other.m_direction);
}
 
inline bool Move::operator!=(const Move& other) const
{
    return !(*this == other); 
}
 
#endif
    
