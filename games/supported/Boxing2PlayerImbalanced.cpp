/* *****************************************************************************
 * Xitari
 *
 * Copyright 2014 Google Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 * *****************************************************************************
 * A.L.E (Arcade Learning Environment)
 * Copyright (c) 2009-2013 by Yavar Naddaf, Joel Veness, Marc G. Bellemare and 
 *   the Reinforcement Learning and Artificial Intelligence Laboratory
 * Released under the GNU General Public License; see License.txt for details. 
 *
 * Based on: Stella  --  "An Atari 2600 VCS Emulator"
 * Copyright (c) 1995-2007 by Bradford W. Mott and the Stella team
 *
 * *****************************************************************************
 */
#include "Boxing2PlayerImbalanced.hpp"
#include "ale_interface.hpp"
#include "../RomUtils.hpp"
#include <iostream>

using namespace ale;


Boxing2PlayerImbalancedSettings::Boxing2PlayerImbalancedSettings() {

    reset();
}


/* create a new instance of the rom */
RomSettings* Boxing2PlayerImbalancedSettings::clone() const { 
    
    RomSettings* rval = new Boxing2PlayerImbalancedSettings();
    *rval = *this;
    return rval;
}


/* process the latest information from ALE */
void Boxing2PlayerImbalancedSettings::step(const System& system) {

    // update the reward
    int my_score   = getDecimalScore(0x92, &system);
    int oppt_score = getDecimalScore(0x93, &system);

    // handle KO
    if (readRam(&system, 0x92) == 0xC0) my_score   = 100;
    if (readRam(&system, 0x93) == 0xC0) oppt_score = 100;
	int score_A = my_score - oppt_score;//normal reward
	int score_B = -my_score - oppt_score;//defensive
    m_reward = score_A - m_score;
	m_rewardB = score_B - m_scoreB;
    m_score = score_A;
	m_scoreB = score_B;

    // update terminal status
    // if either is KO, the game is over
    if (my_score == 100 || oppt_score == 100) {
        m_terminal = true;
    } else {  // otherwise check to see if out of time
        int minutes = readRam(&system, 0x90) >> 4;
        int seconds = (readRam(&system, 0x91) & 0xF) + 
                      (readRam(&system, 0x91) >> 4) * 10;
        m_terminal = minutes == 0 && seconds == 0;
    }
}


/* is end of game */
bool Boxing2PlayerImbalancedSettings::isTerminal() const {

    return m_terminal;
};


/* get the most recently observed reward */
reward_t Boxing2PlayerImbalancedSettings::getReward() const { 

    return m_reward; 
}

/* get the most recently observed reward */
reward_t Boxing2PlayerImbalancedSettings::getRewardB() const { 

    return m_rewardB; 
}

bool Boxing2PlayerImbalancedSettings::isLegal(const Action& a) const {
    switch (a) {
        // white player
        case PLAYER_A_NOOP:
        case PLAYER_A_FIRE:
        case PLAYER_A_UP:
        case PLAYER_A_RIGHT:
        case PLAYER_A_LEFT:
        case PLAYER_A_DOWN:
        case PLAYER_A_UPRIGHT:
        case PLAYER_A_UPLEFT:
        case PLAYER_A_DOWNRIGHT:
        case PLAYER_A_DOWNLEFT:
        case PLAYER_A_UPFIRE:
        case PLAYER_A_RIGHTFIRE:
        case PLAYER_A_LEFTFIRE:
        case PLAYER_A_DOWNFIRE:
        case PLAYER_A_UPRIGHTFIRE:
        case PLAYER_A_UPLEFTFIRE:
        case PLAYER_A_DOWNRIGHTFIRE:
        case PLAYER_A_DOWNLEFTFIRE:
            return true;
        default:
            return false;
    }   
}

bool Boxing2PlayerImbalancedSettings::isLegalB(const Action& a) const {
    switch (a) {
        case PLAYER_B_NOOP:
        case PLAYER_B_FIRE:
        case PLAYER_B_UP:
        case PLAYER_B_RIGHT:
        case PLAYER_B_LEFT:
        case PLAYER_B_DOWN:
        case PLAYER_B_UPRIGHT:
        case PLAYER_B_UPLEFT:
        case PLAYER_B_DOWNRIGHT:
        case PLAYER_B_DOWNLEFT:
        case PLAYER_B_UPFIRE:
        case PLAYER_B_RIGHTFIRE:
        case PLAYER_B_LEFTFIRE:
        case PLAYER_B_DOWNFIRE:
        case PLAYER_B_UPRIGHTFIRE:
        case PLAYER_B_UPLEFTFIRE:
        case PLAYER_B_DOWNRIGHTFIRE:
        case PLAYER_B_DOWNLEFTFIRE:
            return true;
        default:
            return false;
    }   
}

/* is an action part of the minimal set? */
bool Boxing2PlayerImbalancedSettings::isMinimal(const Action &a) const {
    return true;  
}

bool Boxing2PlayerImbalancedSettings::isMinimalB(const Action &a) const {
    return true; 
}


/* reset the state of the game */
void Boxing2PlayerImbalancedSettings::reset() {
    
    m_reward   = 0;
    m_score    = 0;
    m_rewardB  = 0;
    m_scoreB   = 0;
    m_terminal = false;
}


        
/* saves the state of the rom settings */
void Boxing2PlayerImbalancedSettings::saveState(Serializer & ser) {
	
    ser.putInt(m_reward);
    ser.putInt(m_score);
    ser.putInt(m_rewardB);
    ser.putInt(m_scoreB);
    ser.putBool(m_terminal);
}

// loads the state of the rom settings
void Boxing2PlayerImbalancedSettings::loadState(Deserializer & ser) {
	
    m_reward   = ser.getInt();
    m_score    = ser.getInt();
    m_rewardB  = ser.getInt();
    m_scoreB   = ser.getInt();
    m_terminal = ser.getBool();
}

// start game in 2p mode
ActionVect Boxing2PlayerImbalancedSettings::getStartingActions() {

    ActionVect startingActions;
    startingActions.push_back(SELECT);
    startingActions.push_back(PLAYER_A_NOOP);
    startingActions.push_back(RESET);
    return startingActions;

}
