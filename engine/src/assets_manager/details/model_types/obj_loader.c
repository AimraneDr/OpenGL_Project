#include "assets_manager/details/model_types/obj_loader.h"

#include "core/files.h"
#include "core/CDS/strings.h"
#include "core/logger.h"
#include "core/CDS/dynamic_array.h"

#include "math/math_defines.h"

#include "utils/caster.h"
#include "math/math.h"

#include <stdio.h>
#include <stdlib.h>

bool load_obj(Mesh* out, const char* location){
    FILE* file = fopen(location, "r");
    if (file == NULL) {
        ERR("Failed to open OBJ file: %s", location);
        return 0;
    }

    u32 numVertices = 0;
    u32 numNormals = 0;
    u32 numUVs = 0;
    u32 numIndices = 0;

    char line[128];
    u32 ln = 0;
    while (fgets(line, sizeof(line), file)) {
        ln++;
        if (line[0] == 'v') {
            if (line[1] == ' ') {
                numVertices++;
            }
            else if (line[1] == 'n') {
                numNormals++;
            }
            else if (line[1] == 't') {
                numUVs++;
            }
        }
        else if (line[0] == 'f') {
            numIndices += 3;
        }
        //DEBUG("line : %s", line)
    }
    // Allocate memory for vertices and indices
    Vertex* vertices = (Vertex*)malloc(numVertices * sizeof(Vertex));
    Vec2* uvs = (Vec2*)malloc(numUVs * sizeof(Vec2));
    Vec3* normals = (Vec3*)malloc(numNormals * sizeof(Vec3));
    u32* indices = (u32*)malloc(numIndices * sizeof(u32));

    // Reset file pointer to the beginning of the file
    fseek(file, 0, SEEK_SET);

    u32 vertexIndex = 0;
    u32 normalIndex = 0;
    u32 uvIndex = 0;
    u32 index = 0;
    
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == 'v') {
            if (line[1] == ' ') {
                // Parse vertex position
                Vertex v = {};
                sscanf(line, "v %f %f %f", &v.position.x, &v.position.y, &v.position.z);
                v.color = vec3(1.f,1.f,1.f);
                vertices[vertexIndex] = v;
                vertexIndex++;
            }
            else if (line[1] == 'n') {
                // Parse vertex normal
                Vec3 n = vec3(0.f,0.f,0.f);
                sscanf(line, "vn %f %f %f", &n.x, &n.y, &n.z);
                // n.z*=-1;
                normals[normalIndex] = n;
                normalIndex++;
            }
            else if (line[1] == 't') {
                // Parse texture coordinates
                sscanf(line, "vt %f %f", &uvs[uvIndex].x, &uvs[uvIndex].y);
                uvIndex++;
            }
        }
        else if (line[0] == 'f') {
            // Parse face indices
            u32 vertexIndices[3];
            u32 normalIndices[3];
            u32 uvIndices[3];
            sscanf(
                line, "f %u/%u/%u %u/%u/%u %u/%u/%u",
                &vertexIndices[0],&uvIndices[0],&normalIndices[0],
                &vertexIndices[1],&uvIndices[1],&normalIndices[1],
                &vertexIndices[2],&uvIndices[2],&normalIndices[2]);

            // Subtract 1 from indices to convert to 0-based indexing
            indices[index++] = vertexIndices[0] - 1;
            indices[index++] = vertexIndices[1] - 1;
            indices[index++] = vertexIndices[2] - 1;
            if(numNormals > 0){
                vertices[vertexIndices[0] - 1].normal = normals[normalIndices[0] - 1];
                vertices[vertexIndices[1] - 1].normal = normals[normalIndices[1] - 1];
                vertices[vertexIndices[2] - 1].normal = normals[normalIndices[2] - 1];
            }
            if(numUVs > 0){
                vertices[vertexIndices[0] - 1].uv = uvs[uvIndices[0] - 1];
                vertices[vertexIndices[1] - 1].uv = uvs[uvIndices[1] - 1];
                vertices[vertexIndices[2] - 1].uv = uvs[uvIndices[2] - 1];
            }
        }
    }

    // Close the file
    fclose(file);

    // Set the output values
    out->vertices = vertices;
    out->indices = indices;
    out->numVertices = numVertices;
    out->numIndices = numIndices;

    return 1;
}