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
 * RomUtils.hpp
 *
 * Additional utilities to operate on RAM.
 * *****************************************************************************
 */
#include "RomUtils.hpp"

#include "emucore/m6502/src/System.hxx"


namespace ale {


/* reads a byte at a memory location between 0 and 128 */
int readRam(const System* system, int offset) {

    // peek modifies data-bus state, but is logically const from
    // the point of view of the RL interface
    System* sys = const_cast<System*>(system);

    return sys->peek((offset & 0x7F) + 0x80);
}


/* extracts a decimal value from a byte that stores 2 decimal digits with 4 bits each */
int getDecimalScore(int index, const System* system) {
    
    int score = 0;
    if (index < 0) {
        return score;
    }
    int digits_val = readRam(system, index);
    int right_digit = digits_val & 15;
    int left_digit = digits_val >> 4;
    score += ((10 * left_digit) + right_digit);    

    return score;
}


/* extracts a decimal value from 2 bytes */
int getDecimalScore(int lower_index, int higher_index, const System* system) {

    int score = getDecimalScore(lower_index, system);
    if (higher_index < 0) {
        return score;
    }
    score += 100 * getDecimalScore(higher_index, system);
    return score;
}


/* extracts a decimal value from 3 bytes */
int getDecimalScore(int lower_index, int middle_index, int higher_index, const System* system) {

    int score = getDecimalScore(lower_index, middle_index, system);
    if (higher_index < 0) {
        return score;
    }
    score += 10000 * getDecimalScore(higher_index, system);
    return score;
}


} // namespace ale


