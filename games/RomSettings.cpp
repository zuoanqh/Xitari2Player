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
 *
 * RomSettings.cpp
 *
 * The interface to describe games as RL environments. It provides terminal and
 *  reward information.
 * *****************************************************************************
 */
#include "RomSettings.hpp"
#include <iostream>
using namespace ale;

bool RomSettings::isLegal(const Action& a) const {
  return a < PLAYER_A_MAX;
}

bool RomSettings::isLegalB(const Action& a) const {
  return a > PLAYER_A_MAX && a < PLAYER_B_MAX;
}

ActionVect& RomSettings::getMinimalActionSet() {
  if (actions.empty()) {
    for (int a = 0; a < PLAYER_B_MAX; a++)
      if (isMinimal((Action)a) && isLegal((Action)a))
        actions.push_back((Action)a);
  }

  return actions;
}

ActionVect& RomSettings::getMinimalActionSetB() {
  if (actionsB.empty()) {
    for (int a = 0; a < PLAYER_B_MAX; a++)
      if (isMinimalB((Action)a) && isLegalB((Action)a))
        actionsB.push_back((Action)a);
  }

  return actionsB;
}

ActionVect& RomSettings::getAllActions() {
  // Generate the set of all actions
  if (all_actions.empty()) {
    for (int a = 0; a < PLAYER_B_MAX; a++)
      if (isLegal((Action)a))
        all_actions.push_back((Action)a);
  }

  return all_actions;
}

ActionVect& RomSettings::getAllActionsB() {
  // Generate the set of all actions
  if (all_actionsB.empty()) {
    for (int a = 0; a < PLAYER_B_MAX; a++)
      if (isLegalB((Action)a))
        all_actionsB.push_back((Action)a);
  }

  return all_actionsB;
}

ActionVect RomSettings::getStartingActions() {
    return ActionVect();
}
