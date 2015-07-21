//
//  ConnectBoard.cpp
//  ConnectGame
//
//  Created by Erik Buck on 6/5/13.
//

#include "ConnectBoard.h"
#include <cstdlib>
#include <sstream>
#include <cassert>
#include <cstring>
#include <algorithm>

size_t ConnectBoard::maxInstanceCount = 0;
size_t ConnectBoard::currentInstanceCount = 0;
size_t ConnectBoard::reusedCount =0;

//////////////////////////////////////////////////////////////
/// This constructor initializes the board instance to contain
/// Board::Open at every position.
ConnectBoard::ConnectBoard() :
   mScore(0),
   mIsScored(false)
{  // Make every position open
   mSuccessorBoardsPtr = new AbstractBoard::boards;
   
   for(int i = 0; i < ConnectBoard::width; i++)
   {
      for(int j = 0; j < ConnectBoard::height; j++)
      {
         this->setOccupantAtPosition(
            AbstractBoard::None, 
            positionCoordinate(i, j));
      }
   }
   
   currentInstanceCount++;
   maxInstanceCount = std::max(
      currentInstanceCount, maxInstanceCount);
}

//////////////////////////////////////////////////////////////
/// This constructor initializes the board as a copy of
/// original. The newly initialized board contains tokens
/// in every position to match the tokens in original.
ConnectBoard::ConnectBoard(
      const ConnectBoard &original) :
   mScore(0),
   mIsScored(false)
{  // copy every position from original
   mSuccessorBoardsPtr = new AbstractBoard::boards;
   
   memcpy(mPositions, original.mPositions, sizeof(mPositions));
   
   currentInstanceCount++;
   maxInstanceCount = std::max(
      currentInstanceCount, maxInstanceCount);
}

//////////////////////////////////////////////////////////////
ConnectBoard::~ConnectBoard()
{
   AbstractBoard::boards::iterator it(
      mSuccessorBoardsPtr->begin());
   AbstractBoard::boards::iterator end(
      mSuccessorBoardsPtr->end());
   
   while(it != end)
   {
      delete (*it);
      ++it;
   }
   
   delete mSuccessorBoardsPtr;
   mSuccessorBoardsPtr = NULL;
   currentInstanceCount--;
}


//////////////////////////////////////////////////////////////
bool ConnectBoard::operator ==(const ConnectBoard &other)
{
   for(int i = 0; i < ConnectBoard::width; i++)
   {
      for(int j = 0; j < ConnectBoard::height; j++)
      {
         ConnectBoard::positionCoordinate position(i, j);
         
         if(this->occupantAtPosition(position) !=
            other.occupantAtPosition(position))
         {
            return false;
         }
      }
   }
   
   return true;
}


//////////////////////////////////////////////////////////////
const AbstractBoard::boards &
ConnectBoard::allLegalMovesForPlayer(
   AbstractBoard::player player)
{
   if(0 == mSuccessorBoardsPtr->size())
   {
      for(int i = 0; i < ConnectBoard::width; i++)
      {
         if(AbstractBoard::None ==
            mPositions[i][ConnectBoard::height - 1])
         {  // At least one position is available in column
            ConnectBoard *newBoard = new ConnectBoard(*this);
            bool status = newBoard->dropTokenInColumn(player, i);
            assert(status);
            mSuccessorBoardsPtr->insert(newBoard);
         }
      }
   }
   else
   {
      reusedCount++;
   }
   
   return *mSuccessorBoardsPtr;
}

//////////////////////////////////////////////////////////////
/// This function returns a calculated score based on the
/// specified pattern, aPattern, and a starting position
/// within the board. The returned score is less than or equal
/// to Board::winningRunLength if the Black player's tokens
/// match aPattern. The returned score is greater than or
/// equal to -Board::winningRunLength if the Red player's
/// tokens match aPattern. Call this function for each
/// possible x and y position and each possible winning
/// pattern to assure detection of a win by either player.
int ConnectBoard::patternScoreAt(
   const ConnectBoard::winPattern aPattern, 
   int x, 
   int y) const
{
   int result = 0;
   for(int i = 0; i < ConnectBoard::winningRunLength; i++)
   {
      positionCoordinate position(
         x + aPattern[i].x, 
         y + aPattern[i].y);
         
      result += this->occupantAtPosition(position);
   }
   
   return result;
}

//////////////////////////////////////////////////////////////
/// This function returns an identifier for the player who
/// has won the game. If no player has won, this function
/// returns AbstractBoard::None.
AbstractBoard::player ConnectBoard::winner()

{
   AbstractBoard::player result = AbstractBoard::None;
   
   int currentScore = this->score();
   
   if(currentScore == scoreForMaximizerWin())
   {
      result = AbstractBoard::Maximizer;
   }
   else if(currentScore == scoreForMinimizerWin())
   {
      result = AbstractBoard::Minimizer;
   }
    
   return result;
}


//////////////////////////////////////////////////////////////
int ConnectBoard::score()
{
   if(!mIsScored)
   {
      {  // Pattern 0 is vertical win
         const int applicableHeight = ConnectBoard::height -
            (ConnectBoard::winningRunLength - 1);
         
         for(int i = 0; i < ConnectBoard::width; i++)
         {
            for(int j = 0; j < applicableHeight; j++)
            {
               int candidateScore = this->patternScoreAt(
                  ConnectBoard::winPatterns[0], i, j);
                                    
               if(abs(candidateScore) > abs(mScore))
               {
                  mScore = candidateScore;
                  
                  if(mScore >= ConnectBoard::winningRunLength)
                  {  // maximizer won
                     mIsScored = true;
                     mScore = scoreForMaximizerWin();
                     return mScore;  // bail out of nested loops
                  }
                  else if(mScore <=
                     -ConnectBoard::winningRunLength)
                  {  // minimizer won
                     mIsScored = true;
                     mScore = scoreForMinimizerWin();
                     return mScore;  // bail out of nested loops
                  }
               }
            }
         }
      }
      {  // Pattern 1 is horizontal win
         const int applicableWidth = ConnectBoard::width -
            (ConnectBoard::winningRunLength - 1);
         
         for(int i = 0; i < applicableWidth; i++)
         {
            for(int j = 0; j < ConnectBoard::height; j++)
            {
               int candidateScore = this->patternScoreAt(
                  ConnectBoard::winPatterns[1], i, j);
                                    
               if(abs(candidateScore) > abs(mScore))
               {
                  mScore = candidateScore;
                  
                  if(mScore >= ConnectBoard::winningRunLength)
                  {  // maximizer won
                     mIsScored = true;
                     mScore = scoreForMaximizerWin();
                     return mScore;  // bail out of nested loops
                  }
                  else if(mScore <=
                     -ConnectBoard::winningRunLength)
                  {  // minimizer won
                     mIsScored = true;
                     mScore = -INT_MAX + 1;
                     return mScore;  // bail out of nested loops
                  }
               }
            }
         }
      }
      {  // Pattern 2 is diagonal win
         const int applicableWidth = ConnectBoard::width -
            (ConnectBoard::winningRunLength - 1);
         const int applicableHeight = ConnectBoard::height -
            (ConnectBoard::winningRunLength - 1);
         
         for(int i = 0; i < applicableWidth; i++)
         {
            for(int j = 0; j < applicableHeight; j++)
            {
               int candidateScore = this->patternScoreAt(
                  ConnectBoard::winPatterns[2], i, j);
                                    
               if(abs(candidateScore) > abs(mScore))
               {
                  mScore = candidateScore;
                  
                  if(mScore >= ConnectBoard::winningRunLength)
                  {  // maximizer won
                     mIsScored = true;
                     mScore = scoreForMaximizerWin();
                     return mScore;  // bail out of nested loops
                  }
                  else if(mScore <=
                     -ConnectBoard::winningRunLength)
                  {  // minimizer won
                     mIsScored = true;
                     mScore = -INT_MAX + 1;
                     return mScore;  // bail out of nested loops
                  }
               }
            }
         }
      }
      {  // Pattern 3 is diagonal win
         const int applicableWidth = ConnectBoard::width -
            (ConnectBoard::winningRunLength - 1);
         const int applicableHeight = ConnectBoard::height -
            (ConnectBoard::winningRunLength - 1);
         
         for(int i = 0; i < applicableWidth; i++)
         {
            for(int j = 0; j < applicableHeight; j++)
            {
               int candidateScore = this->patternScoreAt(
                  ConnectBoard::winPatterns[3], i, j);
                                    
               if(abs(candidateScore) > abs(mScore))
               {
                  mScore = candidateScore;
                  
                  if(mScore >= ConnectBoard::winningRunLength)
                  {  // maximizer won
                     mIsScored = true;
                     mScore = scoreForMaximizerWin();
                     return mScore;  // bail out of nested loops
                  }
                  else if(mScore <=
                     -ConnectBoard::winningRunLength)
                  {  // minimizer won
                     mIsScored = true;
                     mScore = -INT_MAX + 1;
                     return mScore;  // bail out of nested loops
                  }
               }
            }
         }
      }
      
      mIsScored = true;
   }
   
   return mScore;
}

//////////////////////////////////////////////////////////////
/// This function returns true if the drop succeeded: a token
/// has been added to the board in the specified column. This
/// function returns false if the specified column is already
/// full or doesn't exist. Columns are identified by integers
/// from 0 on the left to (Board::width - 1) for the right
/// most column.
bool ConnectBoard::dropTokenInColumn(
   AbstractBoard::player player,
   int column)
{
   bool result = false;
   
   for(int j = 0; !result && j < ConnectBoard::height; j++)
   {
      positionCoordinate position(column, j);
      
      if(AbstractBoard::None ==
         this->occupantAtPosition(position))
      {  // Attempt to set the token in the lowest open
         /// position within the specified column,
         this->setOccupantAtPosition(
            player,
            position);
         
         // Verify the block was added to the board
         result = (player == this->occupantAtPosition(
            position));
      }
   }
   
   return result;
};

//////////////////////////////////////////////////////////////
/// This function sets the player token at the specified
/// position within the board. If the specified position is
/// not within the board, this function does not modify the
/// board.
void ConnectBoard::setOccupantAtPosition(
   AbstractBoard::player player, 
   ConnectBoard::positionCoordinate position)
{
   if(position.x >= 0 && 
      position.x < ConnectBoard::width &&
      position.y >= 0 &&
      position.y < ConnectBoard::height)
   {  // Position is within the board
      mPositions[position.x][position.y] = player;
   }
}


//////////////////////////////////////////////////////////////
ConnectBoard::operator std::string () const
{
   std::stringstream sstream;
   sstream << std::endl;
   
   // Print column labels
   for(int i = 0; i < ConnectBoard::width; i++)
   {
      char label = 'A' + i;
      sstream << "  " << label << ' ';
   }
   sstream << "\n";
   
   // Print play area
   for(int j = ConnectBoard::height-1; j >= 0; j--)
   {
      for(int i = 0; i < ConnectBoard::width; i++)
      {
         sstream << "----";
      }
      sstream << "-\n";
      for(int i = 0; i < ConnectBoard::width; i++)
      {
         ConnectBoard::positionCoordinate position(i, j);
         
         if(AbstractBoard::None ==
            this->occupantAtPosition(position))
         {
             sstream << "|   ";
         }
         else if(AbstractBoard::Minimizer ==
            this->occupantAtPosition(position))
         {
             sstream << "| O ";
         }
         else
         {
             sstream << "| X ";
         }
      }
      sstream << "|\n";
   }
   
   // Print bottom edge of play area
   for(int i = 0; i < ConnectBoard::width; i++)
   {
      sstream << "----";
   }
   sstream << "-\n";

   // Report number of boards currently considered
   // out of total boards ever considered
   sstream << ConnectBoard::currentInstanceCount <<
      " / " <<
      ConnectBoard::maxInstanceCount <<
      " (" << ConnectBoard::reusedCount << ")" <<
      std::endl;
   
   return sstream.str();
}

/////////////////////////////////////////////////////////////////
/// This array stores all the valid winning patterns for player
/// tokens.
const ConnectBoard::winPattern ConnectBoard::winPatterns[
   ConnectBoard::winningRunLength] =
{
   {  // Vertical pattern
      positionCoordinate(0,0),
      positionCoordinate(0,1),
      positionCoordinate(0,2),
      positionCoordinate(0,3),
   },
   {  // Horizontal pattern
      positionCoordinate(0,0),
      positionCoordinate(1,0),
      positionCoordinate(2,0),
      positionCoordinate(3,0),
   },
   {  // Diagonal pattern A
      positionCoordinate(0,0),
      positionCoordinate(1,1),
      positionCoordinate(2,2),
      positionCoordinate(3,3),
   },
   {  // Diagonal pattern B
      positionCoordinate(0,3),
      positionCoordinate(1,2),
      positionCoordinate(2,1),
      positionCoordinate(3,0),
   },
};
