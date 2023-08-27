/**
 * @file game_types.h
 * @author your name (aimranedr@gmail.com)
 * @brief defines types related to the game interface
 * @version 0.1
 * @date 2023-07-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef GAME_TYPES_H
#define GAME_TYPES_H

#include "engine_types.h"

typedef struct GameSetupInfo{
    windowConfig window_info;

    /**
     * @brief setup functionality, called when the system first initialize.
     * usualy used to define game parameters and to register 
     * any costume components or systems the game is supposed to use.
     * 
     */
    bool (* setup)();
    /**
     * @brief game start functionality, called just before the start of the game loop.
     * here goes any logic needs to be set before running the game.
     */
    bool (* start)();
    /**
     * @brief game update functionality, called each frame.
     * here goes the game update logic
     * 
     */
    bool (* update)(FrameInfo* frame);
    /**
     * @brief game quit functionality, called when the game shuts down.
     * any last logic or cleanings needs to be done goes here
     * 
     */
    bool (* quit)();
}GameSetupInfo;



#endif /* GAME_TYPES_H */