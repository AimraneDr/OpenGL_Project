#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include "render/render_types.h"
#include "engine_types.h"

bool vbo_create(Vertex* vertices,u32 size, VBO* out_VBO);
bool vbo_bind(VBO* vbo);
bool vbo_unbind(VBO* vbo);
bool vbo_delete(VBO* vbo);

#endif /* VERTEX_BUFFER_H */