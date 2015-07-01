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
#include "Surround2Player.hpp"
#include "ale_interface.hpp"
#include "../RomUtils.hpp"
#include <iostream>

using namespace ale;


Surround2PlayerSettings::Surround2PlayerSettings() {
    reset();
}


/* create a new instance of the rom */
RomSettings* Surround2PlayerSettings::clone() const { 
    
    RomSettings* rval = new Surround2PlayerSettings();
    *rval = *this;
    return rval;
}


/* process the latest information from ALE */
void Surround2PlayerSettings::step(const System& system) {
    int Score = getDecimalScore(0xF6, &system); 
    int ScoreB = getDecimalScore(0xF7, &system);
    // The RL score is the difference in scores
    int newScore = Score-ScoreB;
    int newScoreB = ScoreB-Score;

    m_reward = newScore - m_score;
    m_rewardB = newScoreB - m_scoreB;
    m_score = newScore;
    m_scoreB = newScoreB;

    if(m_reward!=0){
    }
    // The game ends when we reach 10 points
    m_terminal = (Score == 10 || ScoreB == 10); 
}


/* is end of game */
bool Surround2PlayerSettings::isTerminal() const {

    return m_terminal;
};


/* get the most recently observed reward */
reward_t Surround2PlayerSettings::getReward() const { 

    if(m_reward!=0){
    std::cout<< "\n\n\n-----------------SurroundGetRewrd rewardA  "<<m_reward << "rewardB" << m_rewardB <<"--------------"<<std::endl;
    }
    return m_reward; 
}
/* get the most recently observed reward */
reward_t Surround2PlayerSettings::getRewardB() const { 

    return m_rewardB; 
}


bool Surround2PlayerSettings::isLegal(const Action& a) const {
    switch (a) {
        // left player
        case PLAYER_A_NOOP:
        case PLAYER_A_FIRE:
        case PLAYER_A_RIGHT:
        case PLAYER_A_LEFT:
        case PLAYER_A_UP:
        case PLAYER_A_DOWN:
            return true;
        default:
            return false;
    }   
}

bool Surround2PlayerSettings::isLegalB(const Action& a) const {
    switch (a) {
        // left player
        case PLAYER_B_NOOP:
        case PLAYER_B_FIRE:
        case PLAYER_B_RIGHT:
        case PLAYER_B_LEFT:
        case PLAYER_B_UP:
        case PLAYER_B_DOWN:
            return true;
        default:
            return false;
    }   
}
/* is an action part of the minimal set? */
bool Surround2PlayerSettings::isMinimal(const Action &a) const {
    return true; // all legal actions are minimal
}
bool Surround2PlayerSettings::isMinimalB(const Action &a) const {
    return true; // all legal actions are minimal
}

/* reset the state of the game */
void Surround2PlayerSettings::reset() {
    
    m_reward   = 0;
    m_rewardB  = 0;
    m_score    = 0;
    m_scoreB   = 0;
    m_terminal = false;
}

        
/* saves the state of the rom settings */
void Surround2PlayerSettings::saveState(Serializer & ser) {
  ser.putInt(m_reward);
  ser.putInt(m_score);
  ser.putInt(m_rewardB);
  ser.putInt(m_scoreB);
  ser.putBool(m_terminal);
}

// loads the state of the rom settings
void Surround2PlayerSettings::loadState(Deserializer & ser) {
  m_reward = ser.getInt();
  m_score = ser.getInt();
  m_rewardB = ser.getInt();
  m_scoreB = ser.getInt();
  m_terminal = ser.getBool();
}

ActionVect Surround2PlayerSettings::getStartingActions() {

    // This is allocated here rather than once per instantiation to ensure thread safety 
    ActionVect vec;

    // Select game, then reset again
    vec.push_back(RESET);

    return vec;
}
