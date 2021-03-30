#pragma once

#include "../board/chessgame.hh"
#include "../parsing/uci.hh"

namespace ai
{
    int eval_game_board(board::Chessgame &game);
    int minimax(board::Chessgame &game, int depth, int alpha, int beta,
                bool is_maximising);
    board::Move minimax_root(board::Chessgame &game);
    void ai_loop();
    board::Chessgame parse_uci_board(std::string uci);
    board::Move uci_to_move(std::string uci, board::Chessgame &game);
    void parse_moves(std::string str_move_list, board::Chessgame &game);
} // namespace ai
