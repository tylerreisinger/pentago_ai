#include <iostream>

#include "Board.h"
#include "Pentago.h"

#include "MinimaxComputerController.h"
#include "MctsComputerController.h"

float arena(int argc, char** argv, int max_runs);

Pentago* initialize_game(int argc, char** argv) {
    Board board;

    PlayerController* p1 = new MinimaxComputerController("p1", WhitePlayer, board, 2, 15.00);
   //PlayerController* p2 = new MinimaxComputerController("p2", BlackPlayer, board, 2, 15.00);
   PlayerController* p2 = new MctsComputerController("p2", BlackPlayer, board);

    Pentago* game = new Pentago(board, p1, p2);
    return game;
}

int main(int argc, char** argv) {
    arena(argc, argv, 1000);
}

float arena(int argc, char** argv, int max_runs) {

    int wins = 0;

    for(int i = 0; i < max_runs; ++i) {
        Pentago* game = initialize_game(argc, argv);
        if(i % 2 == 1) {
            game->swap_payers();
        }

        WinStatus win = game->play_game();
        if(win == WhiteWin) {

            wins += 1;
        }

        std::cout << "===== Game " << i << " =====" << std::endl;
        std::cout << "White: " << static_cast<double>(wins) / (i+1) << std::endl;
        std::cout << "Black: " << static_cast<double>((i+1) - wins) / (i+1) << std::endl;
        
        delete game;
    }

    std::cout << "========== Final Result ==========" << std::endl;
    std::cout << "White: " << static_cast<double>(wins) / max_runs << std::endl;
    std::cout << "Black: " << static_cast<double>(max_runs - wins) / max_runs << std::endl;
    std::cout << "==================================" << std::endl;

    return static_cast<double>(wins) / max_runs;
}
