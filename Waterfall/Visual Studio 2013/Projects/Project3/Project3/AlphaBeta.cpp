//
//  AlphaBeta.cpp
//  ConnectGame
//
//  Created by Erik Buck on 6/5/13.
//

/*
		CS3100 Data Structures
		Project 3
		by Thomas Platt

	This program dynamically creates a "game tree" as the alphaBeta function
	is called recursively. For each function call, it iterates through
	all possible moves and for each move, it calls the function recursively.
	The result is a determination of the maximum score for the Maximizer and
	a minimum score for the Minimizer. 

	The program uses the alpha beta pruning algorithm from the Wikipedia page.

*/

#include "AlphaBeta.h"
#include <cassert>
#include <algorithm>
#include <iostream>

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/// See comments in header.
int AbstractBoard::makeAutomaticPlayerMove(
	AbstractBoard::player player,
	AbstractBoard *&resultingBoard)
{
	return this->alphaBeta(player, resultingBoard, 4);
}


/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/// See comments in header.
int AbstractBoard::alphaBeta(
	AbstractBoard::player player,
	AbstractBoard *&nextBoardPtr,
	int depth,
	int alpha,
	int beta)
{
	int score = this->score();
	nextBoardPtr = NULL;

	boards availableMoves =
		allLegalMovesForPlayer(player);


	if (AbstractBoard::None != winner())
	{  // winner found
		return score;
	}
	else if (evaluationDepth() <= depth ||
		0 == availableMoves.size())
	{  // search limit reached or no legal moves
		return score;
	}
	else
	{
		/*
			Fill the "game tree" children for this depth level.
		*/
		AbstractBoard** childrenNodes = (AbstractBoard**)malloc(sizeof(AbstractBoard)*availableMoves.size());
		int index = 0;
		int numChildren = availableMoves.size();

		for(boards::iterator it(
			availableMoves.begin()); it != availableMoves.end(); ++it)
		{ 
			nextBoardPtr = (*it);
			childrenNodes[++index] = nextBoardPtr;
		}
		printf("Depth: %i\n", depth);

		/*
		If depth is 0, return the score.
		*/
		if (depth == 0)
		{
			return score;
		}


		/*
			 If the player is the maximizer, then find the best (highest) score.
			 */
		if (player == Maximizer)
		{

			score = scoreForMinimizerWin();   // V = -infinity
			for (int i = 0; i < numChildren; i++)
			{
				nextBoardPtr = childrenNodes[i];
				score = std::max(score, alphaBeta(Minimizer, nextBoardPtr, depth - 1, alpha, beta));
				alpha = std::max(alpha, score);
				if (beta <= alpha)
				{
					break; /* Beta cut-off*/
				}
			}
			return score;
		}
		else
		{	/*
				 If the player is the minimizer, then find the "worst" (lowest) score.
				 */

			score = scoreForMaximizerWin();
			for (int j= 0; j < numChildren; j++)  /* for each child of node*/
			{
				nextBoardPtr = childrenNodes[j];
				score = std::min(score, alphaBeta(Maximizer, nextBoardPtr, depth - 1, alpha, beta));
				beta = std::min(beta, score);
				if (beta <= alpha)
				{
					break; /* Alpha cut-off*/
				}
			}
			return score;
		}

	}
}