//
//  Player.hpp
//  __CS32_Project_3
//
//  Created by Arnold Pfahnl on 5/14/19.
//  Copyright © 2019 Arnold Pfahnl. All rights reserved.
//

#ifndef Player_hpp
#define Player_hpp

#include <string>
#include "Side.h"
class Board;
//your Player class definition here, as well as the definitions for the HumanPlayer, BadPlayer, and SmartPlayer classes.

//==========================================================================
// AlarmClock ac(numMilliseconds);  // Set an alarm clock that will time out
//                                  // after the indicated number of ms
// if (ac.timedOut())  // Will be false until the alarm clock times out; after
//                     // that, always returns true.
//==========================================================================

#include <chrono>
#include <future>
#include <atomic>

class AlarmClock
{
public:
    AlarmClock(int ms)
    {
        m_timedOut = false;
        m_isRunning = true;
        m_alarmClockFuture = std::async([=]() {
            for (int k = 0; k < ms  &&  m_isRunning; k++)
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            if (m_isRunning)
                m_timedOut = true;
        });
    }
    
    ~AlarmClock()
    {
        m_isRunning = false;
        m_alarmClockFuture.get();
    }
    
    bool timedOut() const
    {
        return m_timedOut;
    }
    
    AlarmClock(const AlarmClock&) = delete;
    AlarmClock& operator=(const AlarmClock&) = delete;
private:
    std::atomic<bool> m_isRunning;
    std::atomic<bool> m_timedOut;
    std::future<void> m_alarmClockFuture;
};

class Player
{
public:
    Player(std::string name) : m_name(name) {}
    //Create a Player with the indicated name.
    
    std::string name() const {
        return m_name;
    }
    //Return the name of the player.
    
    virtual bool isInteractive() const {
        return false;
    }
    //Return false if the player is a computer player. Return true if the player is human. Most kinds of players will be computer players.
    
    virtual int chooseMove(const Board& b, Side s) const = 0;
    //Every concrete class derived from this class must implement this function so that if the player were to be playing side s and had to make a move given board b, the function returns the move the player would choose. If no move is possible, return −1.
    virtual ~Player() {}
    //Since this class is designed as a base class, it should have a virtual destructor.
private:
    std::string m_name;
};

class HumanPlayer : public Player
{
public:
    HumanPlayer(std::string name) : Player(name) {}
    virtual bool isInteractive() const {
        return true;
    }
    virtual int chooseMove(const Board& b, Side s) const;
    ~HumanPlayer() {}
};

class BadPlayer : public Player
{
public:
    BadPlayer(std::string name) : Player(name) {}
    virtual int chooseMove(const Board& b, Side s) const;
    ~BadPlayer() {}
};

class SmartPlayer : public Player
{
public:
    SmartPlayer(std::string name) : Player(name) {}
    virtual int chooseMove(const Board& b, Side s) const;
    ~SmartPlayer() {}
private:
    void chooseMoveHelper(AlarmClock& ac, Board b, Side s, int& optHole, int& diff, int depth) const;
    bool move(Board& b, Side s, int hole, bool& moveAgain) const;
};

#endif /* Player_hpp */
