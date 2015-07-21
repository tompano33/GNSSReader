//
//  TicTacToeBoard.h
//  ConnectGame
//
//  Created by Erik Buck on 6/5/13.
//

#ifndef __ConnectGame__TicTacToeBoard__
#define __ConnectGame__TicTacToeBoard__

#include "AlphaBeta.h"

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/// This class encapsulates "Tic Tac Toe" game board and is a
/// concrete implementation of the AbstractBoard class. See
/// AbstractBoard.h for details about the virtual member
/// functions implemented by this class.
class TicTacToeBoard : public AbstractBoard
{
public:
   AbstractBoard::player mBoard[3][3];
   AbstractBoard::boards *mSuccessorBoardsPtr;

   ///////////////////////////////////////////////////////////
   TicTacToeBoard();
   TicTacToeBoard(const TicTacToeBoard &);

   ///////////////////////////////////////////////////////////
   virtual const AbstractBoard::boards &
      allLegalMovesForPlayer(AbstractBoard::player);

   ///////////////////////////////////////////////////////////
   virtual AbstractBoard::player winner();
 
   ///////////////////////////////////////////////////////////
   virtual int score();

   ///////////////////////////////////////////////////////////
   virtual std::string maximizerName() const
   {
      return std::string("X");
   }

   ///////////////////////////////////////////////////////////
   virtual std::string minimizerName() const
   {
      return std::string("O");
   }

   ///////////////////////////////////////////////////////////
   bool operator ==(const TicTacToeBoard &);

   ///////////////////////////////////////////////////////////
   operator std::string () const;

   ///////////////////////////////////////////////////////////
   bool placePlayerToken(AbstractBoard::player, int pos);
};

#endif /* defined(__ConnectGame__TicTacToeBoard__) */
