// Scaffold.cpp

#include "provided.h"

#include <string>
#include <vector>
#include <stack>
#include <iostream>

using namespace std;

class ScaffoldImpl
{
public:
    ScaffoldImpl(int nColumns, int nLevels);
    int cols() const;
    int levels() const;
    int numberEmpty() const;
    int checkerAt(int column, int level) const;
    void display() const;
    bool makeMove(int column, int color);
    int undoMove();
private:
    vector<vector<char>> m_scaffold;       // creating a two-dimensional vector to represent scaffold
    int m_cols;
    int m_levels;
    stack<int> m_recentCol;
};

ScaffoldImpl::ScaffoldImpl(int nColumns, int nLevels)
    : m_cols(nColumns), m_levels(nLevels)
{
    if (nColumns <= 0 || nLevels <= 0)     // if either parameter is non-positive, terminate
    {
        cerr << "Invalid Input" << endl;
        exit(-1);
    }

    vector<char> temp(nLevels, ' ');
    for (int i = 0; i < nColumns; i++)    // fill m_scaffold with a vector for every column
    {
        m_scaffold.push_back(temp);
    }

}

int ScaffoldImpl::cols() const
{
    return m_cols;
}

int ScaffoldImpl::levels() const
{
    return m_levels;
}

int ScaffoldImpl::numberEmpty() const
{
    return (m_cols * m_levels) - m_recentCol.size();
}

int ScaffoldImpl::checkerAt(int column, int level) const
{
    if (column < 1 || column > m_cols || level < 1 || level > m_levels)
        return VACANT;

    int adjustedCol = column - 1;
    int adjustedLvl = level - 1;

    switch (m_scaffold[adjustedCol][adjustedLvl])  
    {
    case 'R':
        return RED;
    case 'B':
        return BLACK;
    default:
        return VACANT;
    }

}

void ScaffoldImpl::display() const
{
    for (int j = m_levels - 1; j >= 0; j--)         // construct the board from left to right, top down
    {
        for (int i = 0; i < m_scaffold.size(); i++)
        {
            cout << "|" << m_scaffold[i][j];
        }
        cout << "|" << endl;
    }

    int boundary = 0;                   // bottom row
    while (boundary < (m_cols * 2) + 1)
    {
        if (boundary % 2 == 0) {    // bottom starts with +
            cout << '+';
            boundary++;
        }
        else {
            cout << '-';
            boundary++;
        }
    }

    cout << endl;

}

bool ScaffoldImpl::makeMove(int column, int color)
{
    if (column > 0 && column <= m_cols && (color == RED || color == BLACK)) // checking for valid parameters
    {
        char temp = 'T';  // 'temp' hold the content at each level as we go downward
        int i = 0;

        while (i < m_levels && temp != ' ') // finding first empty slot in the column
        {
            temp = m_scaffold[column - 1][i];  
            i++;
        }

        if (temp == ' ')    // drop checker at empty slot
        {
            if (color == RED)
                m_scaffold[column - 1][i - 1] = 'R';
            else
                m_scaffold[column - 1][i - 1] = 'B';
            m_recentCol.push(column);
            return true;
        }
        else
            return false;
    }
    return false;
}

int ScaffoldImpl::undoMove()
{
    if (m_recentCol.empty())
        return 0;

    int recentCol = m_recentCol.top();      // get column of most recent move
    for (int i = m_levels - 1; i >= 0; i--)     // find and remove checker
    {
        if (m_scaffold[recentCol - 1][i] == 'R' || m_scaffold[recentCol - 1][i] == 'B')
        {
            m_scaffold[recentCol - 1][i] = ' ';
            m_recentCol.pop();
            return recentCol;
        }
    }
    return 0;
}

//******************** Scaffold functions *******************************

//  These functions simply delegate to ScaffoldImpl's functions.  You should
//  not change any of the code from here to the end of this file.

Scaffold::Scaffold(int nColumns, int nLevels)
{
    m_impl = new ScaffoldImpl(nColumns, nLevels);
}

Scaffold::~Scaffold()
{
    delete m_impl;
}

Scaffold::Scaffold(const Scaffold& other)
{
    m_impl = new ScaffoldImpl(*other.m_impl);
}

Scaffold& Scaffold::operator=(const Scaffold& rhs)
{
    if (this != &rhs)
    {
        Scaffold temp(rhs);
        swap(m_impl, temp.m_impl);
    }
    return *this;
}

int Scaffold::cols() const
{
    return m_impl->cols();
}

int Scaffold::levels() const
{
    return m_impl->levels();
}

int Scaffold::numberEmpty() const
{
    return m_impl->numberEmpty();
}

int Scaffold::checkerAt(int column, int level) const
{
    return m_impl->checkerAt(column, level);
}

void Scaffold::display() const
{
    m_impl->display();
}

bool Scaffold::makeMove(int column, int color)
{
    return m_impl->makeMove(column, color);
}

int Scaffold::undoMove()
{
    return m_impl->undoMove();
}
