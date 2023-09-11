/**
 * @file uniform_buffer.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-08-22
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef UNIFORM_BUFFER_H
#define UNIFORM_BUFFER_H

#include "render/render_types.h"

bool ubo_create(u32 bindingPointIndex, u32 size, UBO* out_UBO);
bool ubo_bind(UBO* ubo);
bool ubo_unbind();
bool ubo_delete(UBO* ubo);

#endif /* UNIFORM_BUFFER_H */