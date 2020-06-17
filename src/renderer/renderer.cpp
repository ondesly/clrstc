//
// clrstc micro engine
// сopyright (C) 2019-2020 dmitrii torkhov <dmitriitorkhov@gmail.com>
//

#include <random>

#include "renderer.h"

namespace {

    const char *c_vertex_shader = R"(
        attribute lowp vec4 positionAttribute;
        attribute lowp vec2 textureCoordsAttribute;

        varying lowp vec2 textureCoords;

        void main() {
            gl_Position = positionAttribute;
            textureCoords = textureCoordsAttribute;
        }
    )";

    const char *c_fragment_shader = R"(
        varying lowp vec2 textureCoords;

        uniform sampler2D texture;

        void main() {
            gl_FragColor = texture2D(texture, textureCoords);
        }
    )";

    const char *c_position_attribute_name = "positionAttribute";
    const char *c_texture_coords_attribute_name = "textureCoordsAttribute";

    const GLuint c_position_attribute = 0;
    const GLuint c_texture_coords_attribute = c_position_attribute + 1;

    const float c_point_size = 8.F;

    const GLushort c_indices[] = {0, 1, 2, 1, 2, 3};

}

cc::renderer::renderer() {
    glGenBuffers(1, &m_vertices_buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertices_buffer_id);

    //

    m_program = make_and_use_program();
}

cc::renderer::~renderer() {
    glDeleteBuffers(1, &m_vertices_buffer_id);

    delete[] m_tex_data;
    delete[] m_sample;

    //

    glDeleteProgram(m_program);
}


void cc::renderer::render() {

    // Update

    static auto offset = m_tex_size;
    for (auto i = 0; i < m_tex_size; ++i) {
        m_tex_data[i] = m_sample[(i + offset) % m_tex_size];
    }
    ++offset;

    // Draw

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_tex_width, m_tex_height, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, m_tex_data);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, c_indices);
}

void cc::renderer::set_screen_size(const cc::float2 &size) {
    m_tex_width = GLsizei(size.w / c_point_size);
    m_tex_height = GLsizei(size.h / c_point_size);
    m_tex_size = m_tex_width * m_tex_height;

    // Vertices buffer

    const auto normalized_w = m_tex_width * c_point_size / size.w;
    const auto normalized_h = m_tex_height * c_point_size / size.h;

    vertex vertices_buffer[4] = {
            {{-normalized_w, -normalized_h}, {0.F, 1.F}},
            {{normalized_w, -normalized_h}, {1.F, 1.F}},
            {{-normalized_w, normalized_h}, {0.F, 0.F}},
            {{normalized_w, normalized_h}, {1.F, 0.F}}
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * 4, vertices_buffer, GL_STATIC_DRAW);

    // Texture data

    delete[] m_tex_data;
    m_tex_data = new unsigned short[m_tex_size];

    glPixelStorei(GL_UNPACK_ALIGNMENT, 2);

    glGenTextures(1, &m_texture_id);
    glBindTexture(GL_TEXTURE_2D, m_texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Sample

    std::random_device device;
    std::default_random_engine rng(device());
    std::uniform_int_distribution<int> rnd(0, 65535);

    delete[] m_sample;
    m_sample = new unsigned short[m_tex_size];

    for (auto i = 0; i < m_tex_size; ++i) {
        m_sample[i] = static_cast<unsigned short>(rnd(rng));
    }
}

GLuint cc::renderer::make_and_use_program() const {
    auto program = glCreateProgram();

    const auto vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, sizeof(GLchar), &c_vertex_shader, nullptr);
    glCompileShader(vertex_shader);
    glAttachShader(program, vertex_shader);

    const auto fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, sizeof(GLchar), &c_fragment_shader, nullptr);
    glCompileShader(fragment_shader);
    glAttachShader(program, fragment_shader);

    glEnableVertexAttribArray(c_position_attribute);
    glBindAttribLocation(program, c_position_attribute, c_position_attribute_name);
    glVertexAttribPointer(c_position_attribute, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid *) offsetof(vertex, pos));

    glEnableVertexAttribArray(c_texture_coords_attribute);
    glBindAttribLocation(program, c_texture_coords_attribute, c_texture_coords_attribute_name);
    glVertexAttribPointer(c_texture_coords_attribute, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid *) offsetof(vertex, tex));

    glLinkProgram(program);
    glUseProgram(program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return program;
}
