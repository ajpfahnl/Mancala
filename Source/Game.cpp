//
//  Game.cpp
//  Mancala
//
//  Created by Arnold Pfahnl on 5/14/19.
//  Copyright © 2019 Arnold Pfahnl. All rights reserved.
//

#include "Game.h"
#include "Board.h"
#include "Player.h"
#include <iostream>
using namespace std;
Game::Game(const Board& b, Player* south, Player* north) : m_b(b), m_playerSouth(south), m_playerNorth(north), playerSouthTurn(true) {}

Game::~Game() {}

void Game::display() const
{
    cout << "      " << m_playerNorth->name() << endl;
    cout << "  ";
    for(int i = 1; i <= m_b.holes(); i++)
    {
        cout << beans(NORTH, i) << " ";
    }
    cout << endl;
    
    cout << beans(NORTH, 0) << " ";
    for(int i = 1; i <= m_b.holes(); i++)
    {
        cout <<"  ";
    }
    cout << beans(SOUTH, 0) << endl;
    
    cout << "  ";
    for(int i = 1; i <= m_b.holes(); i++)
    {
        cout << beans(SOUTH, i) << " ";
    }
    cout << endl;
    cout << "      " << m_playerSouth->name() << endl << endl;
}

void Game::status(bool& over, bool& hasWinner, Side& winner) const
{
    if(m_b.beansInPlay(NORTH) != 0 && m_b.beansInPlay(SOUTH) != 0)
    {
        over = false;
        return;
    }
    
    over = true;
    hasWinner = true;
    if(m_b.beans(NORTH, 0) == m_b.beans(SOUTH, 0)) //tie
        hasWinner = false;
    
    //determine winner
    if(hasWinner == true)
    {
        if(m_b.beans(NORTH, 0) < m_b.beans(SOUTH, 0))
            winner = SOUTH;
        else
            winner = NORTH;
    }
}
//make a complete move for the player whose turn it is (so that it becomes the other player's turn) and return true. "Complete" means that the player sows the seeds from a hole and takes any additional turns required or completes a capture. If the player gets an additional turn, you should display the board so someone looking at the screen can follow what's happening.
bool Game::move()
{
    //game over
    if(m_b.beansInPlay(NORTH) == 0 && m_b.beansInPlay(SOUTH) == 0)
    {
        return false;
    }
    
    //play with the player who's turn it is
    Player* currentPlayer;
    Side s;
    if(playerSouthTurn == true)
    {
        currentPlayer = m_playerSouth;
        s = SOUTH;
    }
    else
    {
        currentPlayer = m_playerNorth;
        s = NORTH;
    }
    
    //the moves
    bool playerTurn = true;
    int hole; Side endSide; int endHole;
    while (playerTurn == true)
    {
        hole = currentPlayer->chooseMove(m_b, s);
        cout << currentPlayer->name() << " chooses hole " << hole << "." <<endl;
        if(hole == -1)
            return false;
        m_b.sow(s, hole, endSide, endHole);
        //ending turn on opponents side
        if(s == SOUTH && endSide == NORTH)
            break;
        if(s == NORTH && endSide == SOUTH)
            break;
        //capture
        Side otherSide;
        if(endSide == SOUTH)
            otherSide = NORTH;
        else
            otherSide = SOUTH;
        if(endHole > 0 && m_b.beans(endSide, endHole) == 1 && m_b.beans(otherSide, endHole) != 0) // == 1 implies that hole was empty before
        {
            if(s == SOUTH)
            {
                m_b.moveToPot(SOUTH, endHole, SOUTH);
                m_b.moveToPot(NORTH, endHole, SOUTH);
            }
            else //s == NORTH
            {
                m_b.moveToPot(SOUTH, endHole, NORTH);
                m_b.moveToPot(NORTH, endHole, NORTH);
            }
            playerTurn = false;
        }
        else //otherwise no capture can be made and the turn ends
            playerTurn = false;
        if(endHole == 0 && endSide == s && m_b.beansInPlay(s) != 0) //if the player ended on his hole, additional move
            playerTurn = true;
    
        if(playerTurn == true)
            display();
    }
    
    //change playerSouthTurn status
    if(playerSouthTurn == true) //now it's north's turn
        playerSouthTurn = false;
    else                        //now it's south's turn
        playerSouthTurn = true;
    
    //check for end game condition (one side of board empty); deposit all 
    if(m_b.beansInPlay(NORTH) == 0 || m_b.beansInPlay(SOUTH) == 0)
    {
        cout << endl;
        display();
        for(int i = 1; i <= m_b.holes(); i++)
        {
            m_b.moveToPot(NORTH, i, NORTH);
            m_b.moveToPot(SOUTH, i, SOUTH);
        }
        cout << "Sweep." << endl;

    }
    
    return true;
}
void Game::play()
{
    cout << "Here is your initial board setup." << endl <<endl;
    display();
    bool gameOver = false;
    bool hasWinner = false;
    Side winner = SOUTH;
    while(gameOver == false)
    {
        if(playerSouthTurn)
            cout << m_playerSouth->name() << "'s turn is next." << endl;
        else
            cout << m_playerNorth->name() << "'s turn is next." << endl;
        if(!m_playerNorth->isInteractive() && !m_playerSouth->isInteractive())
        {
            cout << "Press ENTER to continue: ";
            cin.ignore();
            cout << endl;
        }
        move();
        display();
        status(gameOver, hasWinner, winner);
            
    }
    if(hasWinner == true && winner == SOUTH)
        cout << "The winner is: " << m_playerSouth->name() << endl;
    else if(hasWinner == true && winner == NORTH)
        cout << "The winner is: " << m_playerNorth->name() << endl;
    else
        cout << "Tie!" << endl;

}
int Game::beans(Side s, int hole) const
{
    int beansInHole = m_b.beans(s, hole);
    return beansInHole; //will return -1 if invalid
}
