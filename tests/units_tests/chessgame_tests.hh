#include "../../src/board/chessgame.hh"
#include "../../src/board/moves.hh"
#include "../../src/board/player.hh"
#include "../../src/ia/ia.hh"

using namespace board;

Position str_to_pos(std::string str);
bool test_move(Chessgame &game, std::string start, std::string end, char piece,
               bool capture);
int test_main(void);