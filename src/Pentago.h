#ifndef PENTAGO_H__
#define PENTAGO_H__

#include "Board.h"
#include "PlayerController.h"

class Pentago
{
public:
    Pentago(Board board, PlayerController* player1, PlayerController* player2);
    ~Pentago();

    WinStatus play_game();

protected:
    Board m_board;

    PlayerController* m_current_player;
    PlayerController* m_next_player;
private:

    bool is_valid_move(const Move& move) const;
};


#endif
    
