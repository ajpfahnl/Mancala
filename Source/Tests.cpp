//
//  Tests.cpp
//  __CS32_Project_3
//
//  Created by Arnold Pfahnl on 8/17/20.
//  Copyright © 2020 Arnold Pfahnl. All rights reserved.
//

#include "Tests.h"
#include "Player.h"
#include "Board.h"
#include "Side.h"
#include "Game.h"
#include <iostream>
#include <cassert>

using namespace std;

void doBoardTests()
{
    Board b(3, 2); //normal board
    assert(b.holes() == 3  &&  b.totalBeans() == 12  &&
           b.beans(SOUTH, POT) == 0  &&  b.beansInPlay(SOUTH) == 6);
    b.setBeans(SOUTH, 1, 1); //normal setBeans
    b.moveToPot(SOUTH, 2, SOUTH); //normal moveToPot
    assert(b.totalBeans() == 11  &&  b.beans(SOUTH, 1) == 1  &&
           b.beans(SOUTH, 2) == 0  &&  b.beans(SOUTH, POT) == 2  &&
           b.beansInPlay(SOUTH) == 3);
    Side es;
    int eh;
    b.sow(SOUTH, 3, es, eh);
    //check a normal valid sow
    assert(es == NORTH  &&  eh == 3  &&  b.beans(SOUTH, 3) == 0  &&
           b.beans(NORTH, 3) == 3  &&  b.beans(SOUTH, POT) == 3  &&
           b.beansInPlay(SOUTH) == 1  &&  b.beansInPlay(NORTH) == 7);
    //Board with 0 for both parameters
    Board c(0,0);
    assert(c.holes() == 1  &&  c.totalBeans() == 0  &&
           c.beans(SOUTH, POT) == 0  &&  c.beansInPlay(SOUTH) == 0);
    //Board with negative values for both parameters
    Board d(-1,-1);
    assert(d.holes() == 1  &&  d.totalBeans() == 0  &&
           d.beans(SOUTH, POT) == 0  &&  d.beansInPlay(SOUTH) == 0);
    //check copy constructor
    Board bb(b);
    assert(es == NORTH  &&  eh == 3  &&  b.beans(SOUTH, 3) == 0  &&
           b.beans(NORTH, 3) == 3  &&  b.beans(SOUTH, POT) == 3  &&
           b.beansInPlay(SOUTH) == 1  &&  b.beansInPlay(NORTH) == 7);
    assert(!bb.setBeans(SOUTH, 1, -1)); //invalid setBeans
    assert(!bb.setBeans(SOUTH, 4, 1)); //invalid setBeans
    assert(!bb.moveToPot(SOUTH, 0, NORTH)); //invalid moveToPot
    assert(!bb.sow(SOUTH, 0, es, eh)); //invalid sow
    assert(!bb.sow(SOUTH, -1, es, eh)); //invalid sow
    //check that previous operations had no impact on the board
    assert(es == NORTH  &&  eh == 3  &&  b.beans(SOUTH, 3) == 0  &&
           b.beans(NORTH, 3) == 3  &&  b.beans(SOUTH, POT) == 3  &&
           b.beansInPlay(SOUTH) == 1  &&  b.beansInPlay(NORTH) == 7);
    Board cc(c);
    assert(c.holes() == 1  &&  c.totalBeans() == 0  &&
           c.beans(SOUTH, POT) == 0  &&  c.beansInPlay(SOUTH) == 0);
    cc = bb;
    assert(es == NORTH  &&  eh == 3  &&  b.beans(SOUTH, 3) == 0  &&
           b.beans(NORTH, 3) == 3  &&  b.beans(SOUTH, POT) == 3  &&
           b.beansInPlay(SOUTH) == 1  &&  b.beansInPlay(NORTH) == 7);
}

void doPlayerTests()
{
    HumanPlayer hp("Marge");
    assert(hp.name() == "Marge"  &&  hp.isInteractive());
    BadPlayer bp("Homer");
    assert(bp.name() == "Homer"  &&  !bp.isInteractive());
    SmartPlayer sp("Lisa");
    assert(sp.name() == "Lisa"  &&  !sp.isInteractive());
    Board b(3, 2);
    b.setBeans(SOUTH, 2, 0);
    //check that the chooseMove functions choose valid holes
    cout << "=========" << endl;
    int n = hp.chooseMove(b, SOUTH);
    cout << "=========" << endl;
    assert(n == 1  ||  n == 3);
    n = bp.chooseMove(b, SOUTH);
    assert(n == 1  ||  n == 3);
    n = sp.chooseMove(b, SOUTH);
    assert(n == 1  ||  n == 3);
    Board c(3,0);
    //check no moves possible for a board with no beans
    cout << "=========" << endl;
    n = hp.chooseMove(c, SOUTH);
    cout << "=========" << endl;
    assert(n == -1);
    n = bp.chooseMove(c, SOUTH);
    assert(n == -1);
    n = sp.chooseMove(c, SOUTH);
    assert(n == -1);
}

void doGameTests()
{
    //check a game that forces the any to move in a specific sequence of steps
    BadPlayer bp1("Bart");
    BadPlayer bp2("Homer");
    Board b(3, 0);
    b.setBeans(SOUTH, 1, 2);
    b.setBeans(NORTH, 2, 1);
    b.setBeans(NORTH, 3, 2);
    Game g(b, &bp1, &bp2);
    bool over;
    bool hasWinner;
    Side winner;
    g.display();
    //    Homer
    //   0  1  2
    // 0         0
    //   2  0  0
    //    Bart
    g.status(over, hasWinner, winner);
    assert(!over && g.beans(NORTH, POT) == 0 && g.beans(SOUTH, POT) == 0 &&
           g.beans(NORTH, 1) == 0 && g.beans(NORTH, 2) == 1 && g.beans(NORTH, 3) == 2 &&
           g.beans(SOUTH, 1) == 2 && g.beans(SOUTH, 2) == 0 && g.beans(SOUTH, 3) == 0);
    
    g.move();
    g.display();
    //   0  1  0
    // 0         3
    //   0  1  0
    g.status(over, hasWinner, winner);
    assert(!over && g.beans(NORTH, POT) == 0 && g.beans(SOUTH, POT) == 3 &&
           g.beans(NORTH, 1) == 0 && g.beans(NORTH, 2) == 1 && g.beans(NORTH, 3) == 0 &&
           g.beans(SOUTH, 1) == 0 && g.beans(SOUTH, 2) == 1 && g.beans(SOUTH, 3) == 0);
    
    g.move();
    g.display();
    //   1  0  0
    // 0         3
    //   0  1  0
    g.status(over, hasWinner, winner);
    assert(!over && g.beans(NORTH, POT) == 0 && g.beans(SOUTH, POT) == 3 &&
           g.beans(NORTH, 1) == 1 && g.beans(NORTH, 2) == 0 && g.beans(NORTH, 3) == 0 &&
           g.beans(SOUTH, 1) == 0 && g.beans(SOUTH, 2) == 1 && g.beans(SOUTH, 3) == 0);
    
    g.move();
    g.display();
    //   1  0  0
    // 0         3
    //   0  0  1
    g.status(over, hasWinner, winner);
    assert(!over && g.beans(NORTH, POT) == 0 && g.beans(SOUTH, POT) == 3 &&
           g.beans(NORTH, 1) == 1 && g.beans(NORTH, 2) == 0 && g.beans(NORTH, 3) == 0 &&
           g.beans(SOUTH, 1) == 0 && g.beans(SOUTH, 2) == 0 && g.beans(SOUTH, 3) == 1);
    
    g.move();
    //   0  0  0
    // 1         4
    //   0  0  0
    g.status(over, hasWinner, winner);
    assert(over);
    assert(over && g.beans(NORTH, POT) == 1 && g.beans(SOUTH, POT) == 4 &&
           g.beans(NORTH, 1) == 0 && g.beans(NORTH, 2) == 0 && g.beans(NORTH, 3) == 0 &&
           g.beans(SOUTH, 1) == 0 && g.beans(SOUTH, 2) == 0 && g.beans(SOUTH, 3) == 0);
    assert(hasWinner && winner == SOUTH);
}

void doAllTests() {
    doBoardTests();
    doPlayerTests();
    doGameTests();
    cout << "Passed all tests" << endl;
}
