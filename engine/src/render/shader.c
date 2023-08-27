#include "render/shader.h"

#include "core/files.h"
#include "core/logger.h"
#include "core/CDS/strings.h"
#include <stdlib.h>
#include <glad/glad.h>

void compileErrors(u32 shader, const char* type);


bool shader_create(shaderConfigInfo* config, Shader* out_shader){
    FileHandler vertFileH ,fragFileH ,geoFileH;
    u32 vertShader, fragShader, geoShader;

    bool geoIsSet = true;
    if(config->geometryShaderFilename == null || config->geometryShaderFilename == 0 || stringCompare(config->geometryShaderFilename,"")){
        geoIsSet = false;
    }

    out_shader->id = glCreateProgram();

    //vertex
    if(file_grab(&vertFileH,config->vertexShaderFilename,0)){
        const char* vertContent = file_get_whole_content(&vertFileH);
        vertShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertShader,1,&vertContent,NULL);
        glCompileShader(vertShader);
        compileErrors(vertShader,"VERTEX");
        glAttachShader(out_shader->id,vertShader);
    }else{
        ERR("failed to load vertex shader src file !");
        return false;
    }

    if(geoIsSet == true){
        //geometry
        if(file_grab(&geoFileH,config->geometryShaderFilename,0)){
            const char* geoContent = file_get_whole_content(&geoFileH);
            geoShader = glCreateShader(GL_GEOMETRY_SHADER);
            glShaderSource(geoShader,1,&geoContent,NULL);
            glCompileShader(geoShader);
            compileErrors(geoShader,"GEOMETRY");
            glAttachShader(out_shader->id,geoShader);
        }else{
            ERR("failed to load geometry shader src file !");
            return false;
        }
    }

    //fragment
    if(file_grab(&fragFileH,config->fragmentShaderFilename,0)){
        const char* fragContent = file_get_whole_content(&fragFileH);
        fragShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragShader,1,&fragContent,NULL);
        glCompileShader(fragShader);
        compileErrors(fragShader,"FRAGMENT");
        glAttachShader(out_shader->id,fragShader);
    }else{
        ERR("failed to load fragment shader src file !");
        return false;
    }


    glLinkProgram(out_shader->id);
    compileErrors(out_shader->id,"PROGRAM");

    out_shader->UBOs_count = config->UBOs_initial_num;
    out_shader->UBOs = (UBO*)malloc(sizeof(UBO) * config->UBOs_initial_num);

    glDeleteShader(vertShader);
    if(geoIsSet) glDeleteShader(geoShader);
    glDeleteShader(fragShader);
    file_ditch(&vertFileH);
    if(geoIsSet) file_ditch(&geoFileH);
    file_ditch(&fragFileH);
    return true;
}

bool shader_delete(Shader* shader){
    glDeleteProgram(shader->id);
    return true;
}

bool shader_use(Shader* shader){
    glUseProgram(shader->id);
    return true;
}

void compileErrors(u32 shader, const char* type){
    s32 hasCompiled;
    char infolog[1024];
    if(!stringCompare(type,"PROGRAM")){
        glGetShaderiv(shader,GL_COMPILE_STATUS,&hasCompiled);
        if(hasCompiled == GL_FALSE){
            glGetShaderInfoLog(shader,1024,NULL,infolog);
            ERR("SHADER_COMPILATION_ERROR FOR : %s << %s >>",type,infolog);
        }
    }else{
        glGetProgramiv(shader,GL_COMPILE_STATUS,&hasCompiled);
        if(hasCompiled == GL_FALSE){
            glGetProgramInfoLog(shader,1024,NULL,infolog);
            ERR("SHADER_LINKING_ERROR FOR : %s << %s >>",type,infolog);
        }
    }
}