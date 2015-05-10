#include "Pentago.h"

#include <algorithm>
#include <iostream>

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
    std::cout << "Welcome to Pentago!" << std::endl;
    std::cout << m_board;

    while(m_board.check_for_wins() == NoWin) {
        
        std::cout << m_current_player->name() << "'s turn." << std::endl;
        Move move = m_current_player->make_move(m_board, *this);

        std::cout << move << std::endl;
        while(!is_valid_move(move)) {
            std::cout << "That is not a valid move." << std::endl;
            move = m_current_player->make_move(m_board, *this);
        }

        WinStatus pre_twist_win =
            m_board.apply_move(move, m_current_player->color());

        if(pre_twist_win != NoWin) {
            return pre_twist_win;
        }

        std::cout << move << std::endl;
        std::cout << m_board << std::endl;

        std::swap(m_current_player, m_next_player);
    } 

    return m_board.check_for_wins();
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
 
