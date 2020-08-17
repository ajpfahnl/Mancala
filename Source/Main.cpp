//
//  Main.cpp
//  __CS32_Project_3
//
//  Created by Arnold Pfahnl on 5/14/19.
//  Copyright © 2019 Arnold Pfahnl. All rights reserved.
//


#include "Player.h"
#include "Board.h"
#include "Side.h"
#include "Game.h"
#include "Tests.h"
#include <iostream>
#include <cassert>
using namespace std;


void smartcomputer_v_badcomputer()
{
    //check two games played on the same board where the smart player is South in the first game and North in the second
    Board b(3, 2);
    cout << " ====SMART BART====" <<endl;
    SmartPlayer bp1("Bart");
    BadPlayer bp2("Homer");
    Game g(b, &bp1, &bp2);
    g.play();
    
    cout << " ====SMART HOMER====" <<endl;
    BadPlayer bp11("Bart");
    SmartPlayer bp22("Homer");
    Game gg(b, &bp11, &bp22);
    gg.play();
    
}

void smartcomputer_v_smartcomputer()
{
    //check two games played on the same board where the smart player is South in the first game and North in the second
    Board b(3, 2);
    cout << " ====SMART BART and SMART HOMER====" <<endl;
    SmartPlayer bp1("Bart");
    SmartPlayer bp2("Homer");
    Game g(b, &bp1, &bp2);
    g.play();
    
}

void human_v_human()
{
    //game created so that
    HumanPlayer bp1("Bart");
    HumanPlayer bp2("Homer");
    
    Board b(6, 0);
    b.setBeans(NORTH, 2, 2);
    b.setBeans(NORTH, 4, 1);
    b.setBeans(SOUTH, 5, 2);
    b.setBeans(SOUTH, 3, 1);
    
    Game g(b, &bp1, &bp2);
    g.play();
}

void human_v_smartcomputer()
{
    //game created so that
    HumanPlayer bp1("Bart");
    SmartPlayer bp2("Homer");
    
    Board b(6, 0);
    b.setBeans(NORTH, 2, 2);
    b.setBeans(NORTH, 4, 1);
    b.setBeans(SOUTH, 5, 2);
    b.setBeans(SOUTH, 3, 1);
    
    Game g(b, &bp1, &bp2);
    g.play();
}

int main()
{
    /* uncomment to perform tests in Tests.h */
    // doAllTests();
    
    /* sample game modes */
    smartcomputer_v_badcomputer();
    // smartcomputer_v_smartcomputer();
    // human_v_human();
    // human_v_smartcomputer();
}

