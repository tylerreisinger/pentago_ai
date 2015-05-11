#include "Pentago.h"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <stdexcept>

#include "HumanPlayerController.h"

Pentago::Pentago(Board board, PlayerController* player1, PlayerController* player2):
    m_board(board), m_current_player(player1), m_next_player(player2)
{
 
}
 
Pentago::~Pentago()
{
    delete m_current_player;
    delete m_next_player; 
}


 
WinStatus Pentago::play_game()
{
    //First, display the starting board
    std::cout << m_board;

    //Make moves until someone wins.
    while(m_board.check_for_wins() == NoWin) {
        
        //Get the move from the current player.
        std::cout << m_current_player->name() << "'s turn." << std::endl;
        Move move = m_current_player->make_move(m_board, *this);

        std::cout << move << std::endl;

        while(!is_valid_move(move)) {
            std::cout << "That is not a valid move." << std::endl;
            move = m_current_player->make_move(m_board, *this);
        }

        //Apply the move. If the game is won before the twist but after the piece
        //is played, apply_move will return a win. Otherwise, the win condition is
        //checked in the while condition.
        WinStatus pre_twist_win =
            m_board.apply_move(move, m_current_player->color());

        if(pre_twist_win != NoWin) {
            return pre_twist_win;
        }

        std::cout << m_board << std::endl;

        //Swap the players and store the move in the history.
        std::swap(m_current_player, m_next_player);
        m_past_moves.push_back(move);
        save_state();
    } 

    return m_board.check_for_wins();
}
 
void Pentago::serialize_state(std::ostream& stream)
{
    //Serialize the player data
    stream << m_current_player->name() << "\n";
    stream << m_next_player->name() << "\n";
    stream << color_to_serial_str(m_current_player->color()) << "\n"; 
    stream << color_to_serial_str(m_next_player->color()) << "\n"; 

    //We always serialize the next player to make a move first.
    stream << "1\n";

    //Now serialize the board
    for(int y = 0; y < m_board.board_size(); ++y) {
        for(int x = 0; x < m_board.board_size(); ++x) {
            stream << m_board.get_value_absolute(x, y);
        }
        stream << "\n";
    }

    //And the past moves
    for(std::vector<Move>::const_iterator it = m_past_moves.begin(); it != m_past_moves.end(); ++it) {
        stream << *it << "\n";
    }

    stream << std::endl;
}

PlayerColor char_to_color(char ch) {
    if(ch == 'b' || ch == 'B') {
        return BlackPlayer;
    } else {
        return WhitePlayer;
    }
}
 
Pentago* Pentago::load_game(std::istream& in_stream)
{
    std::string player1_name;
    std::string player2_name;

    char player1_color;
    char player2_color;

    //Load the player information
    if(!(in_stream >> player1_name)) {
        return NULL;
    }
    if(!(in_stream >> player2_name)) {
        return NULL;
    }
    if(!(in_stream >> player1_color)) {
        return NULL;
    }
    if(!(in_stream >> player2_color)) {
        return NULL;
    }

    //Create our controllers. In the future this will have to change to accomodate computer players.
    PlayerController* player1 = new HumanPlayerController(player1_name, char_to_color(player1_color));
    PlayerController* player2 = new HumanPlayerController(player2_name, char_to_color(player2_color));

    int next_player;
    if(!(in_stream >> next_player)) {
        return NULL;
    }

    //If the second listed player is first, swap the pointers
    if(next_player != 1) {
        std::swap(player1, player2);
    }

    //Ignore all characters up to the new line so getline starts with the board data.
    in_stream.ignore(1000, '\n');

    Board board(3, 2);

    //Read in the board state
    for(int i = 0; i < board.board_size(); ++i) {
        std::string line;
        std::getline(in_stream, line);
        if(line.size() < board.board_size()) {
            return NULL;
        }
        for(int j = 0; j < board.board_size(); ++j) {
            board.set_value_absolute(j, i, board_entry_from_char(line[j]));    
        }
    }

    Pentago* game = new Pentago(board, player1, player2);

    //Read in the list of past moves
    while(in_stream.good()) {
        try {
            Move move = Move::read_from_stream(in_stream);
            game->m_past_moves.push_back(move);
        } catch(std::runtime_error& e) {

        }
    }
    
    return game;
}
 
void Pentago::save_state()
{
    std::ofstream save_file(m_save_state_file_name.c_str());

    if(save_file.good()) {
        serialize_state(save_file);
    }
}
 
std::string Pentago::color_to_serial_str(PlayerColor color)
{
    if(color == WhitePlayer) {
        return "W";
    } else if(color == BlackPlayer) {
        return "B";
    } else {
        return "?";
    }
}
 
bool Pentago::is_valid_move(const Move& move) const
{
    //Check for out of bound or invalid values.
    if(move.play_cell() < 0 || move.play_cell() >= m_board.cell_count()) {
        return false;
    } 
    if(move.rotate_cell() < 0 || move.rotate_cell() >= m_board.cell_count()) {
        return false;
    } 
    if(move.play_index() < 0 || move.play_index() >= m_board.cell_size()*m_board.cell_size()) {
        return false;
    }
    if(move.rotation_direction() != RotateLeft && move.rotation_direction() != RotateRight) {
        return false;
    }

    //Check if the move's entry is already set.
    if(m_board.get_value(move.play_cell(), move.play_index()) != EmptyEntry) {
        return false;
    }
    return true;
}
 
