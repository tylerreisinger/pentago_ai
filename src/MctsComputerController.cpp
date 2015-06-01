#include "MctsComputerController.h"

#include <algorithm>
#include <iostream>

MctsComputerController::MctsComputerController(std::string name, PlayerColor color,
        const Board& board): PlayerController(name, color)
{
    m_move_loc_list.resize(board.total_entries(), Move::invalid_move());
    m_random_move_set.resize(37, Move::invalid_move());
}
 
Move MctsComputerController::make_move(const Board& board, const Pentago& game)
{
    build_static_move_list(board);

    float max_val = -1.0;
    Move best_move = Move::invalid_move();

    for(int i = 0; i < m_potential_moves.size(); ++i) {
        Move move = m_potential_moves[i];
        Board move_board = board.clone();
        move_board.apply_move_no_check(move, color());

        float score = monte_carlo_trials(move_board, 1000);

        if(score > max_val) {
            max_val = score;
            best_move = move;
        }

        //std::cout << score << " - " << move << std::endl;
    } 
    
    std::cout << max_val << " - " << best_move << std::endl;
    return best_move;
}
 
void MctsComputerController::build_static_move_list(const Board& board)

{
    const int MOVES_PER_ENTRY = board.cell_count()*2;

    int move_count = 0;
    for(int cell = 0; cell < board.cell_count(); ++cell) {
        for(int entry = 0; entry < board.entries_per_cell(); ++entry) {
            if(board.is_cell_empty(cell, entry)) {
                m_move_loc_list[move_count] = Move(cell, entry, 1,  RotateRight);
                move_count += 1;
            }
        }
    }

    std::random_shuffle(m_move_loc_list.begin(), m_move_loc_list.begin()+move_count);
    

    m_potential_moves.clear();
    m_potential_moves.reserve(move_count*board.cell_count()*2);

    for(int i = 0; i < move_count; ++i) {
        for(int rot_cell = 0; rot_cell < board.cell_count(); ++rot_cell) {
            m_potential_moves.push_back(Move(m_move_loc_list[i].play_cell(),
                    m_move_loc_list[i].play_index(), rot_cell, RotateLeft));
            m_potential_moves.push_back(Move(m_move_loc_list[i].play_cell(),
                    m_move_loc_list[i].play_index(), rot_cell, RotateRight));
        }        
    }
}

float MctsComputerController::monte_carlo_trials(const Board& board, int count)
{
    int wins = 0;
    for(int i = 0; i < count; ++i) {
        build_move_list(board);
        if(monte_carlo_trial(board)) {
            wins += 1;
        }
    } 
    return static_cast<float>(wins) / static_cast<float>(count);
}
 
bool MctsComputerController::monte_carlo_trial(const Board& board)
{
    PlayerColor player_color = color();
    PlayerColor opponent_color = opposing_color(player_color);
    std::swap(player_color, opponent_color);

    WinStatus win_status = board.check_for_wins();

    Board board_copy = board.clone();

    int i = 0;
    while(win_status == NoWin && i < 36) {
        Move move = m_random_move_set[i];
        if(move.is_invalid()) {
            break;
        }
        board_copy.apply_move_no_check(move, player_color);
        i += 1;
        std::swap(player_color, opponent_color);
        win_status = board_copy.check_for_wins();
    }

    win_status = board_copy.check_for_wins();

    if((win_status == WhiteWin && color() == WhitePlayer) ||
            (win_status == BlackWin && color() == BlackPlayer)) {
        return true;
    }

    return false;
}
 
void MctsComputerController::build_move_list(const Board& board)
{
    int i = 0;
    for(int cell = 0; cell < board.cell_count(); ++cell) {
        for(int entry = 0; entry < board.entries_per_cell(); ++entry) {
            if(board.is_cell_empty(cell, entry)) {
                int rot_cell = std::rand() % board.cell_count();
                int dir = std::rand() % 2;
                m_random_move_set[i] = Move(cell, entry, rot_cell, 
                    dir == 0 ? RotateLeft : RotateRight);
                i+=1;
            }
        } 
    }
    std::random_shuffle(m_random_move_set.begin(), m_random_move_set.begin()+i);
    m_random_move_set[i] = Move::invalid_move();
}
 
