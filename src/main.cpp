#include <iostream>

#include "Board.h"
#include "HumanPlayerController.h"
#include "Enums.h"
#include "Pentago.h"

int main(int argc, char** argv) {

    Board board(3, 2);
    
    PlayerController* player1 = new HumanPlayerController("Alphonse", WhitePlayer);
    PlayerController* player2 = new HumanPlayerController("Gaston", BlackPlayer);

    Pentago game(board, player1, player2);
    WinStatus winner = game.play_game();

    if(winner == BlackWin) {
        std::cout << "Black wins!" << std::endl;
    } else if(winner == WhiteWin) {
        std::cout << "White wins!" << std::endl;
    } else {
        std::cout << "Tie!" << std::endl;
    }

    return 0;
}
