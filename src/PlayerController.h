#ifndef PLAYERCONTROLLER_H__
#define PLAYERCONTROLLER_H__

#include <string>

#include "Move.h"
#include "Board.h"

#include "Enums.h"

class Board;
class Pentago;

class PlayerController
{
public:
    PlayerController(std::string name, PlayerColor color);
    ~PlayerController() {};

    const std::string& name() const {return m_name;}
    PlayerColor color() const {return m_color;}

    virtual Move make_move(const Board& board, const Pentago& game) = 0;

protected:
private:
    std::string m_name;
    PlayerColor m_color;
};


#endif
    
