
||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
||||||||                     GNU License                        ||||||||
||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

< The Knowledge Island game done by HS1917 UNSW .>
    Copyright (C) <19/05/2013>  < HS1917 UNSW Group >

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

This is the README for the Knowledge Island (KI) game done by the UNSW
tutorial group. The latest version of this program can be done by doing a git pull
from https://github.com/kotarou3/Knowledge-Island. The current version of this 
program is xx. This program was written in C compiled via GCC.http://gcc.gnu.org/

This program simulates the Knowledge Island game (similar to Settler's of Catan). The
description of the game can be found at https://www.openlearning.com/unsw/courses/Computing1/Activities/KnowledgeIsland

|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
||||||||               System Requirements                 ||||||||
|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

To run this program your system needs to be able to run League of Legends at 120
FPS. 

||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
||||||||               Operating Guide                          ||||||||
||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

This program can be compiled by GCC or via Microsoft's Visual C compiler. It is
recommended to compile and run this program/game on a Linux platform. 
Compile via:
gcc -std=c99 -Wall -Wextra -Wno-unused-parameter -Wno-return-type -pedantic *.c -o ki
And run as usual by runinng the executable.

||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
||||||||               Files                                          ||||||||
||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||


----------------------------------------                          ----------------------------------------
|             Game.c                    |  <===================== |             Game.h                   |
----------------------------------------                          ----------------------------------------
                |
                |
                |
                |
                |                ----------------------------------------
                ===============> |       testGame.c (main)              |
                                 ----------------------------------------
                                                   
                                                   
                                                   
|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
||||||||                       Commits                           ||||||||
|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||        

May-11-2013 :-

>Inital Commit
>Code formatting
>Started code
>Add tests and function prototypes and placeholders
>Simplify redefines
>Fix typing errors
>Fixing map types
>Add exchange rate tests
>Add remaining game creation tests
>Fix tests
>Formatting
>Implement all constructors, destructors and getters
>Implement buyArc() and buyCampus()
>Fix getCampus tests
>Split testGameCreation() into three functions
>Make tests match board on openlearning

May-12-2013 :-
>Put the tests in its own function
>Change some constants
>Fix warnings
>Fix exhcange rate testing
>Fix constructRegions()
>Renaming
>Use post-increments instead of pre-increments
>Partial implementation of twoTestRounds()
>Combine test-utils.c, test-utils.h and testGame.c
>Silence warnings
>Allow getOwnedUniversity(), getRegion(), getEdge() and getVertex()
to be passed from asserts for easier implementation of isLegalAction()
>Partial throwDice() implementation

May-13-2013 :-
>Testing
>Removed testing
>Added code for the moving around functions in comments.
>Revert isRegionsEqual(), isEdgesEqual, isVerticesEqul() and isLand
changes
>Revert uncessary comments and line splitting
>Remove uncesscary newlines
>changed isRegion() to isValidRegion()
>Changed pre-decrement to post-decrement
>More tests
>Adding structure for testing ARCs
>Added tags to structs
>Splitted student count struct from University struct
>Changed getArgcs to calculate number each time
>Fixed getArcss() to check if edge is owned.

May-14-2013 :-
>Merged of functions
>Simplifying of functions
>Eliminating redundancy
>Structure updates
>Revert of some changes

May-15-2013 :-
>Revert of changes
>Changed coding style
>Fixed getOwnedUniversity()
>More tests
>Simplifcation of makeAction()
>Improvements
>Fix of typed errors
>Implement isLegalAction START_SPINOFF, RETRAIN_STUDENTS

May-16-2013 :-
>Header upgrade
>Merging of all files and removing, upgrading a fixes of merged file.

||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
||||||||                         FAQ                                ||||||||
||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||                                   
                                                   

//TODO




