#include <boost/algorithm/string.hpp>
#include <boost/program_options.hpp>
#include <fstream>
#include <iostream>
#include <string>

#include "../tests/units_tests/chessgame_tests.hh"
#include "board/chessgame.hh"
#include "board/gametracer.hh"
#include "ia/ia.hh"
#include "parsing/uci.hh"

int perft_run(int const &depth, board::Chessgame &game)
{
    if (depth == 0)
        return 0;

    std::vector<board::Move> moves_list;
    board::Player player = game.playing_color_ == Color::WHITE
        ? game.white_player_
        : game.black_player_;

    moves_list = player.generate_player_moves(game);

    if (depth == 1)
    {
        return moves_list.size();
    }

    int count = 0;
    board::Chessgame tmp_game = game;
    for (auto const &move : moves_list)
    {
        if (move.capture_get())
        {
            int index = player.is_white_ ? 11 : 5;
            if (game.is_occupied(game.get_boards()[index], move.end_get()))
                continue;
        }
        tmp_game.execute_move(move);
        count += perft_run(depth - 1, tmp_game);
        tmp_game = game;
    }
    return count;
}

int perft_main(const std::string &filepath)
{
    std::ifstream fs;
    fs.open(filepath, std::ios::in);
    if (!fs)
    {
        std::cerr << "Unable to open file";
        fs.close();
        return 1;
    }
    std::string line;
    std::string file;
    while (std::getline(fs, line))
    {
        file.append(line);
    }
    std::vector<std::string> perft_map;

    // 0: board 1: player 2: castling 3: en_passant 4: halfmove 5: fullmove 6:
    // depth
    boost::split(perft_map, file, boost::is_any_of(" "));
    if (perft_map.size() != 7)
    {
        fs.close();
        return 2;
    }
    // start game
    board::Chessgame game = Chessgame(file);

    // count moves here
    int depth = std::stoi(perft_map[perft_map.size() - 1]);
    return perft_run(depth, game);
}

int main(int argc, char **argv)
{
    try
    {
        namespace po = boost::program_options;

        // Declare the supported options.
        po::options_description desc("Allowed options");
        desc.add_options()("help,h", "show usage")(
            "pgn", po::value<std::string>(), "path to the PGN game file")(
            "listeners,l", po::value<std::vector<std::string>>()->multitoken(),
            "list of paths to listener plugins")(
            "perft", po::value<std::string>(),
            "path to a perft file")("test,t", "run units tests");

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);

        try
        {
            if (vm.count("help"))
            {
                std::cout << desc << "\n";
                return 0; // Success return
            }

            if (vm.count("pgn"))
            {
                std::vector<std::string> tmp_vec;
                if (vm.count("listeners"))
                    tmp_vec = vm["listeners"].as<std::vector<std::string>>();
                GameTracer gametracer = GameTracer(tmp_vec);
                gametracer.play_pgn_game(vm["pgn"].as<std::string>());
                for (auto l : gametracer.listeners_)
                    delete l;
                for (auto h : gametracer.handles_)
                    dlclose(h);
                return 0; // Success return
            }

            if (vm.count("perft"))
            {
                std::cout << perft_main(vm["perft"].as<std::string>())
                          << std::endl;
                return 0; // Success return
            }

            if (vm.count("test"))
            {
                return test_main(); // Success return
            }

            po::notify(vm);
        }
        catch (po::error &e)
        {
            std::cerr << "Error: " << e.what() << "\n" << std::endl;
            std::cerr << desc << std::endl;
            return 2; // Unhandled execption return
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Unhandled Exception in : main: " << e.what() << std::endl;
        return 2; // Unhandled execption return
    }

    ai::ai_loop();

    return 0;
}