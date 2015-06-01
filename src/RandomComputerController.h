#ifndef RANDOMCOMPUTERCONTROLLER_H__
#define RANDOMCOMPUTERCONTROLLER_H__

#include "PlayerController.h"

class RandomComputerController: public PlayerController
{
public:
    RandomComputerController(std::string name, PlayerColor color);
    ~RandomComputerController();

    virtual Move make_move(const Board& board, const Pentago& game);

protected:
private:
};


#endif
    
