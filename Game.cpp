// Game.cpp

#include "provided.h"
#include "support.h"
#include <string>
#include <cmath>
#include <iostream>

class GameImpl
{
public:
    GameImpl(int nColumns, int nLevels, int N, Player* red, Player* black);
    bool completed(int& winner) const;
    bool takeTurn();
    void play();
    int checkerAt(int c, int r) const;
private:
    Scaffold m_board;
    Player* m_playerRed;
    Player* m_playerBlack;
    int m_turn;
    int m_goal;
};

GameImpl::GameImpl(int nColumns, int nLevels, int N, Player* red, Player* black)
    : m_board(nColumns, nLevels), m_playerRed(red), m_playerBlack(black), m_turn(RED), m_goal(N)
{
    if (N <= 0)
    {
        std::cerr << "Invalid 'N'" << std::endl;
        exit(1);
    }
}

bool GameImpl::completed(int& winner) const
{
   
    if (checkAll(m_board, m_goal, RED)) // did red win?
    {
        winner = RED;
        return true;
    }
    else if (checkAll(m_board, m_goal, BLACK)) // did blue win?
    {
        winner = BLACK;
        return true;
    }
    else if (m_board.numberEmpty() == 0) // is the board full w/out winner?
    {
        winner = TIE_GAME;
        return true;
    }
    return false;  
}

bool GameImpl::takeTurn()
{
    int winner = VACANT;         // checking to see if game is over
    if (completed(winner))
        return false;           
 
    if (m_turn == RED)      // red player chooses column, checker is dropped
    {
        int move = m_playerRed->chooseMove(m_board, m_goal, RED);
        if (move < 1 || move > m_board.cols() || !m_board.makeMove(move, RED))
            return false;
        m_turn = BLACK;
        return true;
    }
    else if (m_turn == BLACK)   // black's turn
    {
        int move = m_playerBlack->chooseMove(m_board, m_goal, BLACK);
        if (move < 1 || move > m_board.cols() || !m_board.makeMove(move, BLACK))
            return false;
        m_turn = RED;       // switch back to RED
        return true;
    }
    return false;
}

void GameImpl::play()
{
    int winner = VACANT;
    std::cout << "CONNECT N" << std::endl << std::endl;     // title & initial empty board
    m_board.display();
    std::cout << std::endl << std::endl;

    while (!completed(winner))  // Loop until someone wins or no moves remain
    {
        if (!m_playerRed->isInteractive() && !m_playerBlack->isInteractive())   // if both players are AI, pause so humans can follow
        {
            std::cout << "Press ENTER to continue...";
            std::cin.ignore(10000, '\n');
        }

        if (!takeTurn())    // if there are no more turns able to be played then game is over
            break;

        m_board.display();
    }

    if (completed(winner)) {    // announce final result
        if (winner == RED) {
            std::cout << "Red ("  << m_playerRed->name() 
                      << ") wins!\n";
        }
        else if (winner == BLACK) {
            std::cout << "Black (" << m_playerBlack->name() 
                      << ") wins!\n";
        }
        else {  // tie game
            std::cout << "The game is a tie.\n";
        }
    }
}


int GameImpl::checkerAt(int c, int r) const
{
    return m_board.checkerAt(c, r); 
}

//******************** Game functions **************************************

//  These functions simply delegate to GameImpl's functions.  You should not
//  change any of the code from here to the end of this file.

Game::Game(int nColumns, int nLevels, int N, Player* red, Player* black)
{
    m_impl = new GameImpl(nColumns, nLevels, N, red, black);
}

Game::~Game()
{
    delete m_impl;
}

bool Game::completed(int& winner) const
{
    return m_impl->completed(winner);
}

bool Game::takeTurn()
{
    return m_impl->takeTurn();
}

void Game::play()
{
    m_impl->play();
}

int Game::checkerAt(int c, int r) const
{
    return m_impl->checkerAt(c, r);
}
