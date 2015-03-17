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
#include "WizardOfWor2Player.hpp"

#include "../RomUtils.hpp"
#include "stdio.h"

using namespace ale;

WizardOfWor2PlayerSettings::WizardOfWor2PlayerSettings() {

    reset();
}


/* create a new instance of the rom */
RomSettings* WizardOfWor2PlayerSettings::clone() const { 
    
    RomSettings* rval = new WizardOfWor2PlayerSettings();
    *rval = *this;
    return rval;
}


/* process the latest information from ALE */
void WizardOfWor2PlayerSettings::step(const System& system) {

    // update the reward
    reward_t player_a_score = getDecimalScore(0x86, 0x88, &system);
    if (player_a_score >= 8000) player_a_score -= 8000; // MGB score does not go beyond 999
    player_a_score *= 100;

    reward_t player_b_score = getDecimalScore(0x85, 0x87, &system);
    if (player_b_score >= 8000) player_b_score -= 8000; // MGB score does not go beyond 999
    player_b_score *= 100;

    m_reward = player_a_score + player_b_score - m_score;
    m_score = player_a_score + player_b_score;

    // update terminal status
    int newLives = readRam(&system, 0x8D) & 0xF;
    int byte1 = readRam(&system, 0xF4);

    bool isWaiting = (readRam(&system, 0xD7) & 0x1) == 0;

    m_terminal = newLives == 0 && byte1 == 0xF8;

    // Wizard of Wor decreases the life total when we move into the play field; we only
    // change the life total when we actually are waiting 
    m_lives = isWaiting ? newLives : m_lives; 
}


/* is end of game */
bool WizardOfWor2PlayerSettings::isTerminal() const {

    return m_terminal;
};


/* get the most recently observed reward */
reward_t WizardOfWor2PlayerSettings::getReward() const { 

    return m_reward; 
}

bool WizardOfWor2PlayerSettings::isLegal(const Action& a) const {
  return true;
}

/* is an action part of the minimal set? */
bool WizardOfWor2PlayerSettings::isMinimal(const Action &a) const {

    switch (a) {
        case PLAYER_A_NOOP:
        case PLAYER_A_FIRE:
        case PLAYER_A_UP:
        case PLAYER_A_RIGHT:
        case PLAYER_A_LEFT:
        case PLAYER_A_DOWN:
        case PLAYER_A_UPFIRE:
        case PLAYER_A_RIGHTFIRE:
        case PLAYER_A_LEFTFIRE:
        case PLAYER_A_DOWNFIRE:
        case PLAYER_B_NOOP:
        case PLAYER_B_FIRE:
        case PLAYER_B_UP:
        case PLAYER_B_RIGHT:
        case PLAYER_B_LEFT:
        case PLAYER_B_DOWN:
        case PLAYER_B_UPFIRE:
        case PLAYER_B_RIGHTFIRE:
        case PLAYER_B_LEFTFIRE:
        case PLAYER_B_DOWNFIRE:
            return true;
        default:
            return false;
    }   
}


/* reset the state of the game */
void WizardOfWor2PlayerSettings::reset() {
    
    m_reward   = 0;
    m_score    = 0;
    m_terminal = false;
    m_lives    = 3;
}

        
/* saves the state of the rom settings */
void WizardOfWor2PlayerSettings::saveState(Serializer & ser) {
  ser.putInt(m_reward);
  ser.putInt(m_score);
  ser.putBool(m_terminal);
  ser.putInt(m_lives);
}

// loads the state of the rom settings
void WizardOfWor2PlayerSettings::loadState(Deserializer & ser) {
  m_reward = ser.getInt();
  m_score = ser.getInt();
  m_terminal = ser.getBool();
  m_lives = ser.getInt();
}

ActionVect WizardOfWor2PlayerSettings::getStartingActions() {
    ActionVect startingActions;
    startingActions.push_back(SELECT);
    return startingActions;
}

