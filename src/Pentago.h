#ifndef PENTAGO_H__
#define PENTAGO_H__

#include <ostream>
#include <fstream>
#include <istream>
#include <vector>

#include "Board.h"
#include "PlayerController.h"
#include "Enums.h"

class Pentago
{
public:
    Pentago(Board board, PlayerController* player1, PlayerController* player2);
    ~Pentago();

    WinStatus play_game();

    void serialize_state(std::ostream& stream);

    const Board& board() const {return m_board;}

    void set_save_file(std::string file_name) {m_save_state_file_name = file_name;}

    static Pentago* load_game(std::istream& in_stream);

protected:
    Board m_board;

    PlayerController* m_current_player;
    PlayerController* m_next_player;

    std::vector<Move> m_past_moves;
    std::string m_save_state_file_name;
private:
    void save_state();

    std::string color_to_serial_str(PlayerColor color);

    bool is_valid_move(const Move& move) const;

};


#endif
    
