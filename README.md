# CLI Mancala
This CLI Mancala game coded in C++ can be configured with a variety of board sizes and pot sizes. There are three types of players: dumb computer, smart computer, and human.

## Description of Design for SmartPlayer::chooseMove
SmartPlayer::chooseMove uses a helper function called SmartPlayer::chooseMoveHelper to determine the optimal move. SmartPlayer::chooseMoveHelper determines the best move through recursion. For every hole that can be played, the helper function will make a move with that hole. If a hole results in a better “play” than the hole that is currently stored, that hole becomes the new “optimal” hole. Determining how good a “play” is is determined when the game is over, the maximum recursion depth is reached, or the maximum time allotted is reached.  For a game over condition, if the player loses, that’s the worst possible play. If the player wins, that’s the best possible play. For the max recursion depth or out of time conditions, then the best play is determined based on the difference between the pots. For the south player, the value of South pot minus the value of North pot should be as large as possible (the larger, the better the play). For the north player, the difference should be as small as possible (the smaller the better the play).

## Example Play: Smart Computer vs. Dumb Computer
```
 ====SMART BART====
Here is your initial board setup.

      Homer
  2 2 2 
0       0
  2 2 2 
      Bart

Bart's turn is next.
Press ENTER to continue: 

Bart chooses hole 3.
      Homer
  2 2 3 
0       1
  2 2 0 
      Bart

Homer's turn is next.
Press ENTER to continue: 

Homer chooses hole 1.
      Homer
  0 2 3 
1       1
  3 2 0 
      Bart

Bart's turn is next.
Press ENTER to continue: 

Bart chooses hole 2.
      Homer
  0 2 3 
1       2
  3 0 1 
      Bart

Bart chooses hole 3.
      Homer
  0 2 3 
1       3
  3 0 0 
      Bart

Bart chooses hole 1.
      Homer
  0 2 3 
1       4
  0 1 1 
      Bart

Bart chooses hole 3.
      Homer
  0 2 3 
1       5
  0 1 0 
      Bart

Bart chooses hole 2.

      Homer
  0 2 0 
1       9
  0 0 0 
      Bart

Sweep.
      Homer
  0 0 0 
3       9
  0 0 0 
      Bart

The winner is: Bart
 ====SMART HOMER====
Here is your initial board setup.

      Homer
  2 2 2 
0       0
  2 2 2 
      Bart

Bart's turn is next.
Press ENTER to continue: 

Bart chooses hole 1.
      Homer
  2 2 2 
0       0
  0 3 3 
      Bart

Homer's turn is next.
Press ENTER to continue: 

Homer chooses hole 3.
      Homer
  3 3 0 
0       0
  0 3 3 
      Bart

Bart's turn is next.
Press ENTER to continue: 

Bart chooses hole 2.
      Homer
  3 3 1 
0       1
  0 0 4 
      Bart

Homer's turn is next.
Press ENTER to continue: 

Homer chooses hole 3.
      Homer
  3 4 0 
0       1
  0 0 4 
      Bart

Bart's turn is next.
Press ENTER to continue: 

Bart chooses hole 3.

      Homer
  4 5 1 
0       2
  0 0 0 
      Bart

Sweep.
      Homer
  0 0 0 
10       2
  0 0 0 
      Bart

The winner is: Homer
Program ended with exit code: 0
```
