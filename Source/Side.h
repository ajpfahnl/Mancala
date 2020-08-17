//
//  Side.h
//  Mancala
//
//  Created by Arnold Pfahnl on 5/14/19.
//  Copyright © 2019 Arnold Pfahnl. All rights reserved.
//
#ifndef GLOBAL_H
#define GLOBAL_H

enum Side { NORTH, SOUTH };

const int NSIDES = 2;
const int POT = 0;
const int MINVAL = -10000;
const int MAXVAL = 10000;
const int RECURSIVEDEPTH = 1410065407;
//const int RECURSIVEDEPTH = 6;


inline
Side opponent(Side s)
{
    return Side(NSIDES - 1 - s);
}

#endif

