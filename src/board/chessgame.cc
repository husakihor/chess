#include "chessgame.hh"

namespace board
{
    // Generation function
    Chessgame::Chessgame()
    {
        black_player_ = Player(false);
        white_player_ = Player(true);
        castling_K_ = castling_Q_ = castling_k_ = castling_q_ = true;
        en_passant_w_ = en_passant_b_ = -1;
        playing_color_ = Color::WHITE;
        for (int i = 0; i < 12; i++)
        {
            boards_.push_back(0);
        }
        init_basic_board();
        generate_color_boards();
    }

    Chessgame::Chessgame(std::string fen_config)
    {
        black_player_ = Player(false);
        white_player_ = Player(true);
        castling_K_ = castling_Q_ = castling_k_ = castling_q_ = false;
        en_passant_w_ = en_passant_b_ = -1;
        for (int i = 0; i < 12; i++)
        {
            boards_.push_back(0);
        }
        size_t start = 0;
        size_t end = fen_config.find(" ");
        int index = 0;

        while (end != std::string::npos)
        {
            std::string tmp_str = fen_config.substr(start, end - start);
            start = end + 1;
            end = fen_config.find(" ", start);

            if (index == 0)
            {
                init_board_from_fen(tmp_str);
                generate_color_boards();
            }
            else if (index == 1)
            {
                playing_color_ =
                    (!tmp_str.compare("w") ? Color::WHITE : Color::BLACK);
            }
            else if (index == 2)
            {
                for (char c : tmp_str)
                {
                    if (c == 'K')
                    {
                        castling_K_ = true;
                    }
                    else if (c == 'k')
                    {
                        castling_k_ = true;
                    }
                    else if (c == 'Q')
                    {
                        castling_Q_ = true;
                    }
                    else if (c == 'q')
                    {
                        castling_q_ = true;
                    }
                }
            }
            else if (index == 3)
            {
                if (!tmp_str.compare("-"))
                {
                    en_passant_w_ = -1;
                    en_passant_b_ = -1;
                }
                else
                {
                    int file = (tmp_str[0] - 'a');
                    int rank = tmp_str[1] - '0' - 1;
                    if (playing_color_ == Color::WHITE)
                        en_passant_b_ =
                            Position((File)file, (Rank)rank).pos_to_index();
                    else
                        en_passant_b_ =
                            Position((File)file, (Rank)rank).pos_to_index();
                }
            }
            index++;
        }
    }

    // Index functions
    void Chessgame::modify_index(int board_index, int index)
    {
        boards_[board_index] &= ~(1ULL << index);
        boards_[board_index] |= (1ULL << index);
    }

    void Chessgame::clear_index(int board_index, int index)
    {
        boards_[board_index] &= ~(1ULL << index);
    }

    int Chessgame::index_from_char(char symbol)
    {
        std::string pieces_chars = "PRNBQKprnbqk";
        size_t res_index = pieces_chars.find(symbol);
        if (res_index != std::string::npos)
        {
            return res_index;
        }
        return -1;
    }

    // Printing functions
    std::string Chessgame::print_single_board(int board_index)
    {
        std::string pieces_chars = "PRNBQKprnbqk";
        char occupied = pieces_chars[board_index];
        std::string ret = "";
        for (int i = 7; i >= 0; i--)
        {
            for (int j = i * 8; j < (i + 1) * 8; j++)
            {
                if (is_occupied(boards_[board_index], j))
                {
                    ret += occupied;
                    ret += ' ';
                }
                else
                    ret += ". ";
            }
            ret += '\n';
        }
        return ret;
    }

    std::string Chessgame::print_full_board()
    {
        std::string ret = "";
        std::string pieces_chars = "PRNBQKprnbqk";
        for (int i = 7; i >= 0; i--)
        {
            for (int j = i * 8; j < (i + 1) * 8; j++)
            {
                bool piece_found = false;
                for (int k = 0; k < 12 && !piece_found; k++)
                {
                    if (is_occupied(boards_[k], j))
                    {
                        ret += pieces_chars[k];
                        ret += ' ';
                        piece_found = true;
                    }
                }
                if (!piece_found)
                {
                    ret += ". ";
                }
            }
            ret += '\n';
        }
        return ret;
    }

    std::string Chessgame::fen_from_game()
    {
        std::string ret = "";
        std::string pieces_chars = "PRNBQKprnbqk";
        int count = 0;
        for (int i = 7; i >= 0; i--)
        {
            count = 0;
            for (int j = i * 8; j < (i + 1) * 8; j++)
            {
                bool piece_found = false;
                for (int k = 0; k < 12 && !piece_found; k++)
                {
                    if (is_occupied(boards_[k], j))
                    {
                        if (count != 0)
                        {
                            ret += std::to_string(count);
                            count = 0;
                        }
                        ret += pieces_chars[k];
                        piece_found = true;
                    }
                }
                if (!piece_found)
                {
                    count++;
                }
            }
            if (count != 0)
            {
                ret += std::to_string(count);
                count = 0;
            }
            if (i != 0)
                ret += '/';
        }
        if (playing_color_ == Color::WHITE)
            ret += " w ";
        else
            ret += " b ";
        if (castling_K_)
            ret += "K";
        if (castling_Q_)
            ret += "Q";
        if (castling_k_)
            ret += "k";
        if (castling_q_)
            ret += "q";

        if (!castling_K_ && !castling_Q_ && !castling_k_ && !castling_q_)
            ret += "-";

        ret += " - 1 0 1";
        return ret;
    }

    std::string Chessgame::print_color_boards()
    {
        std::string ret_white = "";
        std::string ret_black = "";
        for (int i = 7; i >= 0; i--)
        {
            for (int j = i * 8; j < (i + 1) * 8; j++)
            {
                if (is_occupied(white_board_, j))
                {
                    ret_white += "W ";
                }
                else
                {
                    ret_white += ". ";
                }
                if (is_occupied(black_board_, j))
                {
                    ret_black += "B ";
                }
                else
                {
                    ret_black += ". ";
                }
            }
            ret_white += '\n';
            ret_black += '\n';
        }
        return ret_white + '\n' + ret_black;
    }

    // Init functions
    // baseFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
    void Chessgame::init_basic_board()
    {
        std::string basic_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
        init_board_from_fen(basic_fen);
    }

    void Chessgame::init_board_from_fen(std::string fen_str)
    {
        int index = 56;
        clear_boards();
        for (char c : fen_str)
        {
            if (c >= '1' && c <= '9')
            {
                index += (int)c - '0';
                continue;
            }

            if (c == '/')
            {
                index -= 16;
                continue;
            }

            int board_index = index_from_char(c);

            modify_index(board_index, index);

            index++;
        }
    }

    void Chessgame::clear_boards()
    {
        for (int i = 0; i < 12; i++)
        {
            boards_[i] &= (uint64_t)0;
        }
    }

    void Chessgame::generate_color_boards()
    {
        white_board_ &= (uint64_t)0;
        black_board_ &= (uint64_t)0;
        for (int i = 0; i < 6; i++)
        {
            white_board_ |= boards_[i];
            black_board_ |= boards_[i + 6];
        }
    }

    // Getters (for boards)
    uint64_t Chessgame::get_white_board()
    {
        return white_board_;
    }

    uint64_t Chessgame::get_black_board()
    {
        return black_board_;
    }

    std::vector<uint64_t> Chessgame::get_boards()
    {
        return boards_;
    }

    // Move functions
    void Chessgame::execute_castling(int end)
    {
        if (end == 6 && castling_K_ == true)
        {
            clear_index(1, 7);
            modify_index(1, 5);
        }

        else if (end == 2 && castling_Q_ == true)
        {
            clear_index(1, 0);
            modify_index(1, 3);
        }

        else if (end == 62 && castling_k_ == true)
        {
            clear_index(7, 63);
            modify_index(7, 61);
        }

        else if (end == 58 && castling_q_ == true)
        {
            clear_index(7, 56);
            modify_index(7, 59);
        }
    }

    void Chessgame::execute_en_passant(int start, int end)
    {
        bool is_white = playing_color_ == Color::WHITE;

        if (is_white && end == start + 16)
            en_passant_w_ = end - 8;
        else if (!is_white && end == start - 16)
            en_passant_b_ = end + 8;

        if (end == en_passant_b_ || end == en_passant_w_)
        {
            int eat_index = en_passant_b_ - 8;
            if (!is_white)
                eat_index = en_passant_w_ + 8;
            int index_min = is_white ? 6 : 0;
            for (int i = 0; i < 6; i++)
            {
                clear_index(i + index_min, eat_index);
            }
        }
    }

    void Chessgame::execute_move(Move move)
    {
        char piece =
            piece_to_char(move.piece_get(), playing_color_ == Color::BLACK);
        PieceType move_piece = move.piece_get();

        int start = move.start_get();
        int end = move.end_get();

        clear_index(index_from_char(piece), start);

        if (move.promotion_get().has_value())
        {
            move_piece = move.promotion_get().value();
            piece = piece_to_char(move_piece, playing_color_ == Color::BLACK);
        }

        modify_index(index_from_char(piece), end);
        // ====== EN PASSANT ====== //
        if (playing_color_ == Color::WHITE)
            en_passant_w_ = -1;
        else
            en_passant_b_ = -1;
        if (move_piece == PieceType::PAWN)
            execute_en_passant(start, end);

        // ====== CASTLING ====== //
        if (move_piece == PieceType::KING
            && ((start == 4 && (end == 6 || end == 2))
                || (start == 60 && (end == 62 || end == 58))))
            execute_castling(end);

        if (move_piece == PieceType::KING)
        {
            if (start == 4)
            {
                castling_K_ = false;
                castling_Q_ = false;
            }
            else if (start == 60)
            {
                castling_k_ = false;
                castling_q_ = false;
            }
        }
        if (move_piece == PieceType::ROOK)
        {
            if (start == 0 && castling_Q_)
                castling_Q_ = false;
            if (start == 7 && castling_K_)
                castling_K_ = false;
            if (start == 56 && castling_q_)
                castling_q_ = false;
            if (start == 63 && castling_k_)
                castling_k_ = false;
        }

        if (move.capture_get())
        {
            int index_min = playing_color_ == Color::WHITE ? 6 : 0;
            for (int i = 0; i < 6; i++)
            {
                clear_index(i + index_min, end);
            }
        }
        playing_color_ =
            playing_color_ == Color::WHITE ? Color::BLACK : Color::WHITE;
        generate_color_boards();
    }
} // namespace board
