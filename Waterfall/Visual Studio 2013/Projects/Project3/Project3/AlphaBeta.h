//
//  AlphaBeta.h
//  ConnectGame
//
//  Created by Erik Buck on 6/5/13.
//

#ifndef __ConnectGame__AlphaBeta__
#define __ConnectGame__AlphaBeta__

#include <set>
#include <string>
#include <iostream> 
#include <climits>  // for INT_MAX


/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/// This abstract class declares the interface for concrete
/// subclasses encapsulating the logic/rules for two player
/// board games. One player attempts to maximize the "score"
/// while the other tries to minimize the "score" until one
/// player "wins" or the game is a tie. This class provides
/// an implementation of the classic alpha-beta search algorithm
/// to select moves for one or both players.
/// http://en.wikipedia.org/wiki/Alpha-beta_pruning
///
/// Concrete subclasses of this class must implement the pure
/// virtual member functions declared here to benefit from the
/// alpha-beta function. Calculation of a board's score,
/// determination of wins, determination of available next moves,
/// etc. are all left to concrete subclasses that encapsulate
/// specific games like Connect Four or Tic Tac Toe.
/// Instances of concrete subclasses should each store one board
/// configuration in the respective game. Even simple games may
/// have tens of millions of possible board configuration
/// combinations and permutations (producing tens of millions
/// of instances of concrete subclasses of this class).
/// Therefore, keep the memory size of concrete subclass
/// instances as small as reasonable.
class AbstractBoard
{
public:
   //////////////////////////////////////////////////////////////
   /// This type identifies the players. One player tries to
   /// maximize the score, and the other tries to minimize the
   /// score. A third value, None, indicates board positions that
   /// no player controls.
   /// These values are also returned from the winner() member
   /// function. If the maximizer has won, the winner() function
   /// returns Maximizer. If the minimizer has won, the function
   /// returns Minimizer. If neither player has won yet or the
   /// game has ended in a tie, the winner() function must return
   /// None.
   typedef enum
   {
      Maximizer = 1,
      None = 0,
      Minimizer = -1
   }
   player;
   
   //////////////////////////////////////////////////////////////
   /// One of the crucial functions of any board game is to
   /// identify the set of legal moves that may follow from the
   /// current board configuration. The set of available next
   /// moves is returned from the allLegalMovesForPlayer() member
   /// function using this type.
   typedef std::set<AbstractBoard *> boards;
   
   //////////////////////////////////////////////////////////////
   /// A virtual destructor is essential when a class has one or
   /// more virtual member functions. This implementation does
   /// nothing.
   virtual ~AbstractBoard()
   {
   }
   
///////////////////////////////////////////////////////////
/// BEGIN Pure virtual member functions to be implemented
/// by concrete subclasses
///////////////////////////////////////////////////////////

   //////////////////////////////////////////////////////////////
   /// Return the set of all boards that may result
   /// from the current board configuration when the specified
   /// player makes a move. In other words, after the specified
   /// player moves, the board will match one of the boards
   /// returned from this function.
   /// This function is called by the alpha-beta function, and
   /// the move selected by the alpha-beta function will produce
   /// one of the boards in the returned set. Implement this
   /// function based on the logic and rules for the game
   /// encapsulated by a concrete subclass of this class. For
   /// example, a tic-tac-toe game will return a set containing
   /// boards matching the boards that would result if the
   /// specified player played in any one of the positions
   /// remaining open on the current board.
   virtual const AbstractBoard::boards &
      allLegalMovesForPlayer(AbstractBoard::player) = 0;
   
   //////////////////////////////////////////////////////////////
   /// Implement this function based on the logic/rules of the
   /// game encapsulated by a concrete subclass.
   /// If the current board indicates that the maximizer has won,
   /// this function returns Maximizer. If the minimizer has won,
   /// this function returns Minimizer. If neither player has won
   /// yet or the game has ended in a tie, the winner() function
   /// returns None.
   virtual AbstractBoard::player winner() = 0;
   
   //////////////////////////////////////////////////////////////
   /// Return the "score" indicated by the current board
   /// configuration. Score must have meaning for the particular
   /// game encapsulated by a concrete subclass of this class.
   /// If the board favors the maximizer, the returned value
   /// should be positive. If the board favors the minimizer,
   /// the returned score should be negative. If the board favors
   /// neither player, the score should be zero.
   /// Important: The score returned must be less than MAX_INT
   /// and greater than -MAX_INT because the values, MAX_INT and
   /// -MAX_INT have special significance within the alpha-beta
   /// implementation. They represent "impossible" scores.
   virtual int score() = 0;

   //////////////////////////////////////////////////////////////
   /// Implement this function to return the human readable name
   /// of the maximizing player. For example, "White" for a chess
   /// game, Xs for a Tic-tac-toe game, etc.
   virtual std::string maximizerName() const = 0;

   //////////////////////////////////////////////////////////////
   /// Implement this function to return the human readable name
   /// of the minimizing player. For example, "Black" for a chess
   /// game, Os for a Tic-tac-toe game, etc. 
   virtual std::string minimizerName() const = 0;

   //////////////////////////////////////////////////////////////
   /// Return a string representation of the board. This function
   /// is primarily used for debugging purposes: to show the
   /// board configurations considered by the alpha-beta
   /// function.
   virtual operator std::string () const = 0;
   
///////////////////////////////////////////////////////////
/// END Pure virtual member functions to be implemented by
/// concrete subclasses
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
/// BEGIN virtual member functions to be optionally
/// overridden by concrete subclasses
///////////////////////////////////////////////////////////

   //////////////////////////////////////////////////////////////
   /// This function returns a reasonable default search depth
   /// for the alpha-beta function to use with most board games.
   /// The number of moves and therefore board combinations and
   /// permutations grows exponentially as the number returned
   /// from this function increases.
   virtual int evaluationDepth() const
   {  // Return a reasonable default for board games
      return 9; // Abitrary: balances performance with smarts
   }
   
   //////////////////////////////////////////////////////////////
   /// Return the score that indicates the Maximizer has won.
   /// This function returns a reasonable default value. The
   /// score() member function must be implemented to return this
   /// value if/when the maximizer wins.
   virtual int scoreForMaximizerWin() const
   {  // Return a reasonable default for board games
      // Must be greater than 0 and less than INT_MAX
      return INT_MAX - 1;
   }
   
   //////////////////////////////////////////////////////////////
   /// Return the score that indicates the Minimizer has won.
   /// This function returns a reasonable default value. The
   /// score() member function must be implemented to return this
   /// value if/when the minimizer wins.
   virtual int scoreForMinimizerWin() const
   {  // Return a reasonable default for board games
      // Must be less than zero and greater than -INT_MAX
      return -INT_MAX + 1;
   }

///////////////////////////////////////////////////////////
/// END virtual member functions to be optionally
/// overridden by concrete subclasses
///////////////////////////////////////////////////////////
   
   //////////////////////////////////////////////////////////////
   /// Call this function to let the alpha-beta function select
   /// the best move for the specified player. This function
   /// returns the score after the move. A pointer to the board
   /// configuration resulting from the move is returned by
   /// reference in resultingBoard. The returned pointer will
   /// point to one of the boards in set returned from the
   /// allLegalMovesForPlayer() member function.
   int makeAutomaticPlayerMove(
      AbstractBoard::player player,
      AbstractBoard *&resultingBoard);
   
   //////////////////////////////////////////////////////////////
   /// This function outputs the string representation of the
   /// board to the specified stream.
   void print(std::ostream &ostream)
   {
      std::string output(*this);
      ostream << output;
   }
   
private:

   //////////////////////////////////////////////////////////////
   /// This function implements the alpha-beta pruning algorithm
   /// to build and evaluate a tree of possible board
   /// configurations resulting from player moves as players
   /// alternate turns. This is an optimization of the minimax
   /// tree searching algorithm:
   /// Wikipedia: "A minimax algorithm is a recursive
   /// algorithm for choosing the next move in an n-player game,
   /// usually a two-player game. A value is associated with each
   /// position or state of the game. This value is computed by
   /// means of a position evaluation function and it indicates
   /// how good it would be for a player to reach that position.
   /// The player then makes the move that maximizes the minimum
   /// value of the position resulting from the opponent's
   /// possible following moves. If it is A's turn to move, A
   /// gives a value to each of his legal moves.
   /// A possible allocation method consists in assigning a
   /// certain win for A as +1 and for B as −1. This leads to
   /// combinatorial game theory as developed by John Horton
   /// Conway. An alternative is using a rule that if the result
   /// of a move is an immediate win for A it is assigned
   /// positive infinity and, if it is an immediate win for B,
   /// negative infinity. The value to A of any other move is the
   /// minimum of the values resulting from each of B's possible
   /// replies. For this reason, A is called the maximizing
   /// player and B is called the minimizing player, hence the
   /// name minimax algorithm. The above algorithm will assign a
   /// value of positive or negative infinity to any position
   /// since the value of every position will be the value of
   /// some final winning or losing position. Often this is
   /// generally only possible at the very end of complicated
   /// games such as chess or Go, since it is not computationally
   /// feasible to look ahead as far as the completion of the
   /// game, except towards the end, and instead positions are
   /// given finite values as estimates of the degree of belief
   /// that they will lead to a win for one player or another."
   int alphaBeta(
      AbstractBoard::player,
      AbstractBoard *&nextBoardPtr,
      int depth = 0,
      int alpha = -INT_MAX, // an impossible negative score
      int beta = INT_MAX);  // an impossible positive score
};

#endif /* defined(__ConnectGame__AlphaBeta__) */
