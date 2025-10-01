// Player.cpp

#include "provided.h"
#include "support.h"
#include <string>

#include <iostream>

using namespace std;

class HumanPlayerImpl
{
public:
    HumanPlayerImpl(HumanPlayer* p);
    int chooseMove(const Scaffold& s, int N, int color);
private:
    HumanPlayer* m_player;
};

class BadPlayerImpl
{
public:
    int chooseMove(const Scaffold& s, int N, int color);
};

class SmartPlayerImpl
{
public:
    int chooseMove(const Scaffold& s, int N, int color);
};

HumanPlayerImpl::HumanPlayerImpl(HumanPlayer* p)
    : m_player(p)
{}

int HumanPlayerImpl::chooseMove(const Scaffold& s, int N, int color)
{
    if (checkAll(s, N, color))
        return 0;

    if (s.numberEmpty() == 0)
        return 0;

    while (true)    // keep asking until player enters a valid column
    {
        int col;
        cout << "Choose a Column: ";
        cin >> col;

        Scaffold test = s;
        if (test.makeMove(col, color))
        {
            return col;
        }
        else
            cout << "Invalid Column" << endl;
    }

}

int BadPlayerImpl::chooseMove(const Scaffold& s, int N, int color)
{
    if (checkAll(s, N, color))
        return 0;

    if (s.numberEmpty() == 0)
        return 0;

    if (s.numberEmpty() > 0)    // pick the first column that accepts a drop
    {

        for (int i = 1; i <= s.cols(); i++)
        {
            Scaffold test = s;
            if (test.makeMove(i, color))
                return i;
        }
    }

    return 0;
}



struct MoveScore     // for the determineBestMove helper function's return value
{
    int col;
    int score;
};

MoveScore determineBestMove(Scaffold& s, int N, int color, int toPlay, int depth)
{
    int BIGNUM = s.cols() * s.levels() + 1;     // number of total possible moves (depth) will not exceed BIGNUM
   
    MoveScore terminalCase;         // terminal positions
    if (checkAll(s, N, color))      // player wins
    {
        terminalCase.col = 0;
        terminalCase.score = +(BIGNUM - depth);
        return terminalCase;
    }

    if (checkAll(s, N, 1 - color))  // opponent wins
    {
        terminalCase.col = 0;
        terminalCase.score = -(BIGNUM - depth);
        return terminalCase;
    }

    if (s.numberEmpty() == 0)       //tie
    {
        terminalCase.col = 0;
        terminalCase.score = 0;
        return terminalCase;
    }


    MoveScore best;

    if (toPlay == color)
    {
        best.col = 0;
        best.score = -BIGNUM;   // start below any real score
        for (int i = 1; i <= s.cols(); i++)     // try every possible drop
        {
            if (!s.makeMove(i, toPlay))
                continue;

            MoveScore reply = determineBestMove(s, N, color, 1 - toPlay, depth + 1); // recurse to see opponent reply
            s.undoMove();
            if (reply.score > best.score) // if this move is better than any we've seen, update best
            {
                best.score = reply.score;
                best.col = i;

                if (best.score == BIGNUM - depth) // stop early if we hit a perfect win at this depth
                    break;
            }
        }
    }
    else if (toPlay != color)   // opponent's turn; goal is to minimize MY score
    {
        best.col = 0;
        best.score = BIGNUM;    // start above any real score
        for (int i = 1; i <= s.cols(); i++)
        {
            if (!s.makeMove(i, toPlay))
                continue;
       
            MoveScore reply = determineBestMove(s, N, color, 1 - toPlay, depth + 1);
            s.undoMove();
            if (reply.score < best.score)   // opponent chooses move that gives me lowest score
            {
                best.score = reply.score;
                best.col = i;

                if (best.score == -(BIGNUM - depth))
                    break;
            }
        }
    }
    return best;
}


int SmartPlayerImpl::chooseMove(const Scaffold& s, int N, int color)
{
    if (s.numberEmpty() == 0)
        return 0;

    for (int i = 1; i <= s.cols(); i++)         // win if possible
    {
        Scaffold test = s;
        if (test.makeMove(i, color) && checkAll(test, N, color))
            return i;
    }
    
    int opponentColor = 1 - color;
    for (int i = 1; i <= s.cols(); i++)         // block immediate opponent win
    {
        Scaffold test = s;
        if (test.makeMove(i, opponentColor) && checkAll(test, N, opponentColor))
            return i;
    }

    
    Scaffold search = s;        // minimax search
    int depth = ((s.cols() * s.levels()) - s.numberEmpty());
    MoveScore result = determineBestMove(search, N, color, color, depth);
    return result.col;

}

//******************** Player derived class functions *************************

//  These functions simply delegate to the Impl classes' functions.  You should
//  not change any of the code from here to the end of this file.

HumanPlayer::HumanPlayer(string nm)
    : Player(nm)
{
    m_impl = new HumanPlayerImpl(this);
}

HumanPlayer::~HumanPlayer()
{
    delete m_impl;
}

int HumanPlayer::chooseMove(const Scaffold& s, int N, int color)
{
    return m_impl->chooseMove(s, N, color);
}

BadPlayer::BadPlayer(string nm)
    : Player(nm)
{
    m_impl = new BadPlayerImpl;
}

BadPlayer::~BadPlayer()
{
    delete m_impl;
}

int BadPlayer::chooseMove(const Scaffold& s, int N, int color)
{
    return m_impl->chooseMove(s, N, color);
}

SmartPlayer::SmartPlayer(string nm)
    : Player(nm)
{
    m_impl = new SmartPlayerImpl;
}

SmartPlayer::~SmartPlayer()
{
    delete m_impl;
}

int SmartPlayer::chooseMove(const Scaffold& s, int N, int color)
{
    return m_impl->chooseMove(s, N, color);
}