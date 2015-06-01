#ifndef HUMANPLAYERCONTROLLER_H__
#define HUMANPLAYERCONTROLLER_H__

#include "PlayerController.h"

//A PlayerController allowing a human player to control the moves through 
//the terminal.
class HumanPlayerController: public PlayerController
{
public:
    HumanPlayerController(std::string name, PlayerColor color);
    ~HumanPlayerController();

    virtual Move make_move(const Board& board, const Pentago& game);

protected:
private:
};

#endif
    
