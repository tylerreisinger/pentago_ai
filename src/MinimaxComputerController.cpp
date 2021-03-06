#include "MinimaxComputerController.h"

#include <cstdlib>
#include <iostream> 
#include <algorithm>    
#include <ctime>
#include <cassert>

static const float POS_INF = 1e10;
static const float NEG_INF = -1e10;
static const int KILLER_COUNT = 4;

bool scan_compare(BoardEntry entry, BoardEntry scan_entry, int& run_len);
int horiz_scan(const Board& board, int x, int y, BoardEntry entry);
int vert_scan(const Board& board, int x, int y, BoardEntry entry);
int diag_scan(const Board& board, int x, int y, BoardEntry entry);
int diag_scan_neg(const Board& board, int x, int y, BoardEntry entry);

MinimaxComputerController::MinimaxComputerController(std::string name, PlayerColor color,
        const Board& board, int max_depth, float max_turn_time):
    PlayerController(name, color), m_max_depth(max_depth),
    m_max_turn_time(max_turn_time)
{

    m_coeff_center_control = 2.50;
    m_coeff_longest_run = 1.00;

    m_move_loc_list.resize(board.total_entries(), Move::invalid_move());
    build_static_move_list(board);
}

MinimaxComputerController::~MinimaxComputerController() {

}

Move MinimaxComputerController::make_move(const Board& board, const Pentago& game) {
    
    m_time_cancel = false;
    m_search_start_time = std::clock();
    m_node_evals = 0;
    std::cout << "score = " << score_board(board) << std::endl;

    build_static_move_list(board);

    float max = 0.0;
    float old_max = 0.0;

    int depth = 0;

    clock_t elapsed_time = std::clock() - m_search_start_time;

    Move move = Move::invalid_move();

    //Apply iterative deepening. This not only allows the highest depth for the
    //time constrait to be chosen, but guarentees that the quickest win will be
    //selected.
    while(elapsed_time <= m_max_turn_time*CLOCKS_PER_SEC && depth < m_max_depth) {
        m_killer_moves.resize(KILLER_COUNT*(depth+1), Move::invalid_move());
        Move new_move = minimax_2(board, depth, max);
        if(!m_time_cancel) {
            move = new_move;
            depth += 1;
        } else {
            max = old_max;
            break;
        }
        
        //Check if we have a winning move. We should't go deeper if we do.
        Board board_copy = board.clone();
        WinStatus early_win = board_copy.apply_move(new_move, color());
        WinStatus late_win = board_copy.check_for_wins();
        if(early_win == player_win_kind() || late_win == player_win_kind()) {
            break;
        }

        elapsed_time = std::clock() - m_search_start_time;
        std::swap(old_max, max);
    }

    std::cout << "max = " << max << std::endl;
    std::cout << "depth = " << depth << std::endl;

    Board board_copy = board.clone();
    board_copy.apply_move_no_check(move, color());

    std::cout << "end score = " << score_board(board_copy) << std::endl;
    std::cout << "node evals = " << m_node_evals << std::endl;

    clock_t end_time = std::clock();

    double elapsed_s = static_cast<double>(end_time - m_search_start_time) / CLOCKS_PER_SEC;

    std::cout << "Move time: " << elapsed_s << " seconds" << std::endl;

    return move; 
}

 
//Create a semi-random list of all valid moves given board. Only the play positions
//are randomized, all 8 rotations for each move are together to allow for skipping
//8 moves ahead any time a non-empty entry is found by minimax.
void MinimaxComputerController::build_static_move_list(const Board& board)
{
    const int MOVES_PER_ENTRY = board.cell_count()*2;

    int move_count = 0;
    //Shuffle the locations
    for(int cell = 0; cell < board.cell_count(); ++cell) {
        for(int entry = 0; entry < board.entries_per_cell(); ++entry) {
            m_move_loc_list[move_count] = Move(cell, entry, 1,  RotateRight);
            move_count += 1;
        }
    }
    std::random_shuffle(m_move_loc_list.begin(), m_move_loc_list.begin()+move_count);

    m_potential_moves.clear();
    m_potential_moves.reserve(move_count*board.cell_count()*2+KILLER_COUNT);

    //Killer move slots
    for(int i = 0; i < KILLER_COUNT; ++i) {
        m_potential_moves.push_back(Move::invalid_move());
    }

    //Build the actual moves from the locations
    for(int i = 0; i < move_count; ++i) {
        for(int rot_cell = 0; rot_cell < board.cell_count(); ++rot_cell) {
            m_potential_moves.push_back(Move(m_move_loc_list[i].play_cell(),
                    m_move_loc_list[i].play_index(), rot_cell, RotateLeft));
            m_potential_moves.push_back(Move(m_move_loc_list[i].play_cell(),
                    m_move_loc_list[i].play_index(), rot_cell, RotateRight));
        }        
    }
}
 
//Return a score for the given board. A positive score represents a "good" situation
//and a negative one represents a good situation for the opponent.
float MinimaxComputerController::score_board(const Board& board)
{
    float player_score = 0.0;
    float opponent_score = 0.0;

    float player_score_partial = 0.0;
    float opponent_score_partial = 0.0;
    
    center_control_scores(board, player_score_partial, opponent_score_partial);

    player_score += player_score_partial;
    opponent_score += opponent_score_partial;

    run_scores(board, player_score_partial, opponent_score_partial);

    player_score += player_score_partial;
    opponent_score += opponent_score_partial;

    return player_score - opponent_score;
}

//Scan for the top 3 runs of each player. This contains most of minimax's runtime.
void MinimaxComputerController::find_runs(const Board& board,
        std::array<int, BEST_RUN_COUNT>& player_runs,
        std::array<int, BEST_RUN_COUNT>& opponent_runs) {

    for(int y = 0; y < board.board_size(); ++y) {
        for(int x = 0; x < board.board_size()-1; ++x) {
            BoardEntry entry = board.get_value_absolute(x, y);

            if(entry != EmptyEntry) {
                int run_len = horiz_scan(board, x, y, entry);
                add_run(player_runs, opponent_runs, run_len, entry);    
            }
        }        
    }

    for(int y = 0; y < board.board_size()-1; ++y) {
        for(int x = 0; x < board.board_size(); ++x) {
            BoardEntry entry = board.get_value_absolute(x, y);

            if(entry != EmptyEntry) {
                int run_len = vert_scan(board, x, y, entry);
                add_run(player_runs, opponent_runs, run_len, entry);    
            }
        }        
    }

    for(int y = 0; y < board.board_size()-1; ++y) {
        for(int x = 0; x < board.board_size()-1; ++x) {
            BoardEntry entry = board.get_value_absolute(x, y);

            if(entry != EmptyEntry) {
                int run_len = diag_scan(board, x, y, entry);
                add_run(player_runs, opponent_runs, run_len, entry);    
            }
        }        
    }

    for(int y = 1; y < board.board_size(); ++y) {
        for(int x = 0; x < board.board_size()-1; ++x) {
            BoardEntry entry = board.get_value_absolute(x, y);

            if(entry != EmptyEntry) {
                int run_len = diag_scan_neg(board, x, y, entry);
                add_run(player_runs, opponent_runs, run_len, entry);    
            }
        }        
    }
}

//Maintain the list of top 3 runs
void insert_shift_down(int val, std::array<int, 3>& array) {
    if(val > array[0]) {
        array[2] = array[1];
        array[1] = array[0];
        array[0] = val;
    } else if(val > array[1]) {
        array[2] = array[1];
        array[1] = val;
    } else {
        array[2] = val;
    }
}

void MinimaxComputerController::add_run(
        std::array<int, BEST_RUN_COUNT>& player_runs,
        std::array<int, BEST_RUN_COUNT>& opponent_runs,
        int run_len, BoardEntry entry_kind)
{
    if(run_len > player_runs[BEST_RUN_COUNT-1]) {
        BoardEntry player_entry_kind = player_color_to_board_entry(color());
        
        bool is_player = (player_entry_kind == entry_kind); 
        if(is_player) {
            insert_shift_down(run_len, player_runs);
        } else {
            insert_shift_down(run_len, opponent_runs);
        }
    }
}

//Scanning helper functions. Easily inlined (mostly), these are a trade off between
//readable/changable and fast code.

int horiz_scan(const Board& board, int x, int y, BoardEntry entry) {
    int run_len = 0;

    for(int x2 = x+1; x2 < board.board_size(); ++x2) {
        BoardEntry scan_entry = board.get_value_absolute(x2, y);

        if(!scan_compare(entry, scan_entry, run_len)) {
            break;
        }

        if(x2-x >= 4) {
            break;
        }
    }

    return run_len;
}

int vert_scan(const Board& board, int x, int y, BoardEntry entry) {
    int run_len = 0;

    for(int y2 = y+1; y2 < board.board_size(); ++y2) {
        BoardEntry scan_entry = board.get_value_absolute(x, y2);

        if(!scan_compare(entry, scan_entry, run_len)) {
            break;
        }
        if(y2-y >= 4) {
            break;
        }
    }

    return run_len;
}

int diag_scan(const Board& board, int x, int y, BoardEntry entry) {
    int run_len = 0;

    int max_run = board.board_size() - std::max(x, y);
    max_run = std::min(max_run, 5);
    for(int off = 1; off < max_run; ++off) {
        BoardEntry scan_entry = board.get_value_absolute(x+off, y+off);

        if(!scan_compare(entry, scan_entry, run_len)) {
            break;
        }
    }
    return run_len;
}

int diag_scan_neg(const Board& board, int x, int y, BoardEntry entry) {
    int run_len = 0;

    int max_run = std::max(board.board_size() - x, y);
    max_run = std::min(max_run, 5);
    for(int off = 1; off < max_run; ++off) {
        BoardEntry scan_entry = board.get_value_absolute(x+off, y-off);

        if(!scan_compare(entry, scan_entry, run_len)) {
            break;
        }
    }
    return run_len;
}


bool scan_compare(BoardEntry entry, BoardEntry scan_entry, int& run_len) {
    if(scan_entry == EmptyEntry) {
        return true; 
    } else if(entry == scan_entry) {
        ++run_len;
        return true;
    } else {
        return false;
    }     
}

//In general, center tiles are better than edge tiles (confirmed by a monte-carlo
//tree search approach to the problem), so we give them a small positive score.
//Really only affects the early game much.
void MinimaxComputerController::center_control_scores(const Board& board,
        float& player_score, float& opponent_score)
{
    BoardEntry player_entry_kind = player_color_to_board_entry(color());
    player_score = 0.0;
    opponent_score = 0.0;

    for(int i = 0; i < 4; ++i) {
        BoardEntry center_entry = board.get_value(i, 4);
        if(center_entry == player_entry_kind) {
            player_score += 1;
        } else if(center_entry != EmptyEntry) {
            opponent_score += 1;
        }
    }

    player_score *= m_coeff_center_control;
    opponent_score *= m_coeff_center_control;
}
 
//Score runs. The top three are considered, with decreasing significants as you go down.
//Scores for length are the length - 1 squared. This makes the ai more aggressively block
//near wins.
float MinimaxComputerController::run_score(const std::array<int, BEST_RUN_COUNT>& runs)
{

    float score = 0.0;
    if(runs[0] >= Board::WIN_SIZE-1) {
        return POS_INF;
    }

    for(int i = 0; i < BEST_RUN_COUNT; ++i) {
        int urgency_coeff = runs[i];
        score += runs[i]*urgency_coeff*m_coeff_longest_run * (1.0/(1.0+i*2.0));
    }

    return score; 
}
 

void MinimaxComputerController::run_scores(const Board& board,
       float& player_score, float& opponent_score)
{
    std::array<int, BEST_RUN_COUNT> player_runs;
    std::array<int, BEST_RUN_COUNT> opponent_runs;

    player_runs.fill(0);
    opponent_runs.fill(0);

    find_runs(board, player_runs, opponent_runs);

    player_score = run_score(player_runs);
    opponent_score = run_score(opponent_runs);
}

//Minimax entry. Returns a chosen move.
Move MinimaxComputerController::minimax_2(const Board& board, int depth_bound, float& value)
{
    return minimax_max_value(board, depth_bound, NEG_INF*10, POS_INF*10, value); 
}
 
//Minimax function for max levels of the tree. Adapted from the text book description.
Move MinimaxComputerController::minimax_max_value(const Board& board, int depth_bound,
        float alpha, float beta, float& value)
{
    if(std::clock() - m_search_start_time >= CLOCKS_PER_SEC*m_max_turn_time) {
        m_time_cancel = true;
        return Move::invalid_move();
    }

    m_node_evals += 1;
    float board_score = score_board(board);
    if(board_score > 1000.0 || board_score < -1000.0) {
        value = board_score;
        return Move::invalid_move();
    }    

    value = NEG_INF*10;
    Move move = Move::invalid_move();

    int i = copy_killers_to_move_list(depth_bound, board);
    
    for(; i < m_potential_moves.size(); ++i) {
        Move player_move = m_potential_moves[i];
        if(!board.is_cell_empty(player_move.play_cell(), player_move.play_index())) {
            i += 7;
            continue;
        }

        Board new_state = board.clone();
        new_state.apply_move_no_check(player_move, color());
        
        float inner_value = 0.0;

        if(depth_bound > 0) { 
            Move m = minimax_min_value(new_state, depth_bound-1,
                    alpha, beta, inner_value);
        } else {
            inner_value = score_board(new_state);
            m_node_evals += 1;
        }

        if (inner_value > value) {
            value = inner_value;
            move = player_move;
            //We found a win. Nothing will score less than it, so searching onward
            //is a waste of time.
            
            if(value > 10000) {
                return move;
            }
        }

        if(value > beta) {
            add_killer(depth_bound, player_move);
            return move;
        }

        alpha = std::max(alpha, value);
    }
    return move;
}
 
//Minimax function for min levels of the tree. Adapted from the text book description.
Move MinimaxComputerController::minimax_min_value(const Board& board, int depth_bound,
        float alpha, float beta, float& value)
{
    if(std::clock() - m_search_start_time >= CLOCKS_PER_SEC*m_max_turn_time) {
        m_time_cancel = true;
        return Move::invalid_move();
    }

    m_node_evals += 1;
    float board_score = score_board(board);

    if(board_score < -1000.0 || board_score > 1000.0) { 
        value = board_score;
        return Move::invalid_move();
    }

    value = POS_INF*10;
    Move move = Move::invalid_move();

    int i = copy_killers_to_move_list(depth_bound, board);

    for(; i < m_potential_moves.size(); ++i) {
        Move player_move = m_potential_moves[i];
        if(!board.is_cell_empty(player_move.play_cell(), player_move.play_index())) {
            i += 7;
            continue;
        }

        Board new_state = board.clone();
        new_state.apply_move_no_check(player_move, opposing_color(color())); 

        float inner_value = 0.0;

        if(depth_bound > 0) {
            minimax_max_value(new_state, depth_bound-1, alpha, beta, inner_value);
        } else {
            inner_value = score_board(new_state);
            m_node_evals += 1;
        }

        if(inner_value < value) {
            value = inner_value;
            move = player_move;

            //We found a loss. Nothing will score less than it, so searching onward
            //is a waste of time.
            if(value < -100000) {
                return move;
            }
        }

        if(value < alpha) {
            add_killer(depth_bound, player_move);
            return move;
        }

        beta = std::min(beta, value);
    }

    return move;
}

void MinimaxComputerController::add_killer(int depth_bound, Move move)
{
    int killer_start_idx = depth_bound*KILLER_COUNT;

    for(int j = 0; j < KILLER_COUNT; ++j) {
        if(m_killer_moves[killer_start_idx+j] == move) {
          return; 
        }
    }

    for(int j = 1; j < KILLER_COUNT; ++j) {
        m_killer_moves[killer_start_idx+j-1] = m_killer_moves[killer_start_idx+j];
    }

    m_killer_moves[killer_start_idx+KILLER_COUNT-1] = move;
}
 
int MinimaxComputerController::copy_killers_to_move_list(int depth_bound, const Board& board)
{
    int first_move = 0;
    for(int j = 0; j < KILLER_COUNT; ++j) {
        Move killer_move = m_killer_moves[depth_bound*KILLER_COUNT+j];
        if(!killer_move.is_invalid() && 
                board.is_cell_empty(killer_move.play_cell(),
                killer_move.play_index())) {
            m_potential_moves[j] = killer_move;
        } else {
            first_move = j+1;
        }
    }

    return first_move;
}
