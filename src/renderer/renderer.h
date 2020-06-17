//
// clrstc micro engine
// сopyright (C) 2019-2020 dmitrii torkhov <dmitriitorkhov@gmail.com>
//

#pragma once

#include <memory>

#include "core/gl.h"
#include "core/singleton.h"
#include "math/float2.h"

namespace cc {

    class renderer : public singleton<renderer> {
        friend class singleton<renderer>;

    public:

        void set_screen_size(const float2 &size);

        void render();

    private:

        struct vertex {

            GLfloat pos[2];
            GLfloat tex[2];

        };

    private:

        GLuint m_program = 0;

        GLuint m_texture_id = 0;
        GLuint m_vertices_buffer_id = 0;

        GLsizei m_tex_width = 0;
        GLsizei m_tex_height = 0;
        GLsizei m_tex_size = 0;

        unsigned short *m_tex_data = nullptr;
        unsigned short *m_sample = nullptr;

    private:

        renderer();

        ~renderer();

    private:

        GLuint make_and_use_program() const;

    };

}
