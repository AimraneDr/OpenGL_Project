#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include "render/render_types.h"

bool vao_create(VAO* out_VAO);

bool vao_link_attrib(VBO* vbo,u32 layout,u32 components_num,u32 type, u32 stride, void* offset);
bool vao_bind(VAO* vao);
bool vao_unbind(VAO* vao);
bool vao_delete(VAO* vao);

#endif /* VERTEX_ARRAY_H */