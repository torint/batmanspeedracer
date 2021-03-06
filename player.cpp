#include "player.h"
#include <ctime>

using namespace std;
/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish 
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;
    ourboard = new Board();
    
    color = side;
    if (color == BLACK) {otherColor = WHITE;}
    else {otherColor = BLACK;}
    

    /* 
     * TODO: Do any initialization you need to do here (setting up the board,
     * precalculating things, etc.) However, remember that you will only have
     * 30 seconds.
     */
}

Player::Player(Side side, Board *b)
{
	testingMinimax = true;
    ourboard = b;
    
    color = side;
    if (color == BLACK) {otherColor = WHITE;}
    else {otherColor = BLACK;}
}

/*
 * Destructor for the player.
 */
Player::~Player() {
	delete ourboard;
}

/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be NULL.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return NULL.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft) {
    /* 
     * TODO: Implement how moves your AI should play here. You should first
     * process the opponent's opponents move before calculating your own move
     */ 
     
     clock_t begin = clock(), end;
     ourboard->doMove(opponentsMove, otherColor);
     if (ourboard->hasMoves(color))
     {
		 Move *move = ourboard->bestmove(color);
		 ourboard->doMove(move, color);
		 end = clock();
		 std::cerr << double(end - begin) / CLOCKS_PER_SEC << "sec" << std::endl;
		 return move;
	 }
	 end = clock();
	 std::cerr << double(end - begin) / CLOCKS_PER_SEC << "sec" << std::endl;
     return NULL;
}
