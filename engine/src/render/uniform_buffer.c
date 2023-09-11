/**
 * @file uniform_buffer.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-08-22
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "render/uniform_buffer.h"

#include <glad/glad.h>
#include <stdlib.h>
#include "core/logger.h"

bool ubo_create(u32 bindingPointIndex, u32 size, UBO* out_UBO){
    glGenBuffers(1, &out_UBO->id);
    ubo_bind(out_UBO);
    glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_DYNAMIC_DRAW);
    ubo_unbind();
    glBindBufferBase(GL_UNIFORM_BUFFER, bindingPointIndex, out_UBO->id);
    return true;
}
bool ubo_bind(UBO* ubo){
    glBindBuffer(GL_UNIFORM_BUFFER, ubo->id);
    return true;
}
bool ubo_unbind(){
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    return true;
}
bool ubo_delete(UBO* ubo){
    glDeleteBuffers(1,&ubo->id);
    return true;
}