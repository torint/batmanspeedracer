#include "player.h"

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish 
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;
    
    color = side;
    if (color == BLACK) {otherColor = WHITE;}
    else {otherColor = BLACK;}
    std::cerr << "colors initialized" << std::endl;
    ourboard = new Board();
    std::cerr << "board initialized" << std::endl;

    /* 
     * TODO: Do any initialization you need to do here (setting up the board,
     * precalculating things, etc.) However, remember that you will only have
     * 30 seconds.
     */
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
     //dragons are really 
     std::cerr << "in doMove" << std::endl;
     ourboard->doMove(opponentsMove, otherColor);
     //std::cerr << otherColor << ": (" << opponentsMove->getX() << ", " << opponentsMove->getY() << ")" << std::endl;
     std::cerr << "about to check if moves are available" << std::endl;
     if (ourboard->hasMoves(color))
	 {
		 std::cerr << "in for loop" << std::endl;
		 Move *temp = new Move(-1, -1);
		 for (int i = 0; i < 8; i++)
		 {
			 for (int j = 0; j < 8; j++)
			 {
				 temp->setX(i);
				 temp->setY(j);
				 std::cerr << "i:" << i << "j:" << j << ourboard->checkMove(temp, color) << std::endl;
				 //if (ourboard.checkMove(temp, color))
				 //{
				//	 ourboard.doMove(temp, color);
				//	 std::cerr << color << ": (" << temp->getX() << ", " << temp->getY() << ")" << std::endl;
				//	 return temp;
				 //}
			 }
			 
		 }
	 } 
     
     std::cerr << "FUCK" << std::endl;
     return NULL;
}
