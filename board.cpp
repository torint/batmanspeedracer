#include "board.h"
#define CORNER (4)
#define ADJC (-3)
#define EDGE (2)
#define ADJE (-2)
/*
 * Make a standard 8x8 othello board and initialize it to the standard setup.
 */
Board::Board() {
    taken.set(3 + 8 * 3);
    taken.set(3 + 8 * 4);
    taken.set(4 + 8 * 3);
    taken.set(4 + 8 * 4);
    black.set(4 + 8 * 3);
    black.set(3 + 8 * 4);
}

/*
 * Destructor for the board.
 */
Board::~Board() {
}

/*
 * Returns a copy of this board.
 */
Board *Board::copy() {
    Board *newBoard = new Board();
    newBoard->black = black;
    newBoard->taken = taken;
    return newBoard;
}

bool Board::occupied(int x, int y) {
    return taken[x + 8*y];
}

/*
 * True if occupied by side.
 */
bool Board::get(Side side, int x, int y) {
    return occupied(x, y) && (black[x + 8*y] == (side == BLACK));
}

void Board::set(Side side, int x, int y) {
    taken.set(x + 8*y);
    black.set(x + 8*y, side == BLACK);
}

bool Board::onBoard(int x, int y) {
    return(0 <= x && x < 8 && 0 <= y && y < 8);
}

 
/*
 * Returns true if the game is finished; false otherwise. The game is finished 
 * if neither side has a legal move.
 */
bool Board::isDone() {
    return !(hasMoves(BLACK) || hasMoves(WHITE));
}

/*
 * Returns true if there are legal moves for the given side.
 */
bool Board::hasMoves(Side side) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Move move(i, j);
            if (checkMove(&move, side)) return true;
        }
    }
    return false;
}

/*
 * Returns true if a move is legal for the given side; false otherwise.
 */
bool Board::checkMove(Move *m, Side side) {
    // Passing is only legal if you have no moves.
    if (m == NULL) return !hasMoves(side);

    int X = m->getX();
    int Y = m->getY();

    // Make sure the square hasn't already been taken.
    if (occupied(X, Y)) return false;
    //std::cerr << "not occupied" << std::endl;

    Side other = (side == BLACK) ? WHITE : BLACK;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dy == 0 && dx == 0) continue;

            // Is there a capture in that direction?
            int x = X + dx;
            int y = Y + dy;
            if (onBoard(x, y) && get(other, x, y)) {
                do {
                    x += dx;
                    y += dy;
                } while (onBoard(x, y) && get(other, x, y));

                if (onBoard(x, y) && get(side, x, y)) return true;
            }
        }
    }
    //std::cerr << "no capture" << std::endl;
    return false;
}

/*
 * Modifies the board to reflect the specified move.
 */
void Board::doMove(Move *m, Side side) {
    // A NULL move means pass.
    if (m == NULL) return;

    // Ignore if move is invalid.
    if (!checkMove(m, side)) return;

    int X = m->getX();
    int Y = m->getY();
    Side other = (side == BLACK) ? WHITE : BLACK;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dy == 0 && dx == 0) continue;

            int x = X;
            int y = Y;
            do {
                x += dx;
                y += dy;
            } while (onBoard(x, y) && get(other, x, y));

            if (onBoard(x, y) && get(side, x, y)) {
                x = X;
                y = Y;
                x += dx;
                y += dy;
                while (onBoard(x, y) && get(other, x, y)) {
                    set(side, x, y);
                    x += dx;
                    y += dy;
                }
            }
        }
    }
    set(side, X, Y);
}

/*
 * Current count of given side's stones.
 */
int Board::count(Side side) {
    return (side == BLACK) ? countBlack() : countWhite();
}

int Board::score(Move *m, Side side)
{
	Board *check;
	check = copy();
	check->doMove(m, side);
	int base_score = (side == BLACK) ? (check->countBlack() - check->countWhite()) : (check->countWhite() - check->countBlack());
	for(int i = 0; i < 8; i++)
	{
		for(int j = 0; j < 8; j++)
		{
			// corners
			if( ((i == 0) || (i == 7)) && ((j == 0) || (j == 7)) )
			{
				if(check->occupied(i, j))
				{
					if(check->get(side, i, j))
					{
						base_score = base_score + CORNER;
					}
					else
					{
						base_score = base_score - CORNER;
					}
				}
			}
			// adjacent to corners
			else if( ((i == 0) || (i == 1) || (i == 6) || (i == 7)) && ((j == 0) || (j == 1) || (j == 6) || (j == 7)) )
			{
				if(check->occupied(i, j))
				{
					if(check->get(side, i, j))
					{
						base_score = base_score + ADJC;
					}
					else
					{
						base_score = base_score - ADJC;
					}
				}
			}
			// on the edges, but not corner or adjacent to corner
			else if( ((i == 0) || (i == 7)) || ((j == 0) || (j == 7)) )
			{
				if(check->occupied(i, j))
				{
					if(check->get(side, i, j))
					{
						base_score = base_score + EDGE;
					}
					else
					{
						base_score = base_score - EDGE;
					}
				}
			}	
			//adjacent to edges
			else if( ((i == 0) || (i == 1) || (i == 6) || (i == 7)) || ((j == 0) || (j == 1) || (j == 6) || (j == 7)) )
			{
				if(check->occupied(i, j))
				{
					if(check->get(side, i, j))
					{
						base_score = base_score + ADJE;
					}
					else
					{
						base_score = base_score - ADJE;
					}
				}
			}		
		}
	}
	delete check;
	return base_score;			
	
}
Move* Board::bestmove(Side side)
{
	 Move *temp = new Move(-1, -1);
	 Side other = (side == BLACK) ? WHITE : BLACK;
	 int bestX, bestY;
	 int highscore = -10000;
	 for (int i = 0; i < 8; i++)
	 {
		 for (int j = 0; j < 8; j++)
		 {
			 temp->setX(i);
			 temp->setY(j);
			 if (this->checkMove(temp, side)) 
			 {
				 Board *potential = this->copy();
				 potential->doMove(temp, side);
				 Move *temp2 = new Move(-1, -1);
				 int lowscore = 10000;
				 for (int k = 0; k < 8; k++)
				 {
					 for (int l = 0; l < 8; l++)
					 {
						 temp2->setX(k);
						 temp2->setY(l);
						 if (potential->checkMove(temp2, other))
						 {
							 int score = -(potential->score(temp2, other));
							 if(score < lowscore)
							 {
								lowscore = score;
							 }
					     } 
					 }
				 }
				 delete temp2;
				 delete potential;
				 if (lowscore > highscore)
				 {
					 highscore = lowscore;
					 bestX = i;
					 bestY = j;
				 }
			 }
		 }
	 }
	 return new Move(bestX, bestY);
}	 

/*
 * Current count of black stones.
 */
int Board::countBlack() {
    return black.count();
}

/*
 * Current count of white stones.
 */
int Board::countWhite() {
    return taken.count() - black.count();
}

/*
 * Sets the board state given an 8x8 char array where 'w' indicates a white
 * piece and 'b' indicates a black piece. Mainly for testing purposes.
 */
void Board::setBoard(char data[]) {
    taken.reset();
    black.reset();
    for (int i = 0; i < 64; i++) {
        if (data[i] == 'b') {
            taken.set(i);
            black.set(i);
        } if (data[i] == 'w') {
            taken.set(i);
        }
    }
}
