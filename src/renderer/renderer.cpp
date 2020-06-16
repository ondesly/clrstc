//
// clrstc micro engine
// сopyright (C) 2019-2020 dmitrii torkhov <dmitriitorkhov@gmail.com>
//

#include <random>

#include "renderer.h"

namespace {

    const char *c_vertex_shader = R"(
        attribute lowp vec4 positionAttribute;
        attribute lowp vec4 colorAttribute;
        attribute lowp float pointSize;
        varying lowp vec4 fragmentColor;

        void main() {
            gl_Position = positionAttribute;
            fragmentColor = colorAttribute;
            gl_PointSize = pointSize;
        }
    )";

    const char *c_fragment_shader = R"(
        varying lowp vec4 fragmentColor;

        void main() {
            gl_FragColor = fragmentColor;
        }
    )";

    const char *c_position_attribute_name = "positionAttribute";
    const char *c_color_attribute_name = "colorAttribute";
    const char *c_point_size_attribute_name = "pointSize";

    const GLuint c_position_attribute = 0;
    const GLuint c_color_attribute = c_position_attribute + 1;
    const GLuint c_point_size_attribute = c_color_attribute + 1;

    const float c_point_size = 8.F;

}

cc::renderer::renderer() {
    glGenBuffers(1, &m_vertex_buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer_id);
    glVertexAttribPointer(c_position_attribute, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    glGenBuffers(1, &m_color_buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, m_color_buffer_id);
    glVertexAttribPointer(c_color_attribute, 3, GL_UNSIGNED_BYTE, GL_TRUE, 0, nullptr);

    //

    m_program = make_and_use_program();
}

cc::renderer::~renderer() {
    glDeleteBuffers(1, &m_vertex_buffer_id);
    delete[] m_vertex_buffer;

    glDeleteBuffers(1, &m_color_buffer_id);
    delete[] m_color_buffer;

    //

    glDeleteProgram(m_program);
}


void cc::renderer::render() {

    // Update

    static std::random_device device;
    static std::default_random_engine rng(device());
    static std::uniform_int_distribution<int> rnd(0, 255);

    for (auto i = 0; i < m_buffer_size; ++i) {
        m_color_buffer[i].r = static_cast<unsigned char>(rnd(rng));
        m_color_buffer[i].g = static_cast<unsigned char>(rnd(rng));
        m_color_buffer[i].b = static_cast<unsigned char>(rnd(rng));
    }

    // Draw

    glBindBuffer(GL_ARRAY_BUFFER, m_color_buffer_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(color3) * m_buffer_size, m_color_buffer, GL_DYNAMIC_DRAW);

    glDrawArrays(GL_POINTS, 0, m_buffer_size);
}

void cc::renderer::set_screen_size(const cc::float2 &size) {
    delete[] m_vertex_buffer;

    // Make buffers

    const auto width = GLushort(std::ceil(size.w / c_point_size));
    const auto height = GLushort(std::ceil(size.h / c_point_size));

    m_buffer_size = width * height;

    m_vertex_buffer = new float2[m_buffer_size];
    m_color_buffer = new color3[m_buffer_size];

    // Fill vertex buffer

    const float norm_w = c_point_size / (size.w / 2);
    const float norm_h = c_point_size / (size.h / 2);

    for (auto i = 0; i < m_buffer_size; ++i) {
        const auto x = (i % width) - width / 2;
        const auto y = (i / width) - height / 2;
        m_vertex_buffer[i] = {(x + 0.5F) * norm_w, (y + 0.5F) * norm_h};
    }

    // Bind vertex buffer

    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float2) * m_buffer_size, m_vertex_buffer, GL_STATIC_DRAW);
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

    glEnableVertexAttribArray(c_color_attribute);
    glBindAttribLocation(program, c_color_attribute, c_color_attribute_name);

    glBindAttribLocation(program, c_point_size_attribute, c_point_size_attribute_name);
    glVertexAttrib1f(c_point_size_attribute, c_point_size);

    glLinkProgram(program);
    glUseProgram(program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return program;
}
