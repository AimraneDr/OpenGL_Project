#ifndef SHADER_H
#define SHADER_H

#include "render/render_types.h"

/**
 * @brief struct holds the shader's creation info, please set unwanted fileds to 0 or null or empty string
 * 
 */
typedef struct shaderConfigInfo{
    char* vertexShaderFilename;
    char* fragmentShaderFilename;
    char* geometryShaderFilename;

    u32 UBOs_initial_num;
}shaderConfigInfo;

bool shader_create(shaderConfigInfo* config,Shader* out_shader);
bool shader_delete(Shader* shader);
bool shader_use(Shader* shader);

#endif /* SHADER_H */