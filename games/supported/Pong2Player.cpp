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
#include "Pong2Player.hpp"

#include "../RomUtils.hpp"


using namespace ale;


Pong2PlayerSettings::Pong2PlayerSettings() {

    reset();
}


/* create a new instance of the rom */
RomSettings* Pong2PlayerSettings::clone() const {

    RomSettings* rval = new Pong2PlayerSettings();
    *rval = *this;
    return rval;
}


/* process the latest information from ALE */
void Pong2PlayerSettings::step(const System& system) {

    // update the reward
    int left = readRam(&system, 13); // left player score
    int right = readRam(&system, 14); // right player score
    reward_t score = left-right;
    reward_t scoreB = right - left; 
    m_reward = score - m_score;
    m_rewardB =scoreB - m_scoreB;
    m_score = score;
    m_scoreB = scoreB;

    // update terminal status
    // game is over when a player reaches 21,
    // or too many steps have elapsed since the last reset
    m_terminal = left == 21 || right == 21;
}

// we need to lower the episode length further for Pong
int Pong2PlayerSettings::maxFrames() const { 
    return 65000; 
}

/* is end of game */
bool Pong2PlayerSettings::isTerminal() const {

    return m_terminal;
};


/* get the most recently observed reward */
reward_t Pong2PlayerSettings::getReward() const {

    return m_reward;
}

/* get the most recently observed reward  for player 2*/
reward_t Pong2PlayerSettings::getRewardB() const {

    return m_rewardB;
}

bool Pong2PlayerSettings::isLegal(const Action& a) const {
    switch (a) {
        // left player
        case PLAYER_A_NOOP:
        case PLAYER_A_FIRE:
        case PLAYER_A_RIGHT:
        case PLAYER_A_LEFT:
        //case PLAYER_A_UP:
        //case PLAYER_A_DOWN:
/*
        // right player
        case PLAYER_B_NOOP:
        case PLAYER_B_FIRE:
        case PLAYER_B_RIGHT:
        case PLAYER_B_LEFT:
        //case PLAYER_B_UP:
        //case PLAYER_B_DOWN:*/
            return true;
        default:
            return false;
    }   
}

bool Pong2PlayerSettings::isLegalB(const Action& a) const {
        switch (a) {
        // left player
     /*   case PLAYER_A_NOOP:
        case PLAYER_A_FIRE:
        case PLAYER_A_RIGHT:
        case PLAYER_A_LEFT:
        //case PLAYER_A_UP:
        //case PLAYER_A_DOWN:*/

        // right player
        case PLAYER_B_NOOP:
        case PLAYER_B_FIRE:
        case PLAYER_B_RIGHT:
        case PLAYER_B_LEFT:
        //case PLAYER_B_UP:
        //case PLAYER_B_DOWN:
            return true;
        default:
            return false;
    }   
}

/* is an action part of the minimal set? */
bool Pong2PlayerSettings::isMinimal(const Action &a) const {
    return true; // all legal actions are minimal
}


/* reset the state of the game */
void Pong2PlayerSettings::reset() {

    m_reward   = 0;
    m_score    = 0;
    m_rewardB   = 0;
    m_scoreB    = 0;
    m_terminal = false;
}


/* saves the state of the rom settings */
void Pong2PlayerSettings::saveState(Serializer & ser) {
  ser.putInt(m_reward);
  ser.putInt(m_score);
  ser.putInt(m_rewardB);
  ser.putInt(m_scoreB);
  ser.putBool(m_terminal);
}


/* loads the state of the rom settings */
void Pong2PlayerSettings::loadState(Deserializer & ser) {
  m_reward = ser.getInt();
  m_score = ser.getInt();
  m_rewardB = ser.getInt();
  m_scoreB = ser.getInt();
  m_terminal = ser.getBool();
}

ActionVect Pong2PlayerSettings::getStartingActions() {
    ActionVect startingActions;
    startingActions.push_back(SELECT);
    startingActions.push_back(PLAYER_A_NOOP);
    startingActions.push_back(SELECT);
    startingActions.push_back(RESET);
    return startingActions;
}

