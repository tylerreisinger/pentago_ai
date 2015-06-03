#include <string>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <cassert>
#include <ctime>
#include <cstdlib>
#include <memory>

#include "Board.h"
#include "HumanPlayerController.h"
#include "RandomComputerController.h"
#include "MinimaxComputerController.h" 
#include "MctsComputerController.h" 
#include "Enums.h"
#include "Pentago.h"
#include "ControllerFactory.h"

static const char save_file_path[] = "game_state.txt";

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

PlayerController* prepare_player(const ControllerFactory& factory, bool ask_color, 
       const std::string& player_ordinal,
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


    PlayerController* player = factory.prompt_user_controller_selection(player_name,
            player_color, initial_board);

    return player;
}

std::unique_ptr<ControllerFactory> initialize_controller_factory() {
    auto factory = std::unique_ptr<ControllerFactory>(new ControllerFactory());
    
    factory->register_constructor("Player Controlled", 
        [](std::string name, PlayerColor color, const Board& initial_board) -> PlayerController* {
            return new HumanPlayerController(std::move(name), color);
        });

    factory->register_constructor("Computer (Random) Controlled", 
        [](std::string name, PlayerColor color, const Board& initial_board) -> PlayerController* {
            return new RandomComputerController(std::move(name), color);
        });

    factory->register_constructor("Computer (Minimax) Controlled", 
        [](std::string name, PlayerColor color, const Board& initial_board) -> PlayerController* {
            return new MinimaxComputerController(std::move(name), color, initial_board,
                4, 15.00);
        });

    return factory;
}

Pentago* start_new_game(const ControllerFactory& factory) {
    Board board;
    
    PlayerController* player1 = prepare_player(factory, true, "first", board);
    PlayerController* player2 = prepare_player(factory, false, "second", board,
            opposing_color(player1->color()));

    Pentago* game = new Pentago(board, player1, player2);
    return game;
}

Pentago* load_game(std::istream& stream, const ControllerFactory& factory) {
    Pentago* game = Pentago::load_game(stream, factory);
    return game;
}

int main(int argc, char** argv) {

    std::srand(std::time(NULL));

    auto controller_factory = initialize_controller_factory();

    std::cout << "Welcome to Pentago!" << std::endl;

    std::ifstream save_stream(save_file_path);

    Pentago* game = NULL;

    //Only present the menu if the save file exists, otherwise start a new game automatically.
    if(!save_stream.good()) {
        game = start_new_game(*controller_factory);
    }
    while(save_stream.good()) {
        std::cout << "Do you want to 1) Start a new game or 2) Continue the previous game? ";
        int option = 0;
        std::cin >> option;
        std::cout << std::endl;
        if(option == 1) {
            game = start_new_game(*controller_factory);
            break;
        } else if(option == 2) {
            game = load_game(save_stream, *controller_factory);
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
