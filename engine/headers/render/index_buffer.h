#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H

#include "render/render_types.h"

bool ibo_create(u32* indices,u32 size, IBO* out_IBO);
bool ibo_bind(IBO* ibo);
bool ibo_unbind(IBO* ibo);
bool ibo_delete(IBO* ibo);

#endif /* INDEX_BUFFER_H */