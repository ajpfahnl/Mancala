//
//  Board.cpp
//  Mancala
//
//  Created by Arnold Pfahnl on 5/14/19.
//  Copyright © 2019 Arnold Pfahnl. All rights reserved.
//

#include "Board.h"
Board::Board(int nHoles, int nInitialBeansPerHole)
{
    if(nHoles < 1)
        m_Holes = 1;
    else
        m_Holes = nHoles;
    if(nInitialBeansPerHole < 0)
        m_InitialBeansPerHole = 0;
    else
        m_InitialBeansPerHole = nInitialBeansPerHole;
    
    //create arrays representing pots and holes for each side
    m_BeansPerHoleSouth = new int[m_Holes+1];
    m_BeansPerHoleNorth = new int[m_Holes+1];
    
    //initialize pots -- last index
    m_BeansPerHoleSouth[m_Holes] = 0;
    m_BeansPerHoleNorth[m_Holes] = 0;
    
    //initialize holes -- North holes will have to be mapped
    for(int i = 0; i < m_Holes ; i++)
    {
        m_BeansPerHoleSouth[i] = m_InitialBeansPerHole;
        m_BeansPerHoleNorth[i] = m_InitialBeansPerHole;
    }
}

Board::Board(const Board& other)
{
    m_Holes = other.m_Holes;
    m_InitialBeansPerHole = other.m_InitialBeansPerHole;
    
    //create arrays representing pots and holes for each side
    m_BeansPerHoleSouth = new int[m_Holes+1];
    m_BeansPerHoleNorth = new int[m_Holes+1];
    
    //copy over holes and pots
    for(int i = 0; i < m_Holes+1; i++)
    {
        m_BeansPerHoleSouth[i] = other.m_BeansPerHoleSouth[i];
        m_BeansPerHoleNorth[i] = other.m_BeansPerHoleNorth[i];
    }
}

Board& Board::operator=(const Board& rhs)
{
    m_Holes = rhs.m_Holes;
    m_InitialBeansPerHole = rhs.m_InitialBeansPerHole;
    
    //create arrays representing pots and holes for each side
    delete [] m_BeansPerHoleSouth;
    delete [] m_BeansPerHoleNorth;
    m_BeansPerHoleSouth = new int[m_Holes+1];
    m_BeansPerHoleNorth = new int[m_Holes+1];
    
    //copy over holes and pots
    for(int i = 0; i < m_Holes+1; i++)
    {
        m_BeansPerHoleSouth[i] = rhs.m_BeansPerHoleSouth[i];
        m_BeansPerHoleNorth[i] = rhs.m_BeansPerHoleNorth[i];
    }
    return *this;
}

Board::~Board()
{
    delete [] m_BeansPerHoleNorth;
    delete [] m_BeansPerHoleSouth;
}

int Board::holes() const
{
    return m_Holes;
}

int Board::beans(Side s, int hole) const
{
    if(hole < 0 || hole > m_Holes +1)
        return -1;
    if(s == SOUTH)
    {
        if(hole == 0)
            return m_BeansPerHoleSouth[m_Holes];
        else
            return m_BeansPerHoleSouth[hole-1];
    }
    else //NORTH
    {
        if(hole == 0)
            return m_BeansPerHoleNorth[m_Holes];
        else          //beans in South holes; e.g. m_Holes = 6, hole = 2, 6-2 = 4 <-- program's index
            return m_BeansPerHoleNorth[m_Holes - hole];
    }
}

int Board::beansInPlay(Side s) const
{
    int sum = 0;
    if(s == NORTH)
    {
        for(int i = 0; i < m_Holes; i++)
            sum += m_BeansPerHoleNorth[i];
    }
    else
    {
        for(int i = 0; i < m_Holes; i++)
            sum += m_BeansPerHoleSouth[i];
    }
    return sum;
}

int Board::totalBeans() const
{
    int sum = 0;
    for(int i = 0; i < m_Holes+1; i++)
    {
        sum += m_BeansPerHoleNorth[i];
        sum += m_BeansPerHoleSouth[i];
    }
    return sum;
}

bool Board::sow(Side s, int hole, Side& endSide, int& endHole)
{
    if(hole == 0 || beans(s, hole) <= 0)
        return false;
    
    Side play_s = s;
    int play_hole = hole;
    
    //initial removal of beans
    int beansInPlay = removeBeans(play_s, play_hole, 0);
    //sowing
    while(beansInPlay != 0)
    {
        if(play_s == NORTH)
        {
            play_hole--;
            if(play_hole == 0 && s == SOUTH) //skip opponent's pot
            {
                play_s = SOUTH;
                play_hole = 1;
            }
            addBeans(play_s, play_hole, 1);
            beansInPlay--;
            if(play_hole == 0)
            {
                play_s = SOUTH;
            }
            
        }
        else //SOUTH
        {
            play_hole++;
            if(play_hole == m_Holes+1 && s == NORTH) //skip opponent's pot
            {
                play_s = NORTH;
                play_hole = m_Holes;
            }
            addBeans(play_s, play_hole, 1);
            beansInPlay--;
            if(play_hole == m_Holes+1)
            {
                play_s = NORTH;
            }
        }
    }
    if(play_hole == m_Holes+1) //make sure that if we're at a pot hole, play_hole is 0
        play_hole = 0;
    if(play_hole == 0)
    {
        if(play_s == SOUTH)
            play_s = NORTH;
        else
            play_s = SOUTH;
    }

//    if(play_hole == m_Holes+1)
//        m_Holes = 0;
    endSide = play_s;
    endHole = play_hole;
    return true;
}

bool Board::moveToPot(Side s, int hole, Side potOwner)
{
    if(beans(s, hole) == -1 || hole == 0)
        return false;
    else
    {
        int beansToMove = removeBeans(s, hole, 0);
        addBeans(potOwner, 0, beansToMove);
        return true;
    }
}

bool Board::setBeans(Side s, int hole, int beans)
{

    if(hole < 0 || hole > m_Holes)
        return false;
    if(beans < 0)
        return false;
    else
    {
        removeBeans(s, hole, beans);
        return true;
    }
}
//---------------Private Functions---------------
int Board::removeBeans(Side s, int hole, int beansToReplace)
{
    int beansRemoved = 0;
    if(s == SOUTH)
    {
        if(hole == 0)
        {
            beansRemoved = m_BeansPerHoleSouth[m_Holes];
            m_BeansPerHoleSouth[m_Holes] = beansToReplace;
        }
        else
        {
            beansRemoved = m_BeansPerHoleSouth[hole-1];
            m_BeansPerHoleSouth[hole-1] = beansToReplace;
        }
    }
    
    else //North
    {
        if(hole == 0) //beans in North pot
        {
            beansRemoved = m_BeansPerHoleNorth[m_Holes];
            m_BeansPerHoleNorth[m_Holes] = beansToReplace;
        }
        else          //beans in South holes; e.g. m_Holes = 6, hole = 2, 6-2 = 4 <-- program's index
        {
            beansRemoved = m_BeansPerHoleNorth[m_Holes - hole];
            m_BeansPerHoleNorth[m_Holes - hole] = beansToReplace;
        }
    }
    return beansRemoved;
}

void Board::addBeans(Side s, int hole, int amountToAdd)
{
    if (s == SOUTH)
    {
        if(hole == 0)
        {
            m_BeansPerHoleSouth[m_Holes]+=amountToAdd;
        }
        else
        {
            m_BeansPerHoleSouth[hole-1]+=amountToAdd;
        }
    }
    else //North
    {
        if(hole == 0) //beans in North pot
        {
            m_BeansPerHoleNorth[m_Holes]+=amountToAdd;
        }
        else          //beans in South holes; e.g. m_Holes = 6, hole = 2, 6-2 = 4 <-- program's index
        {
            m_BeansPerHoleNorth[m_Holes - hole]+=amountToAdd;
        }
    }
}
