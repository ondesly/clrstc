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

    class program;

    class renderer : public singleton<renderer> {
        friend class singleton<renderer>;

    public:

        void set_screen_size(const float2 &size);

        void render();

    private:

        struct vertex {

            cc::float2 pos;
            cc::color3 color;

        };

    private:

        GLuint m_program;

        GLsizei m_buffer_size = 0;

        GLuint m_array_buffer = 0;
        vertex *m_vertices_buffer = nullptr;

    private:

        renderer();

        ~renderer();

    private:

        GLuint make_and_use_program() const;

    };

}
