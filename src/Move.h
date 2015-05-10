#ifndef MOVE_H__
#define MOVE_H__

#include "Board.h"

#include <ostream>
#include <istream>

class Move
{
public:
    Move(int cell, int index, int rotate_cell, RotationDirection dir);
    ~Move();

    Move(const Move& other);

    int play_cell() const {return m_cell;}
    int rotate_cell() const {return m_rotate_cell;}
    int play_index() const {return m_index;}
    RotationDirection rotation_direction() const {return m_direction;}

    static Move read_from_stream(std::istream& stream);

    friend std::ostream& operator <<(std::ostream& stream, const Move& move);

protected:
private:
    Move() {};

    int m_cell;
    int m_index;
    int m_rotate_cell;
    RotationDirection m_direction;
};


#endif
    
