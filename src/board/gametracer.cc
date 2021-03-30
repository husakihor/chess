#include "gametracer.hh"

namespace board
{
    GameTracer::GameTracer(std::vector<std::string> paths)
    {
        game_ = Chessgame();
        for (auto path : paths)
        {
            auto handle = dlopen(path.c_str(), RTLD_NOW);
            if (!handle)
                std::cerr << "Failed to open : " << path << std::endl;
            else
            {
                auto symbol = dlsym(handle, "listener_create");
                if (!symbol)
                    std::cerr << "Failed to dlsym : " << path << std::endl;
                else
                {
                    listener::Listener *listener =
                        reinterpret_cast<listener::Listener *(*)()>(symbol)();
                    listeners_.push_back(listener);
                    handles_.push_back(handle);
                }
            }
        }
    }

    // Lance et joue une partie à partir d'un fichier pgn
    void GameTracer::play_pgn_game(std::string pgn)
    {
        std::vector<PgnMove> pgn_list = pgn_parser::parse_pgn(pgn);
        if (pgn_list.size() == 0)
            return;
        for (auto l : listeners_)
            l->on_game_started();
        std::vector<Move> move_list = Move::pgn_to_moves(pgn_list);
        int count = 1;

        Player current_player = !(bool)game_.playing_color_
            ? game_.white_player_
            : game_.black_player_;
        Player enemy_player = (bool)game_.playing_color_ ? game_.white_player_
                                                         : game_.black_player_;

        int move_index = 0;
        for (Move move : move_list)
        {
            std::vector<Move> available_moves =
                current_player.generate_player_moves(game_);
            bool has_played = false;
            for (auto m : available_moves)
            {
                if (m == move)
                {
                    if (check_report(game_, move)
                        != pgn_list[move_index].report_get())
                        break;
                    has_played = true;
                    execute_listener_move(pgn_list[move_index]);
                    game_.execute_move(move);
                    if (pgn_list[move_index].report_get()
                        == ReportType::CHECKMATE)
                    {
                        for (auto l : listeners_)
                        {
                            l->on_player_mat(game_.playing_color_);
                            l->on_game_finished();
                            return;
                        }
                    }
                    if (pgn_list[move_index].report_get() == ReportType::CHECK)
                    {
                        for (auto l : listeners_)
                            l->on_player_check(game_.playing_color_);
                    }
                    current_player = !(bool)game_.playing_color_
                        ? game_.white_player_
                        : game_.black_player_;
                    count += (!(bool)game_.playing_color_ ? 1 : 0);
                }
            }
            if (!has_played)
            {
                for (auto l : listeners_)
                {
                    if (available_moves.size() == 0)
                    {
                        l->on_player_pat(game_.playing_color_);
                        l->on_draw();
                    }
                    else
                    {
                        l->on_player_disqualified(!(bool)game_.playing_color_
                                                      ? Color::WHITE
                                                      : Color::BLACK);
                    }
                    l->on_game_finished();
                    return;
                }
            }
            move_index++;
        }
    }

    ReportType GameTracer::check_report(Chessgame game, Move move)
    {
        Chessgame tmp_game = game;
        tmp_game.execute_move(move);
        Player current = (bool)tmp_game.playing_color_ ? tmp_game.black_player_
                                                       : tmp_game.white_player_;
        if (current.is_in_check(tmp_game))
        {
            std::vector<Move> tmp_moves =
                current.generate_player_moves(tmp_game);
            if (tmp_moves.size() == 0)
                return ReportType::CHECKMATE;
            return ReportType::CHECK;
        }
        return ReportType::NONE;
    }

    void GameTracer::set_game(Chessgame &game)
    {
        game_ = game;
    }

    void GameTracer::execute_listener_move(PgnMove move)
    {
        Color color = game_.playing_color_;
        for (auto l : listeners_)
        {
            if (move.is_castling())
            {
                if (move.is_castling_queenside())
                    l->on_queenside_castling(color);
                else
                    l->on_kingside_castling(color);
            }
            else
            {
                l->on_piece_moved(move.piece_get(), move.start_get(),
                                  move.end_get());
                if (move.capture_get())
                    l->on_piece_taken(move.piece_get(), move.end_get());
                if (move.promotion_get())
                    l->on_piece_promoted(move.promotion_get().value(),
                                         move.end_get());
            }
        }
    }
} // namespace board
