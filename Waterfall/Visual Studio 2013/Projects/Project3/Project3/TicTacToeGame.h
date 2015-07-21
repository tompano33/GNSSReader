//
//  TicTacToeGame.h
//  ConnectGame
//
//  Created by Erik Buck on 6/5/13.
//

#ifndef __ConnectGame__TicTacToeGame__
#define __ConnectGame__TicTacToeGame__

/////////////////////////////////////////////////////////////////
/// This function displays an empty game board and then loops
/// accepting user input specifying the position in which a
/// player wishes to place an 'X' or an 'O' token until either
/// the game board is full or a player has won. See the
/// AbstractBoard class in the AlphaBeta.h file for more
/// class description for more information about game play,
/// winning, and tie games.
/// If the command line argument, 0, was provided by the user,
/// this plays the game computer vs. computer. If 1 is specified,
/// the user plays Xs and teh computer plays Os. If two is
/// specified, the game waits for user input from both players.
///
/// Users enter board positions by number using the following
/// key:
///      -------------
///      | 0 | 1 | 2 |
///      -------------
///      | 3 | 4 | 5 |
///      -------------
///      | 6 | 7 | 8 |
///      -------------
int ticTacToeGame(int argc, const char *argv[]);

#endif /* defined(__ConnectGame__TicTacToeGame__) */
