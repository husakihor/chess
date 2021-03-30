#include "chessgame_tests.hh"

#include <fstream>
#include <iostream>

using namespace board;

Position str_to_pos(std::string str)
{
    File file = (File)(int)(str[0] - 'A');
    Rank rank = (Rank)(int)(str[1] - '1');

    Position pos = Position(file, rank);
    return pos;
}

using opt_piece_t = std::optional<PieceType>;
bool test_move(Chessgame &game, std::string start, std::string end, char piece,
               bool capture, opt_piece_t promote)
{
    Player player = game.playing_color_ == Color::WHITE ? game.white_player_
                                                        : game.black_player_;

    std::string player_color =
        game.playing_color_ == Color::WHITE ? "White" : "Black";
    std::cout << player_color << " player is playing...\n" << std::endl;

    std::cout << "Generating player moves\n";
    std::vector<Move> moves_player = player.generate_player_moves(game);
    std::cout << "Moves count : " << player.player_moves_.size() << std::endl;
    for (auto v : player.player_moves_)
    {
        std::cout << v << std::endl;
    }
    std::cout << "Players moves are generated\n";

    Position start_pos = str_to_pos(start);
    Position end_pos = str_to_pos(end);

    int start_index = start_pos.pos_to_index();
    int end_index = end_pos.pos_to_index();

    Move move =
        Move(start_index, end_index, char_to_piece(piece), capture, promote);

    bool played = false;
    for (auto m : moves_player)
    {
        if (m == move)
        {
            played = true;
            std::cout << "\nMOVE IS VALID \n";
            std::cout << move << std::endl;
            game.execute_move(move);
            std::cout << "Chess board after move: \n";
            std::cout << game.print_full_board() << std::endl;
        }
    }
    if (!played)
    {
        std::cout << "\nMOVE IS INVALID \n";
        std::cout << move << std::endl << std::endl;
        throw false;
    }
    return true;
}

int test_main(void)
{
    std::string test_fen = "rnbqk1nr/8/8/8/8/8/8/R1BQKBNR w KQkq - 1 0 1";
    Chessgame test_game =
        Chessgame(test_fen); //"8/6N1/3P2k1/8/8/8/8/8 b - - 1 0 1");
    std::cout << "STARTING BOARD: \n";
    std::cout << test_game.print_full_board() << std::endl;
    /*std::ofstream file;
    for (int i = 0; i < 10; i++)
    {
        std::string name = "./test_boards/board";
        name.append(std::to_string(i));

        file.open(name, std::fstream::out);

        file << test_game.fen_from_game();

        file.close();
    }
    std::cout << ai::eval_game_board(test_game) << std::endl;
    Move best = ai::minimax_root(test_game);
    std::cout << best << std::endl;

    test_move(test_game, "C1", "D2", 'B', false, std::nullopt);
    std::cout << "Board value : " << ai::eval_game_board(test_game)
              << std::endl;

    test_move(test_game, "D7", "D5", 'P', false, std::nullopt);
    std::cout << "Board value : " << ai::eval_game_board(test_game)
              << std::endl;

    test_move(test_game, "B1", "C3", 'N', false, std::nullopt);
    std::cout << "Board value : " << ai::eval_game_board(test_game)
              << std::endl;*/

    /*test_move(test_game, "A4", "B3", 'P', true, std::nullopt);*/

    return 1;
}
