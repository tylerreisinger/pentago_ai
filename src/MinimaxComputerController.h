#ifndef MINIMAXCOMPUTERCONTROLLER_H__
#define MINIMAXCOMPUTERCONTROLLER_H__

#define DIAGNOSTICS 1


#include "PlayerController.h"

#include <string>
#include <vector>
#include <array>

class MinimaxComputerController: public PlayerController
{
public:
    MinimaxComputerController(std::string name, PlayerColor color, 
            const Board& board, int max_depth, float max_turn_time);
    ~MinimaxComputerController();

    virtual Move make_move(const Board& board, const Pentago& game);

private:
    static const int BEST_RUN_COUNT = 3;

    void find_runs(const Board& board, std::array<int, BEST_RUN_COUNT>& player_runs,
            std::array<int, BEST_RUN_COUNT>& opponent_runs);

    void add_run(std::array<int, BEST_RUN_COUNT>& player_runs,
            std::array<int, BEST_RUN_COUNT>& opponent_runs,
            int run_len, BoardEntry entry_kind);

    void build_static_move_list(const Board& board);

    float score_board(const Board& board);

    void center_control_scores(const Board& board, float& player_score,
            float& opponent_score);

    float run_score(const std::array<int, BEST_RUN_COUNT>& runs);
    void run_scores(const Board& board, float& player_score, float& opponent_score);

    Move minimax_2(const Board& board, int depth_bound, float& max);
    Move minimax_max_value(const Board& board, int depth_bound, float alpha, float beta, 
            float& value);
    Move minimax_min_value(const Board& board, int depth_bound, float alpha, float beta, 
            float& value);

    std::vector<Move> m_potential_moves;
    std::vector<Move> m_move_loc_list;

    std::vector<Move> m_killer_moves;

    float m_coeff_center_control;
    float m_coeff_longest_run;

    int m_max_depth;
    float m_max_turn_time;

    int m_node_evals;

    clock_t m_search_start_time;
    bool m_time_cancel;
};


#endif
    
