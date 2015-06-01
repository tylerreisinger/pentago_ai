#include <string>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <cassert>
#include <ctime>
#include <cstdlib>

#include "Board.h"
#include "HumanPlayerController.h"
#include "RandomComputerController.h"
#include "MinimaxComputerController.h" 
#include "MctsComputerController.h" 
#include "Enums.h"
#include "Pentago.h"

static const char save_file_path[] = "game_state.txt";

enum ControllerType {
    ControllerTypePlayer,
    ControllerTypeComputerRandom,
    ControllerTypeComputerMinimax,
    ControllerTypeComputerMcts,
};

ControllerType prompt_player_controller_type(const std::string& player_name) {
    const int CHOICE_PLAYER = 1;
    const int CHOICE_RANDOM_COMPUTER = 2;
    const int CHOICE_MINIMAX_COMPUTER = 3;
    const int CHOICE_MCTS_COMPUTER = 4;

    const int max_choice = 4;
    int choice = -1;
    while(true) {
        std::cout << "How is " << player_name << " controlled?\n";
        std::cout << "1) Player Controlled\n";
        std::cout << "2) Computer (Random) Controlled" << std::endl;
        std::cout << "3) Computer (Minimax) Controlled" << std::endl;
        std::cout << "4) Computer (Mcts) Controlled" << std::endl;

        std::cin >> choice;
        if(choice < 1 || choice > max_choice) {
            std::cout << "That is not a valid choice" << std::endl;
            std::cin.clear();
            std::cin.ignore(1000, '\n');
        } else {
            break;
        }

    }

    switch(choice) {
        case CHOICE_PLAYER: return ControllerTypePlayer;
        case CHOICE_RANDOM_COMPUTER: return ControllerTypeComputerRandom;
        case CHOICE_MINIMAX_COMPUTER: return ControllerTypeComputerMinimax;
        case CHOICE_MCTS_COMPUTER: return ControllerTypeComputerMcts;
        default: throw std::runtime_error("Invalid ControllerType");
    }
}

PlayerController* construct_controller(ControllerType type, std::string name, 
        PlayerColor color, const Board& initial_board) {
    switch(type) {
        case ControllerTypePlayer: 
            return new HumanPlayerController(name, color);
        case ControllerTypeComputerRandom: 
            return new RandomComputerController(name, color);
        case ControllerTypeComputerMinimax: 
            return new MinimaxComputerController(name, color, initial_board, 3);
        case ControllerTypeComputerMcts: 
            return new MinimaxComputerController(name, color, initial_board, 2);
            return new MctsComputerController(name, color, initial_board);
        default: throw std::runtime_error("Invalid ControllerType");
    }
}

PlayerColor prompt_player_color(const std::string& player_name) {
    std::string color_string;

    while(true) {

        std::cout << "What color should " << player_name << " use (W or B)? ";
        std::cin >> color_string;

        if(color_string == "White" || color_string == "white" || color_string == "W" || color_string == "w") {
            return WhitePlayer;
        } else if(color_string == "Black" || color_string == "black" || color_string == "B" || color_string == "b") {
            return BlackPlayer;
            break;
        } else {
            std::cout << "That is not a valid choice" << std::endl;
            std::cin.clear();
            std::cin.ignore(1000, '\n');
        }

    }
}

PlayerController* prepare_player(bool ask_color, const std::string& player_ordinal,
       const Board& initial_board, PlayerColor color = WhitePlayer) {
    std::string player_name;
    PlayerColor player_color;

    std::cout << "What is the name of the " << player_ordinal << " player? ";
    std::cin >> player_name;
    
    if(ask_color) {
        player_color = prompt_player_color(player_name);
    } else {
        player_color = color;
    }


    PlayerController* player = construct_controller(prompt_player_controller_type(player_name),
            player_name, player_color, initial_board);

    return player;
}

Pentago* start_new_game() {
    Board board;
    
    PlayerController* player1 = prepare_player(true, "first", board);
    PlayerController* player2 = prepare_player(false, "second", board,
            opposing_color(player1->color()));

    Pentago* game = new Pentago(board, player1, player2);
    return game;
}

Pentago* load_game(std::istream& stream) {
    Pentago* game = Pentago::load_game(stream);
    return game;
}

int main(int argc, char** argv) {

    std::srand(std::time(NULL));

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
        std::cout << std::endl;
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
            std::cin.clear();
            std::cin.ignore(1000, '\n');
        }

    }

    assert(game != NULL);

    save_stream.close();

    game->set_save_file(save_file_path);

    //Run the game until either someone wins or the board fills entirely.
    WinStatus winner = game->play_game();

    std::cout << game->board();

    if(winner == BlackWin) {
        std::cout << game->get_player_from_color(BlackPlayer)->name() << " wins!" << std::endl;
    } else if(winner == WhiteWin) {
        std::cout << game->get_player_from_color(WhitePlayer)->name() << " wins!" << std::endl;
    } else if(winner == NoWin) {
        std::cout << "No winner" << std::endl;
    } else {
        std::cout << "Tie!" << std::endl;
    }

    delete game;

    return 0;
}
