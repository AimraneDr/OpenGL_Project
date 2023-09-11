#include "render/index_buffer.h"

#include <glad/glad.h>

bool ibo_create(u32* indices,u32 size, IBO* out_IBO){
    glGenBuffers(1,&out_IBO->id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,out_IBO->id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,size,indices,GL_STATIC_DRAW);
    return true;
}
bool ibo_bind(IBO* ibo){
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibo->id);
    return true;
}
bool ibo_unbind(IBO* ibo){
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    return true;
}
bool ibo_delete(IBO* ibo){
    glDeleteBuffers(1,&ibo->id);
    return true;
}