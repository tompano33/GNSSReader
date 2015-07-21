//
//  Connect4Game.h
//  ConnectGame
//
//  Created by Erik Buck on 6/5/13.
//

#ifndef __ConnectGame__Connect4Game__
#define __ConnectGame__Connect4Game__

/////////////////////////////////////////////////////////////////
/// This function displays an empty game board and then loops
/// accepting user input specifying the column in which a player
/// wishes to place a token until either the game board is full
/// or a player has won. See the AbstractBoard class description
/// in the AlphaBeta.h file for more information about game play,
/// winning, and tie games.
///
/// If the command line argument, 0, was provided by the user,
/// this plays the game computer vs. computer. If 1 is specified,
/// the user plays Xs and the computer plays Os. If two is
/// specified, the game waits for user input from both players.
///
int connect4Game(int argc, const char *argv[]);

#endif /* defined(__ConnectGame__Connect4Game__) */
