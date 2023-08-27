#include "render/vertex_buffer.h"

#include <glad/glad.h>
#include "core/CDS/dynamic_array.h"

bool vbo_create(Vertex* vertices,u32 size, VBO* out_VBO){
    glGenBuffers(1,&out_VBO->id);
    glBindBuffer(GL_ARRAY_BUFFER,out_VBO->id);
    glBufferData(GL_ARRAY_BUFFER,size,vertices,GL_STATIC_DRAW);
    return true;
}

bool vbo_bind(VBO* vbo){
    glBindBuffer(GL_ARRAY_BUFFER,vbo->id);
    return true;
}
bool vbo_unbind(VBO* vbo){
    glBindBuffer(GL_ARRAY_BUFFER,0);
    return true;
}
bool vbo_delete(VBO* vbo){
    glDeleteBuffers(1,&vbo->id);
    return true;
}