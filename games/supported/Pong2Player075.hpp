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
#ifndef __Pong2Player075_HPP__
#define __Pong2Player075_HPP__

#include "../RomSettings.hpp"

namespace ale {

// RL wrapper for SpaceInvaders
class Pong2Player075Settings : public RomSettings {

    public:

        Pong2Player075Settings();

        // reset
        void reset();

        // is end of game
        bool isTerminal() const;

        // get the most recently observed reward
        reward_t getReward() const;
	reward_t getRewardB() const;
        // the rom-name
        const char* rom() const { return "Pong2Player075"; }

        // create a new instance of the rom
        RomSettings* clone() const;

		// is an action legal?
        bool isLegal(const Action& a) const;
        bool isLegalB(const Action& a) const;
        // is an action part of the minimal set?
        bool isMinimal(const Action& a) const;
        bool isMinimalB(const Action& a) const;
        // process the latest information from ALE
        void step(const System& system);
		
	// we need to lower the episode length further for Pong
        //virtual int maxFrames() const;

        // saves the state of the rom settings
        void saveState(Serializer & ser);
    
        // loads the state of the rom settings
        void loadState(Deserializer & ser);

        virtual int lives() const { return 0; }
        virtual int livesB() const { return 0; }
        ActionVect getStartingActions();
        double getSideBouncing() const;
        bool getWallBouncing() const;
        int getPoints() const;
        bool getCrash() const;
        bool getServing() const;
    private:

        bool m_terminal;
        reward_t m_reward;
        reward_t m_score;
        reward_t m_rewardB;
        reward_t m_scoreB;
        double sideBouncing;
        bool wallBouncing;
        int points;
        bool crash;
        bool serving;
};

} // namespace ale

#endif // __Pong2Player075_HPP__

