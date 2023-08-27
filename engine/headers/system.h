#ifndef SYSTEM_H
#define SYSTEM_H
#include "defines.h"
#include "engine_types.h"
#include "game_types.h"

DLL_EXPORT bool systems_init(SystemState* sysState, GameSetupInfo* game_info);
DLL_EXPORT bool systems_run(SystemState* sysState);
DLL_EXPORT bool systems_sutdown(SystemState* sysState);
#endif /* SYSTEM_H */