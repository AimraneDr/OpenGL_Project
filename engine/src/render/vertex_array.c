#include "render/vertex_array.h"
#include "render/vertex_buffer.h"
#include <glad/glad.h>

bool vao_create(VAO* out_VAO){
    glGenVertexArrays(1,&out_VAO->id);
    return true;
}

bool vao_link_attrib(VBO* vbo,u32 layout,u32 components_num,u32 type, u32 stride, void* offset){
    vbo_bind(vbo);
    glEnableVertexAttribArray(layout);
    glVertexAttribPointer(layout, components_num, type, GL_FALSE, stride, offset);
    vbo_unbind(vbo);
    return true;
}

bool vao_bind(VAO* vao){
    glBindVertexArray(vao->id);
    return true;
}

bool vao_unbind(VAO* vao){
    glBindVertexArray(0);
    return true;
}

bool vao_delete(VAO* vao){
    glDeleteVertexArrays(1,&vao->id);
    return true;
}

