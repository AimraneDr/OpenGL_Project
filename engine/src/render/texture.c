#include "render/texture.h"

#include "core/image.h"
#include "render/shader.h"

#include <glad/glad.h>
bool texture_create(const char* img_path, Shader* program, Texture* out_tex){
    Img texture;
    imgae_load(img_path,&texture);


    glGenTextures(1,&out_tex->id);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,out_tex->id);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);


    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,texture.resolutionX,texture.resolutionY,0,GL_RGBA,GL_UNSIGNED_BYTE,texture.raw);
    glGenerateMipmap(GL_TEXTURE_2D);

    image_destroy(&texture);
    glBindTexture(GL_TEXTURE0,0);

    out_tex->uniformID = glGetUniformLocation(program->id, "texture");
    shader_use(program);
    glUniform1i(out_tex->uniformID, 0);
    return true;
}


bool texture_bind(Texture* tex, u32 loc){
    u32 l = 0x84C0 + loc;
    glActiveTexture(l);
    glBindTexture(GL_TEXTURE_2D,tex->id);
    return true;
}
bool texture_unbind(u32 loc){
    u32 l = 0x84C0 + loc;
    glBindTexture(l,0);
    return true;
}
bool texture_delete(Texture* tex){
    glDeleteTextures(1,&tex->id);
    return true;
}