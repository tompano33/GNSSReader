//
//  Connect4Game.cpp
//  ConnectGame
//
//  Created by Erik Buck on 6/5/13.
//

#include "Connect4Game.h"
#include "ConnectBoard.h"
#include <cassert>
#include <iostream>  // for ostream and cout
#include <limits>    // for numeric_limits, max()
#include <cstdlib>
#include <cstring>
#include <cctype>

/////////////////////////////////////////////////////////////////
static AbstractBoard *connect4MakeAutomaticMove(
   AbstractBoard::player currentPlayer,
   AbstractBoard *curentGameBoardPtr,
   std::string playerName,
   std::string tiePrediction,
   std::string maximizerWinPrediction,
   std::string maximizerLosePrediction)
{
   assert(NULL != curentGameBoardPtr);
   
   std::string prediction;
   
   std::cout << std::endl <<
      playerName <<
      " Player's Thinking...";
   std::cout.flush();
   
   AbstractBoard *nextBoard = NULL;
   int score =
      curentGameBoardPtr->makeAutomaticPlayerMove(
      currentPlayer,
      nextBoard);
   curentGameBoardPtr =
      dynamic_cast<ConnectBoard *>(nextBoard);
   
   assert(NULL != curentGameBoardPtr);

   if(0 == score)
   {
      prediction = tiePrediction;
   }
   else if(curentGameBoardPtr->scoreForMaximizerWin() ==
      score)
   {
      prediction = maximizerWinPrediction;         
   }
   else if(curentGameBoardPtr->scoreForMinimizerWin() ==
      score)
   {
      prediction = maximizerLosePrediction;         
   }
   
   std::cout << prediction;
   
   return curentGameBoardPtr;
}


/////////////////////////////////////////////////////////////////
static AbstractBoard *connect4MakeUserMove(
   AbstractBoard::player currentPlayer,
   AbstractBoard *curentGameBoardPtr,
   std::string playerName,
   const AbstractBoard::boards &successorBoards)
{
   std::cout << playerName <<
      " Player's Turn. Pick a column: ";
   char chosenColumnLabel = 'A';
   
   ConnectBoard *boardToCopy =
      dynamic_cast<ConnectBoard *>(curentGameBoardPtr);
   ConnectBoard newBoard(*boardToCopy);
   
   do
   {
      std::cin.clear(); 
      std::cin >> chosenColumnLabel;
         
      // Ignore extra characters typed to end of line
      std::cin.ignore(
         std::numeric_limits<std::streamsize>::max(), 
         '\n');
      chosenColumnLabel =
         std::toupper(chosenColumnLabel);
   }
   while(!newBoard.dropTokenInColumn(
      currentPlayer, 
      chosenColumnLabel - 'A'));
   
   AbstractBoard::boards::iterator it(
      successorBoards.begin());
   AbstractBoard::boards::iterator end(
      successorBoards.end());
   bool found = false;
   
   while(it != end && !found)
   {
      ConnectBoard *candidatePtr =
         dynamic_cast<ConnectBoard *>(*it);
      
      if(*candidatePtr == newBoard)
      {
         found = true;
         curentGameBoardPtr = candidatePtr;
      }
      
      ++it;
   }
   
   return curentGameBoardPtr;
}


/////////////////////////////////////////////////////////////////
/// This function displays an empty game board and then loops
/// accepting user input specifying the column in which a player
/// wishes to place a token until either the game board is full
/// or a player has won. See the Board class description for more
/// information about game play, winning, and tie games.
/// If any command line argument was provided by the user, this
/// function accepts user input for both the Black and Red
/// players starting with Black and alternating turns. Otherwise,
/// this function accepts user input specifying the column in
/// which the Black player wishes to place a token and then
/// automatically places a Red token in an available column.
int connect4Game(int argc, const char *argv[])
{
   ConnectBoard initialGameBoard;
   
   AbstractBoard *curentGameBoardPtr = &initialGameBoard;
   int numberOfPlayers = 1;
   
   if(argc > 1)
   {  // Command line argument sets number of human players
      numberOfPlayers = atoi(argv[1]);
   }
   
   curentGameBoardPtr->print(std::cout);
   
   // The Black player goes first
   AbstractBoard::player currentPlayer =
      AbstractBoard::Maximizer;
   
   // Get set of all open columns in the board   
   AbstractBoard::boards successorBoards(
      curentGameBoardPtr->allLegalMovesForPlayer(currentPlayer));
   
   while(0 < successorBoards.size() &&
      AbstractBoard::None == curentGameBoardPtr->winner())
   {  // There is a column in which to play and neither player
      // has won
      if(currentPlayer == AbstractBoard::Maximizer)
      {
         if(0 == numberOfPlayers)
         {
            curentGameBoardPtr = connect4MakeAutomaticMove(
               currentPlayer,
               curentGameBoardPtr,
               curentGameBoardPtr->maximizerName(),
               "Tie is predicted. ",
               "and can force a win!",
               "and knows it's a lost cause.");
         }
         else
         {
            curentGameBoardPtr = connect4MakeUserMove(
               currentPlayer,
               curentGameBoardPtr,
               curentGameBoardPtr->maximizerName(),
               successorBoards);
         }
         
         currentPlayer = AbstractBoard::Minimizer;
      }
      else
      {
         if(2 > numberOfPlayers)
         {
            curentGameBoardPtr = connect4MakeAutomaticMove(
               currentPlayer,
               curentGameBoardPtr,
               curentGameBoardPtr->minimizerName(),
               "Tie is predicted. ",
               "and knows it's a lost cause.",
               "and can force a win!");
         }
         else
         {
            curentGameBoardPtr = connect4MakeUserMove(
               currentPlayer,
               curentGameBoardPtr,
               curentGameBoardPtr->minimizerName(),
               successorBoards);
         }
         
         currentPlayer = AbstractBoard::Maximizer;
      }
            
      curentGameBoardPtr->print(std::cout);
      
      successorBoards = curentGameBoardPtr->
         allLegalMovesForPlayer(currentPlayer);
   }
   
   AbstractBoard::player winner = curentGameBoardPtr->winner();
   if(AbstractBoard::None == winner)
   {
      std::cout << "No Winner" << std::endl; 
   }
   else if(AbstractBoard::Maximizer == winner)
   {
      std::cout << "\n" <<
         curentGameBoardPtr->maximizerName() <<
         " Wins" << std::endl;
   }
   else
   {   
      std::cout << "\n" <<
         curentGameBoardPtr->minimizerName() <<
         " Wins" << std::endl;
   } 
   
   return 0;
}

