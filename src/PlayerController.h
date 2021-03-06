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
    WinStatus player_win_kind() const;

    int kind_id() const {return m_controller_id;}
    void set_kind_id(int value) {m_controller_id = value;}

    virtual Move make_move(const Board& board, const Pentago& game) = 0;

protected:
private:
    std::string m_name;
    PlayerColor m_color;
    int m_controller_id;
};


inline WinStatus PlayerController::player_win_kind() const
{
    if(m_color == BlackPlayer) {
       return BlackWin;
    } else {
       return WhiteWin;
    } 
}
 
#endif
    
