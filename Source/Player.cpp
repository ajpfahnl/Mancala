//
//  Player.cpp
//  __CS32_Project_3
//
//  Created by Arnold Pfahnl on 5/14/19.
//  Copyright © 2019 Arnold Pfahnl. All rights reserved.
//

#include "Player.h"
#include "Board.h"
#include "Side.h"
#include <iostream>
#include <cassert>
using namespace std;

//A HumanPlayer chooses its move by prompting a person running the program for a move (reprompting if necessary until the person enters a valid hole number), and returning that choice.
int HumanPlayer::chooseMove(const Board& b, Side s) const
{
    //check to see that a move can be made
    if(b.beansInPlay(s) == 0)
        return -1;
    
    //prompt for input
    int holeInput;
    cout << "Choose your move: ";
    cin >> holeInput;
    
    //check for invalid input
    while(holeInput < 1 || holeInput > b.holes() || b.beans(s, holeInput) == 0) //check out of range and hole has no beans
    {
        cout << "Invalid input. Please try again: ";
        cin >> holeInput;
    }
    
    return holeInput;
}
//A BadPlayer is a computer player that chooses an arbitrary valid move and returns that choice. "Arbitrary" can be what you like: leftmost, nearest to pot, fewest beans, random, etc.. The point of this class is to have an easy-to-implement class that at least plays legally.
int BadPlayer::chooseMove(const Board& b, Side s) const
{
    //check to see that a move can be made
    if(b.beansInPlay(s) == 0)
        return -1;
    
    for(int i = 1; i <= b.holes(); i++) //finds a hole with beans in it and returns that hole
    {
        if(b.beans(s, i) != 0)
            return i;
        
    }
    return -1;
}
//For any game played on a board of up to six holes per side, with up to four initial beans per hole, SmartPlayer::chooseMove must return its choice in no more than five seconds.
int SmartPlayer::chooseMove(const Board& b, Side s) const
{
    //check to see that a move can be made
    if(b.beansInPlay(s) == 0)
        return -1;
    
    AlarmClock ac(4500);  // will time out after 4.5 seconds
    int optHole = 0; int diff;
    chooseMoveHelper(ac, b, s, optHole, diff, RECURSIVEDEPTH);

    if(optHole == 0 || optHole == -1)
    {
        for(int i = 1; i <= b.holes(); i++) //finds a hole with beans in it and returns that hole
        {
            if(b.beans(s, i) != 0)
                return i;
            
        }
    }
    return optHole;
}

//helper function
void SmartPlayer::chooseMoveHelper(AlarmClock& ac, Board b, Side s, int& optHole, int& diff, int depth) const
{
    //initialize diff
    if(s == SOUTH)
    {
        diff = MINVAL;
    }
    else
    {
        diff = MAXVAL;
    }
//BASE CASES
    //game is over
    if(b.beansInPlay(NORTH) == 0 && b.beansInPlay(SOUTH) == 0)
    {
        //if tie
        if(b.beans(NORTH, 0) == b.beans(SOUTH, 0))
        {
            optHole = -1;
            diff = 0;
            return;
        }
        
        //determine winner
        if(b.beans(NORTH, 0) < b.beans(SOUTH, 0)) //SOUTH is winner
        {
            optHole = -1;
            diff = MAXVAL;
            return;
        }
        else                                      //NORTH is winner
        {
            optHole = -1;
            diff = MINVAL;
            return;
        }
    }
    //at max recursion depth
    if(depth == 0) //return difference -- positive for SOUTH favored
    {
        optHole = -1;
        diff = b.beans(SOUTH, 0) - b.beans(NORTH, 0);
        return;
    }
    if(ac.timedOut())
    {
        optHole = -1;
        diff = b.beans(SOUTH, 0) - b.beans(NORTH, 0);
        return;
    }
//RECURSION
    //determine possible moves
    int* possibleMoves = new int[b.holes()];
    int possibleMovesIndex = 0;
    for(int i = 1; i <= b.holes(); i++)
    {
        if(b.beans(s, i) > 0)
        {
            possibleMoves[possibleMovesIndex] = i;
            possibleMovesIndex++;
        }
    }
    
    //recursive steps
    for (int i = 0; i < possibleMovesIndex; i++)
    {
        Board bb(b);
        
        bool moveAgain;
        move(bb, s, possibleMoves[i], moveAgain);
        
        //determine the diff
        int diff2;
        int optHole2;
        if(moveAgain)
            chooseMoveHelper(ac, bb, s, optHole2, diff2, depth-1);
        else
            chooseMoveHelper(ac, bb, opponent(s), optHole2, diff2, depth-1);
        
        if(s == SOUTH && diff2 >= diff)
        {
            optHole = possibleMoves[i];
            diff = diff2;
        }
        if(s == NORTH && diff2 <= diff)
        {
            optHole = possibleMoves[i];
            diff = diff2;
        }
    }
    delete [] possibleMoves;
    return;
}








bool SmartPlayer::move(Board& b, Side s, int hole, bool& moveAgain) const
{
    //game over
    if(b.beansInPlay(NORTH) == 0 && b.beansInPlay(SOUTH) == 0)
    {
        return false;
    }
    
    //the moves
    moveAgain = true;
    Side endSide; int endHole;
    
    b.sow(s, hole, endSide, endHole);
    //ending turn on opponents side
    if(s == SOUTH && endSide == NORTH)
    {
        //check for end game condition (one side of board empty); deposit all
        if(b.beansInPlay(NORTH) == 0 || b.beansInPlay(SOUTH) == 0)
        {
            for(int i = 1; i <= b.holes(); i++)
            {
                b.moveToPot(NORTH, i, NORTH);
                b.moveToPot(SOUTH, i, SOUTH);
            }
        }
        
        return true;
    }
    if(s == NORTH && endSide == SOUTH)
    {
        //check for end game condition (one side of board empty); deposit all
        if(b.beansInPlay(NORTH) == 0 || b.beansInPlay(SOUTH) == 0)
        {
            for(int i = 1; i <= b.holes(); i++)
            {
                b.moveToPot(NORTH, i, NORTH);
                b.moveToPot(SOUTH, i, SOUTH);
            }
        }
        
        return true;
    }
    //capture
    Side otherSide;
    if(endSide == SOUTH)
        otherSide = NORTH;
    else
        otherSide = SOUTH;
    if(endHole > 0 && b.beans(endSide, endHole) == 1 && b.beans(otherSide, endHole) != 0) // == 1 implies that hole was empty before
    {
        if(s == SOUTH)
        {
            b.moveToPot(SOUTH, endHole, SOUTH);
            b.moveToPot(NORTH, endHole, SOUTH);
        }
        else //s == NORTH
        {
            b.moveToPot(SOUTH, endHole, NORTH);
            b.moveToPot(NORTH, endHole, NORTH);
        }
        moveAgain = false;
    }
    
    else //otherwise no capture can be made and the turn ends
        moveAgain = false;
    if(endHole == 0 && endSide == s && b.beansInPlay(s) != 0) //if the player ended on his hole, additional move
        moveAgain = true;
    
    //check for end game condition (one side of board empty); deposit all
    if(b.beansInPlay(NORTH) == 0 || b.beansInPlay(SOUTH) == 0)
    {
        for(int i = 1; i <= b.holes(); i++)
        {
            b.moveToPot(NORTH, i, NORTH);
            b.moveToPot(SOUTH, i, SOUTH);
        }
    }
    
    return true;
}
