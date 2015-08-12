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
#include "FlapPing2Player.hpp"

#include "../RomUtils.hpp"


using namespace ale;


FlapPing2PlayerSettings::FlapPing2PlayerSettings() {

    reset();
}


/* create a new instance of the rom */
RomSettings* FlapPing2PlayerSettings::clone() const {

    RomSettings* rval = new FlapPing2PlayerSettings();
    *rval = *this;
    return rval;
}


/* process the latest information from ALE */
void FlapPing2PlayerSettings::step(const System& system) {

   
    int leftScore = getDecimalScore(0x94, &system); 
    int rightScore = getDecimalScore(0x95, &system);

    // The RL score is the difference in scores
    int newScore = -(leftScore + rightScore);
    m_reward = newScore - m_score;
    m_score = newScore;

    // The game ends when we reach 10 points
    m_terminal = (leftScore == 10 || rightScore == 10); 
}


/* is end of game */
bool FlapPing2PlayerSettings::isTerminal() const {

    return m_terminal;
};


/* get the most recently observed reward */
reward_t FlapPing2PlayerSettings::getReward() const {

    return m_reward;
}

bool FlapPing2PlayerSettings::isLegal(const Action& a) const {
    switch (a) {
        // left player
        case PLAYER_A_NOOP:
        case PLAYER_A_FIRE:
        case PLAYER_A_RIGHT:
        case PLAYER_A_LEFT:
        //case PLAYER_A_UP:
        //case PLAYER_A_DOWN:

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
bool FlapPing2PlayerSettings::isMinimal(const Action &a) const {
    return true; // all legal actions are minimal
}


/* reset the state of the game */
void FlapPing2PlayerSettings::reset() {

    m_reward   = 0;
    m_score    = 0;
    m_lives    = 5;
    m_terminal = false;
    m_started  = false;
}


/* saves the state of the rom settings */
void FlapPing2PlayerSettings::saveState(Serializer & ser) {
  ser.putInt(m_reward);
  ser.putInt(m_score);
  ser.putBool(m_terminal);
  ser.putBool(m_started);
  ser.putInt(m_lives);
}


/* loads the state of the rom settings */
void FlapPing2PlayerSettings::loadState(Deserializer & ser) {
  m_reward = ser.getInt();
  m_score = ser.getInt();
  m_terminal = ser.getBool();
  m_started = ser.getBool();
  m_lives = ser.getInt();
}

ActionVect FlapPing2PlayerSettings::getStartingActions() {
    ActionVect startingActions;
    startingActions.push_back(SELECT);
    startingActions.push_back(PLAYER_A_NOOP);
    startingActions.push_back(SELECT);
    startingActions.push_back(PLAYER_A_NOOP);
    startingActions.push_back(SELECT);
    startingActions.push_back(RESET);
    return startingActions;
}
