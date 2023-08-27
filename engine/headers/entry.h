#ifndef ENGINE_ENTRY_H
#define ENGINE_ENTRY_H

#include "defines.h"
#include "system.h"
#include "game_types.h"
#include "core/logger.h"

/**
 * @brief thiis function is the connections point between the game and the engine.
 * 
 * @param info game configurations
 * @return true if success.
 * @return false if failed
 */
bool game_init(GameSetupInfo* info);

int main(){
    SystemState sysState = {};
    GameSetupInfo gSetup = {};
    if(!game_init(&gSetup)) return -1;
    
    systems_init(&sysState, &gSetup);

    systems_run(&sysState);

    systems_sutdown(&sysState);
    return 0;
}

#endif /* ENGINE_ENTRY_H */