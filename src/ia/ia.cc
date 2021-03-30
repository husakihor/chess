#include "ia.hh"

namespace ai
{
    constexpr int max_value = 999999; // std::numeric_limits<int>::max();
    constexpr int min_value = -999999; // std::numeric_limits<int>::lowest();

    // PRNBQK
    std::vector<std::vector<int>> position_weights{
        { 0,  0,  0,   0,  0,  0,   0,  0,  5,  10, 10, -20, -20, 10, 10, 5,
          5,  -5, -10, 0,  0,  -10, -5, 5,  0,  0,  0,  20,  20,  0,  0,  0,
          5,  5,  10,  25, 25, 10,  5,  5,  10, 10, 20, 30,  30,  20, 10, 10,
          50, 50, 50,  50, 50, 50,  50, 50, 0,  0,  0,  0,   0,   0,  0,  0 },

        { 0,  0,  0,  5,  5,  0,  0,  0,  -5, 0, 0, 0, 0, 0, 0, -5,
          -5, 0,  0,  0,  0,  0,  0,  -5, -5, 0, 0, 0, 0, 0, 0, -5,
          -5, 0,  0,  0,  0,  0,  0,  -5, -5, 0, 0, 0, 0, 0, 0, -5,
          5,  10, 10, 10, 10, 10, 10, 5,  0,  0, 0, 0, 0, 0, 0, 0 },

        { -50, -40, -30, -30, -30, -30, -40, -50, -40, -20, 0,   5,   5,
          0,   -20, -40, -30, 5,   10,  15,  15,  10,  5,   -30, -30, 0,
          15,  20,  20,  15,  0,   -30, -30, 5,   15,  20,  20,  15,  5,
          -30, -30, 0,   10,  15,  15,  10,  0,   -30, -40, -20, 0,   0,
          0,   0,   -20, -40, -50, -40, -30, -30, -30, -30, -40, -50 },

        { -20, -10, -10, -10, -10, -10, -10, -20, -10, 5,   0,   0,   0,
          0,   5,   -10, -10, 10,  10,  10,  10,  10,  10,  -10, -10, 0,
          10,  10,  10,  10,  0,   -10, -10, 5,   5,   10,  10,  5,   5,
          -10, -10, 0,   5,   10,  10,  5,   0,   -10, -10, 0,   0,   0,
          0,   0,   0,   -10, -20, -10, -10, -10, -10, -10, -10, -20 },

        { -20, -10, -10, -5,  -5,  -10, -10, -20, -10, 0,   5,   0,   0, 0, 0,
          -10, -10, 5,   5,   5,   5,   5,   0,   -10, 0,   0,   5,   5, 5, 5,
          0,   -5,  -5,  0,   5,   5,   5,   5,   0,   -5,  -5,  0,   5, 5, 5,
          5,   0,   -5,  -10, 0,   5,   5,   5,   5,   0,   -10, -10, 0, 0, 0,
          0,   0,   0,   -10, -20, -10, -10, -5,  -5,  -10, -10, -20 },

        { 20,  30,  10,  0,   0,   10,  30,  20,  20,  20,  0,   0,   0,
          0,   20,  20,  -10, -20, -20, -20, -20, -20, -20, -10, -20, -30,
          -30, -40, -40, -30, 30,  -20, -30, -40, -40, -50, -50, -40, -40,
          -30, -30, -40, -40, -50, -50, -40, -40, -30, -30, -40, -40, -50,
          -50, -40, -40, -30, -30, -40, -40, -50, -50, -40, -40, -30 },

        // BLACK TABLES
        { 0,  0,  0,  0,   0,   0,  0,  0,  50, 50, 50,  50, 50, 50,  50, 50,
          10, 10, 20, 30,  30,  20, 10, 10, 5,  5,  10,  25, 25, 10,  5,  5,
          0,  0,  0,  20,  20,  0,  0,  0,  5,  -5, -10, 0,  0,  -10, -5, 5,
          5,  10, 10, -20, -20, 10, 10, 5,  0,  0,  0,   0,  0,  0,   0,  0 },

        { 0,  0, 0, 0, 0, 0, 0, 0,  5,  10, 10, 10, 10, 10, 10, 5,
          -5, 0, 0, 0, 0, 0, 0, -5, -5, 0,  0,  0,  0,  0,  0,  -5,
          -5, 0, 0, 0, 0, 0, 0, -5, -5, 0,  0,  0,  0,  0,  0,  -5,
          -5, 0, 0, 0, 0, 0, 0, -5, 0,  0,  0,  5,  5,  0,  0,  0 },

        { -50, -40, -30, -30, -30, -30, -40, -50, -40, -20, 0,   0,   0,
          0,   -20, -40, -30, 0,   10,  15,  15,  10,  0,   -30, -30, 5,
          15,  20,  20,  15,  5,   -30, -30, 0,   15,  20,  20,  15,  0,
          -30, -30, 5,   10,  15,  15,  10,  5,   -30, -40, -20, 0,   5,
          5,   0,   -20, -40, -50, -40, -30, -30, -30, -30, -40, -50 },

        { -20, -10, -10, -10, -10, -10, -10, -20, -10, 0,   0,   0,   0,
          0,   0,   -10, -10, 0,   5,   10,  10,  5,   0,   -10, -10, 5,
          5,   10,  10,  5,   5,   -10, -10, 0,   10,  10,  10,  10,  0,
          -10, -10, 10,  10,  10,  10,  10,  10,  -10, -10, 5,   0,   0,
          0,   0,   5,   -10, -20, -10, -10, -10, -10, -10, -10, -20 },

        { -20, -10, -10, -5,  -5,  -10, -10, -20, -10, 0,   0,   0,   0, 0, 0,
          -10, -10, 0,   5,   5,   5,   5,   0,   -10, -5,  0,   5,   5, 5, 5,
          0,   -5,  -5,  0,   5,   5,   5,   5,   0,   -5,  0,   0,   5, 5, 5,
          5,   0,   -5,  -10, 5,   5,   5,   5,   5,   0,   -10, -10, 0, 5, 0,
          0,   0,   0,   -10, -20, -10, -10, -5,  -5,  -10, -10, -20 },

        { -30, -40, -40, -50, -50, -40, -40, -30, -30, -40, -40, -50, -50,
          -40, -40, -30, -30, -40, -40, -50, -50, -40, -40, -30, -30, -40,
          -40, -50, -50, -40, -40, -30, -20, -30, -30, -40, -40, -30, 30,
          -20, -10, -20, -20, -20, -20, -20, -20, -10, 20,  20,  0,   0,
          0,   0,   20,  20,  20,  30,  10,  0,   0,   10,  30,  20 }
    };

    void ai_loop()
    {
        std::string name = "AlphaMonkey";
        while (true)
        {
            UCITime time = UCITime();
            board::Chessgame game = parse_uci_board(get_board(name, time));
            std::cerr << game.print_full_board() << std::endl;

            bool is_white = game.playing_color_ == board::Color::WHITE;
            board::Player player =
                is_white ? game.white_player_ : game.black_player_;

            board::Move bestMove = minimax_root(game);

            std::cerr << bestMove << std::endl;
            play_this_move(bestMove);
            game.execute_move(bestMove);
            std::cerr << game.print_full_board() << std::endl;
        }
    }

    board::Chessgame parse_uci_board(std::string uci)
    {
        // std::cerr << uci << std::endl;
        // std::cerr << "Making game from uci\n";
        uci.append(" ");

        std::string base =
            "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

        auto start = 0U;
        auto end = uci.find(" ");
        auto index = 0;

        board::Chessgame game;

        while (end != std::string::npos)
        {
            auto str = uci.substr(start, end - start);
            start = end + 1;
            end = uci.find(" ", start);

            if (index == 1 && str == "startpos")
            {
                game = board::Chessgame(base);
            }

            if (index == 1 && str == "fen")
            {
                game = board::Chessgame(uci.substr(start, uci.length()));
            }

            if (str == "moves")
            {
                parse_moves(uci.substr(start, uci.length()), game);
            }

            index++;
        }

        // std::cerr << "Returning made game\n";
        return game;
    }

    board::Move uci_to_move(std::string uci, board::Chessgame &game)
    {
        int start = (uci[0] - 'a') + ((uci[1] - '1') * 8);
        int end = (uci[2] - 'a') + ((uci[3] - '1') * 8);
        board::PieceType type = board::PieceType::PAWN;
        bool captured = false;

        std::string pieces_array = "PRNBQK";

        for (int i = 0; i < 12; i++)
        {
            if (game.get_boards()[i] & (1ULL << start))
            {
                type = board::char_to_piece(pieces_array[i % 6]);
            }

            if (game.get_boards()[i] & (1ULL << end))
            {
                captured = true;
            }
        }

        if (uci.size() == 5)
        {
            board::PieceType promotion = board::PieceType::QUEEN;
            if (uci[4] == 'b')
            {
                promotion = board::PieceType::BISHOP;
            }
            else if (uci[4] == 'r')
            {
                promotion = board::PieceType::ROOK;
            }
            else if (uci[4] == 'n')
            {
                promotion = board::PieceType::KNIGHT;
            }

            return board::Move(start, end, type, captured, promotion);
        }

        return board::Move(start, end, type, captured);
    }

    void parse_moves(std::string str_move_list, board::Chessgame &game)
    {
        auto start = 0U;
        auto end = str_move_list.find(" ");

        if (end == std::string::npos)
        {
            board::Move move = uci_to_move(str_move_list, game);
            game.execute_move(move);
        }

        while (end != std::string::npos)
        {
            auto str = str_move_list.substr(start, end - start);

            start = end + 1;
            end = str_move_list.find(" ", start);

            board::Move move = uci_to_move(str, game);
            game.execute_move(move);
        }
    }

    int eval_game_board(board::Chessgame &game)
    {
        // PRNBQKprnbqk
        std::vector<int> pieces_weights = { 100, 500, 320, 330, 900, 0 };
        int total_weight = 0;
        bool is_white = game.playing_color_ == board::Color::WHITE;

        for (int i = 0; i < 12; i++)
        {
            for (int j = 0; j < 64; j++)
            {
                if (board::Chessgame::is_occupied(game.get_boards()[i], j))
                {
                    if (i < 6)
                    {
                        if (is_white)
                            total_weight += pieces_weights[(i + 6) % 6]
                                + position_weights[(i + 6) % 6][j];
                        else
                            total_weight -= pieces_weights[(i + 6) % 6]
                                - position_weights[((i + 6) % 6) + 6][j];
                    }
                    else
                    {
                        if (is_white)
                            total_weight -= pieces_weights[(i + 6) % 6]
                                - position_weights[(i + 6) % 6][j];
                        else
                            total_weight += pieces_weights[(i + 6) % 6]
                                + position_weights[((i + 6) % 6) + 6][j];
                    }
                }
            }
        }
        return total_weight;
    }

    int minimax(board::Chessgame &game, int depth, int alpha, int beta,
                bool is_maximising)
    {
        // Check if there is a checkmate to return
        if (depth == 0)
        {
            return is_maximising ? eval_game_board(game)
                                 : -eval_game_board(game);
        }

        board::Player current_player =
            game.playing_color_ == board::Color::WHITE ? game.white_player_
                                                       : game.black_player_;
        std::vector<board::Move> move_list =
            current_player.generate_player_moves(game);

        if (move_list.size() == 0)
        {
            int speed = depth == 3 ? 10000 : 0;
            if (current_player.is_in_check(game))
                return is_maximising ? min_value - speed : max_value + speed;
            return is_maximising ? eval_game_board(game)
                                 : -eval_game_board(game);
        }

        if (is_maximising)
        {
            int max_eval = min_value;
            for (auto move : move_list)
            {
                board::Chessgame tmp_game = game;
                tmp_game.execute_move(move);
                int eval_move =
                    minimax(tmp_game, depth - 1, alpha, beta, false);
                max_eval = std::max(max_eval, eval_move);
                alpha = std::max(alpha, eval_move);
                if (beta <= alpha)
                    break;
            }
            return max_eval;
        }
        else
        {
            int min_eval = max_value;
            for (auto move : move_list)
            {
                board::Chessgame tmp_game = game;
                tmp_game.execute_move(move);
                int eval_move = minimax(tmp_game, depth - 1, alpha, beta, true);
                min_eval = std::min(min_eval, eval_move);
                beta = std::min(beta, eval_move);
                if (beta <= alpha)
                    break;
            }
            return min_eval;
        }
    }

    board::Move minimax_root(board::Chessgame &game)
    {
        board::Player current_player =
            game.playing_color_ == board::Color::WHITE ? game.white_player_
                                                       : game.black_player_;
        std::vector<board::Move> move_list =
            current_player.generate_player_moves(game);
        int max = min_value;
        int dept = 3;
        /*if (move_list.size() < 25)
            dept = 4;*/
        board::Move best_move_max = move_list[0];
        for (auto move : move_list)
        {
            board::Chessgame tmp_game = game;
            tmp_game.execute_move(move);
            int res_move = minimax(tmp_game, dept, min_value, max_value, false);
            if (res_move > max)
            {
                max = res_move;
                best_move_max = move;
            }
        }
        return best_move_max;
    }
} // namespace ai
