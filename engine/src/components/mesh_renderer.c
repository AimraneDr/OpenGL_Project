#include "components/mesh_renderer.h"

//TODO : move to rendering layer
//should be removed and be passed to the reder middleware to handle it
#include <glad/glad.h>

#include "render/index_buffer.h"
#include "render/vertex_buffer.h"
#include "render/vertex_array.h"
#include "math/math.h"
#include "render/camera.h"
#include "core/logger.h"


MeshRenderer mesh_renderer_create(Mesh* m){
    MeshRenderer mr = {};
    mr.mesh = *m;
    vao_create(&mr.VAO);
    vao_bind(&mr.VAO);
    vbo_create(mr.mesh.vertices, (u32)sizeof(Vertex) * mr.mesh.numVertices, &mr.VBO);

    ibo_create(mr.mesh.indices, (u32)sizeof(u32) * mr.mesh.numIndices, &mr.IBO);

    // position
    vao_link_attrib(
        &mr.VBO, 0,
        3, GL_FLOAT,
        sizeof(Vertex),
        (void*)0
    );

    // normal
    vao_link_attrib(
        &mr.VBO, 1,
        3, GL_FLOAT,
        sizeof(Vertex),
        (void*)(sizeof(Vec3))
    );

    // color
    vao_link_attrib(
        &mr.VBO, 2,
        3, GL_FLOAT,
        sizeof(Vertex),
        (void*)(2*sizeof(Vec3))
    );
    
    // uv
    vao_link_attrib(
        &mr.VBO, 3,
        2, GL_FLOAT,
        sizeof(Vertex),
        (void*)(3*sizeof(Vec3))
    );

    vao_unbind(&mr.VAO);
    vbo_unbind(&mr.VBO);
    return mr;
}

bool mesh_renderer_destroy(MeshRenderer* m){
    vao_delete(&m->VAO);
    vbo_delete(&m->VBO);
    ibo_delete(&m->IBO);
    return true;
}

bool mesh_renderer_draw_mesh(Shader* shader, MeshRenderer* m, Transform transform, Camera* cam){
    Mat4 model = mat4(1.f);
    model = mat4_scale(model, transform.scale);
    model = mat4_rotate(&model, transform.rotation);
    model = mat4_translate(model, transform.position);

    
    u32 modelUni = glGetUniformLocation(shader->id, "model");
    glUniformMatrix4fv(modelUni, 1, GL_FALSE, &(model.elements[0]));

    Mat4 projection_view = camera_view_projection_mat(cam);
    glUniformMatrix4fv(glGetUniformLocation(shader->id, "proj_view"),1, GL_FALSE, &(projection_view.elements[0]));
    

    vao_bind(&m->VAO);
    glDrawElements(GL_TRIANGLES, m->mesh.numIndices, GL_UNSIGNED_INT, 0);
    return true;
}