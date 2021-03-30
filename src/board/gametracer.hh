#pragma once

#include <dlfcn.h>
#include <vector>

#include "../listeners/listener.hh"
#include "chessgame.hh"

namespace board
{
    class GameTracer
    {
    public:
        GameTracer(std::vector<std::string> paths);
        void set_game(Chessgame &game);
        void play_pgn_game(std::string pgn_path);
        ReportType check_report(Chessgame game, Move move);
        void execute_listener_move(PgnMove move);

        Chessgame game_;
        std::vector<void *> handles_;
        std::vector<listener::Listener *> listeners_;
    };
} // namespace board
