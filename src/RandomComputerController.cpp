#include "RandomComputerController.h"

#include "Pentago.h"
#include "Board.h"

#include <iostream>
#include <cstdlib>



RandomComputerController::RandomComputerController(std::string name, PlayerColor color):
    PlayerController(name, color)
{
 
}
 
RandomComputerController::~RandomComputerController()
{
 
}
 
Move RandomComputerController::make_move(const Board& board, const Pentago& game)
{
    int cell = 0;
    int entry = 0;

    int rotate_cell = std::rand() % (board.cell_count());

    bool is_left = static_cast<bool>(std::rand() % 2);
    //Loop until we find an empty entry.
    while(true) {
        entry = std::rand() % (board.cell_size()*board.cell_size());
        cell = std::rand() % (board.cell_count());

        if(board.get_value(cell, entry) == EmptyEntry) {
            break;
        }
    }  

    RotationDirection dir = is_left ? RotateLeft : RotateRight;  

    return Move(cell, entry, rotate_cell, dir);
}
 
