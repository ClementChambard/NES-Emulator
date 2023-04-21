#ifndef VERTEXBATCH_H_
#define VERTEXBATCH_H_

#include "Shader.hpp"
#include "Texture.hpp"
#include <glm/glm.hpp>
#include <vector>

struct Vertex {
    glm::vec3 pos = {};
    glm::vec<4, uint8_t> color = {};
    glm::vec2 uv = {};
};

class VertexBatch {
    public:
        VertexBatch();
        ~VertexBatch();

        void clear_vertices() { vertices.clear(); }
        void add_vertex(Vertex const& v) { vertices.push_back(v); }
        void finish_adding();
        void render(Texture* texture);

        void draw_rectangle(float x, float y, float z, float w, float h, glm::mat4 const& transf = glm::mat4(1.f), glm::vec4 uvs = glm::vec4(0,0,1,1), glm::vec<4, uint8_t> color = {255,255,255,255});

    private:
        GLuint vbo;
        std::vector<Vertex> vertices;
};

#endif // VERTEXBATCH_H_
