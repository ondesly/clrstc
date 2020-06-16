//
// clrstc micro engine
// сopyright (C) 2019-2020 dmitrii torkhov <dmitriitorkhov@gmail.com>
//

#pragma once

#include <memory>

#include "core/gl.h"
#include "core/singleton.h"
#include "math/color3.h"
#include "math/float2.h"

namespace cc {

    class renderer : public singleton<renderer> {
        friend class singleton<renderer>;

    public:

        void set_screen_size(const float2 &size);

        void render();

    private:

        GLuint m_program;

        GLsizei m_buffer_size = 0;

        GLuint m_vertex_buffer_id = 0;
        float2 *m_vertex_buffer = nullptr;

        GLuint m_color_buffer_id = 0;
        color3 *m_color_buffer = nullptr;

    private:

        renderer();

        ~renderer();

    private:

        GLuint make_and_use_program() const;

    };

}
