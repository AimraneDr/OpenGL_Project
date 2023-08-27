#ifndef ASSETS_MANAGER_H
#define ASSETS_MANAGER_H

#include "defines.h"
#include "assets_types.h"
#include "render/render_types.h"


DLL_EXPORT bool assets_manager_load_asset(Mesh* out, const char* location, AssetType type);

#endif /* ASSETS_MANAGER_H */