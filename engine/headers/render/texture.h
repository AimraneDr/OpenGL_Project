#ifndef TEXTURE_H
#define TEXTURE_H

#include "defines.h"
#include "render_types.h"

bool texture_create(const char* img_path, Shader* program, Texture* out_tex);
bool texture_bind(Texture* tex);
bool texture_unbind();
bool texture_delete(Texture* tex);

#endif /* TEXTURE_H */