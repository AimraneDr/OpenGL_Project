#include "assets_manager/assets_manager.h"

#include "assets_manager/details/model_types/obj_loader.h"

bool assets_manager_load_asset(Mesh* out, const char* location, AssetType type){
    switch (type)
    {
    case ASSET_TYPE_MODEL:
        load_obj(out,location);
        break;
    default:
        return false;
    }
    return true;
}
