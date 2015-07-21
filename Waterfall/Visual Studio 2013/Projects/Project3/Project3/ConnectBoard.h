//
//  ConnectBoard.h
//  ConnectGame
//
//  Created by Erik Buck on 6/5/13.
//

#ifndef __ConnectGame__ConnectBoard__
#define __ConnectGame__ConnectBoard__

#include "AlphaBeta.h"

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/// This class encapsulates "Connect Four" game board.
/// http://en.wikipedia.org/wiki/Connect_Four There
/// are two players, Black and Red. The Black player drops
/// a black token in any unfilled column of the game board.
/// The token drops to the lowest unoccupied position in
/// the column. Then the Red player drops a red token in
/// in any unfilled column of the game board. Play proceeds
/// alternating turns until either all columns are filled
/// or one player has placed 4 or more of that player's
/// tokes in a line either horizontally, vertically,
/// or diagonally. If all board columns are filled without
/// any player placing 4 tokens in a line, the game is
/// a tie. Otherwise, the first player to place 4 tokens
/// in a line wins.
///
/// Wikipedia: "The game was solved mathematically by James D.
/// Allen (October 1, 1988), and independently by Victor Allis
/// (October 16, 1988). With perfect play, the first player
/// can force a win by starting in the middle column. By
/// starting in the two adjacent columns, the first player
/// allows the second player to reach a draw; by starting with
/// the four outer columns, the first player allows the second
/// player to force a win.
class ConnectBoard : public AbstractBoard
{
public:

   //////////////////////////////////////////////////////////////
   /// This is the width of the board in columns
   static const int width = 7;
   
   //////////////////////////////////////////////////////////////
   /// This is the number of positions in each column
   static const int height = 6;
   
   //////////////////////////////////////////////////////////////
   /// This is the number of tokens in a line needed to win
   static const int winningRunLength = 4;
   
   //////////////////////////////////////////////////////////////
   /// This is the number of winning token patterns: horizontal
   /// vertical, diagonal sloping left to right down, and
   /// diagonal sloping left to right up.
   static const int numberOfWinningPatterns = 4;
   
   //////////////////////////////////////////////////////////////
   /// This type stores the x and y coordinates of each position
   /// within the game board. The position {0, 0} is in the
   /// lower left corner of the board.
   struct positionCoordinate
   {
      int x;
      int y;
      
      ///////////////////////////////////////////////////////////
      /// This constructor initializes the x and y coordinates
      /// of a position.
      positionCoordinate(int anX, int aY) :
         x(anX),
         y(aY)
      {
      }
   };

   //////////////////////////////////////////////////////////////
   /// This constructor initializes the board instance to contain
   /// AbstractBoard::None at every position.
   ConnectBoard();

   //////////////////////////////////////////////////////////////
   /// This constructor initializes the board as a copy of
   /// original. The newly initialized board contains tokens
   /// in every position to match the tokens in original.
   ConnectBoard(const ConnectBoard&);

   //////////////////////////////////////////////////////////////
   virtual ~ConnectBoard();
   
   //////////////////////////////////////////////////////////////
   /// Return all board configurations that can succeed this
   /// board from a legal move by the specified player.
   virtual const AbstractBoard::boards &
      allLegalMovesForPlayer(AbstractBoard::player);
   
   //////////////////////////////////////////////////////////////
   /// If the current board indicates that the maximizer has won,
   /// this function returns Maximizer. If the minimizer has won,
   /// this function returns Minimizer. If neither player has won
   /// yet or the game has ended in a tie, the winner() function
   /// returns None.
   virtual AbstractBoard::player winner();
   
   //////////////////////////////////////////////////////////////
   /// Return the "score" indicated by the current board
   /// configuration. 
   /// If the board favors the Black player, the returned value
   /// is positive. If the board favors the Red player, the
   /// returned score is negative. If the board favors neither
   /// player, the score is zero.
   /// Important: The score returned must be less than MAX_INT
   /// and greater than -MAX_INT because the values, MAX_INT and
   /// -MAX_INT have special significance within the alpha-beta
   /// implementation. They represent "impossible" scores.
   virtual int score();

   //////////////////////////////////////////////////////////////
   virtual std::string maximizerName() const
   {
      return std::string("Black");
   }
   
   //////////////////////////////////////////////////////////////
   virtual std::string minimizerName() const
   {
      return std::string("Red");
   }
   
private:
   
   //////////////////////////////////////////////////////////////
   /// This type stores patterns of coordinates specifying
   /// winning token configuration. (four in a row horizontally,
   /// vertically, or diagonally)
   typedef ConnectBoard::positionCoordinate (winPattern)[
      ConnectBoard::winningRunLength];
   
   //////////////////////////////////////////////////////////////
   /// This constant array stores all possible winning
   /// token patterns.
   static const ConnectBoard::winPattern winPatterns[
      ConnectBoard::numberOfWinningPatterns];

   //////////////////////////////////////////////////////////////
   /// This array stores player tokens.  A special token,
   /// AbstractBoard::None occupies each position in the array
   /// that is not yet occupied by a player token.
   AbstractBoard::player
      mPositions[ConnectBoard::width][ConnectBoard::height];

   //////////////////////////////////////////////////////////////
   /// This variable caches the result of calling
   /// allLegalMovesForPlayer() as an optimization.
   AbstractBoard::boards *mSuccessorBoardsPtr;

   //////////////////////////////////////////////////////////////
   /// This variable caches the most recent score calculated by
   /// the score() function. This variable is an optimization
   /// that reduces the amount of time spent calculating scores.
   int mScore;

   //////////////////////////////////////////////////////////////
   /// This variable is true if the board has ever been scored
   /// and false otherwise.
   bool mIsScored;
   
public:

   //////////////////////////////////////////////////////////////
   /// This variable stores the cumulative number of instances
   /// ever created. It's used for statistical information and
   /// debugging purposes only.
   static size_t maxInstanceCount;

   //////////////////////////////////////////////////////////////
   /// This variable stores the current number of instances
   /// created. It's used for statistical information and
   /// debugging purposes only.
   static size_t currentInstanceCount;

   //////////////////////////////////////////////////////////////
   /// This variable stores a count of the number of instances
   /// reused by the alpha-beta algorithm. This is a count of
   /// times instance creation was avoided.
   static size_t reusedCount;

   //////////////////////////////////////////////////////////////
   /// Compares two instances for equality.
   bool operator ==(const ConnectBoard &);

   //////////////////////////////////////////////////////////////
   /// Returns a string representation of the board.
   operator std::string () const;
   
   //////////////////////////////////////////////////////////////
   /// This function returns true if the drop succeeded: a token
   /// has been added to the board in the specified column. This
   /// function returns false if the specified column is already
   /// full or doesn't exist. Columns are identified by integers
   /// from 0 on the left to (ConnectBoard::width - 1) for the
   /// right most column.
   bool dropTokenInColumn(
      AbstractBoard::player player,
      int column);
   
   //////////////////////////////////////////////////////////////
   /// This function returns the token at the specified
   /// position within the board. Board::Open is returned
   /// if the specified position is not in the board or no
   /// player token is stored in the specified position.
   AbstractBoard::player occupantAtPosition(
      ConnectBoard::positionCoordinate position) const
   {
      AbstractBoard::player result = AbstractBoard::None;
         
      if(position.x >= 0 && 
         position.x < ConnectBoard::width &&
         position.y >= 0 &&
         position.y < ConnectBoard::height)
      {  // Position is within the board
         result = mPositions[position.x][position.y];
      }
         
      return result;
   }
   
   //////////////////////////////////////////////////////////////
   /// This function sets the player token at the specified
   /// position within the board. If the specified position is
   /// not within the board, this function does not modify the
   /// board.
   void setOccupantAtPosition(
      AbstractBoard::player player, 
      ConnectBoard::positionCoordinate position);

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
   int patternScoreAt(
      const ConnectBoard::winPattern aPattern, 
      int x, 
      int y) const;  
};

#endif /* defined(__ConnectGame__ConnectBoard__) */
