#include "HumanPlayerController.h"

#include <iostream>
#include <stdexcept>

HumanPlayerController::HumanPlayerController(std::string name, PlayerColor color):
    PlayerController(name, color)
{
}
 
Move HumanPlayerController::make_move(const Board& board, const Pentago& game)
{
    //Keep asking for moves until a move is of the correct format.
    while(true) {
        std::cout << "Enter move: ";
       
        try {
            Move move = Move::read_from_stream(std::cin);   
            return move;
        } catch(std::runtime_error& ex) {
            std::cout << "Invalid move format" << std::endl; 
        }
    }
}
 
