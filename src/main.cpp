#include <string>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <cassert>

#include "Board.h"
#include "HumanPlayerController.h"
#include "Enums.h"
#include "Pentago.h"

static const char save_file_path[] = "game_state.txt";

Pentago* start_new_game() {
    Board board(3, 2);
    
    PlayerController* player1 = new HumanPlayerController("Alphonse", WhitePlayer);
    PlayerController* player2 = new HumanPlayerController("Gaston", BlackPlayer);

    Pentago* game = new Pentago(board, player1, player2);
    return game;
}

Pentago* load_game(std::istream& stream) {
    Pentago* game = Pentago::load_game(stream);
    return game;
}

int main(int argc, char** argv) {

    std::cout << "Welcome to Pentago!" << std::endl;

    std::ifstream save_stream(save_file_path);

    Pentago* game = NULL;

    //Only present the menu if the save file exists, otherwise start a new game automatically.
    if(!save_stream.good()) {
        game = start_new_game();
    }
    while(save_stream.good()) {
        std::cout << "Do you want to 1) Start a new game or 2) Continue the previous game? ";
        int option = 0;
        std::cin >> option;
        if(option == 1) {
            game = start_new_game();
            break;
        } else if(option == 2) {
            game = load_game(save_stream);
            if(game == NULL) {
                std::cerr << "Couldn't load the save file!";
                return -1;
            }
            break;
        } else {
            std::cout << "That is not a valid option\n";
        }

    }

    assert(game != NULL);

    save_stream.close();

    game->set_save_file(save_file_path);

    WinStatus winner = game->play_game();

    std::cout << game->board();

    if(winner == BlackWin) {
        std::cout << "Black wins!" << std::endl;
    } else if(winner == WhiteWin) {
        std::cout << "White wins!" << std::endl;
    } else {
        std::cout << "Tie!" << std::endl;
    }

    delete game;

    return 0;
}
