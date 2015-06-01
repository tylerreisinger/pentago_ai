#ifndef MCTSCOMPUTERCONTROLLER_H__
#define MCTSCOMPUTERCONTROLLER_H__

#include "PlayerController.h"

#include <vector>


class MctsComputerController: public PlayerController
{
public:
    MctsComputerController(std::string name, PlayerColor color, const Board& board);
    ~MctsComputerController();

    virtual Move make_move(const Board& board, const Pentago& game);

private:

    std::vector<Move> m_potential_moves;
    std::vector<Move> m_move_loc_list;

    std::vector<Move> m_random_move_set;

    void build_static_move_list(const Board& board);

    float monte_carlo_trials(const Board& board, int count);
    bool monte_carlo_trial(const Board& board);

    void build_move_list(const Board& board);
};


#endif
    
