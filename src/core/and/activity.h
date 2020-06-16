//
// clrstc micro engine
// сopyright (C) 2019-2020 dmitrii torkhov <dmitriitorkhov@gmail.com>
//

#pragma once

#include <functional>
#include <memory>

#include <EGL/egl.h>

struct android_app;

namespace cc {

    class activity {
    public:

        struct activity_state {

            struct android_app *app;

            bool animating;
            EGLDisplay display = EGL_NO_DISPLAY;
            EGLSurface surface = EGL_NO_SURFACE;
            EGLContext context = EGL_NO_CONTEXT;
            EGLConfig config;
            EGLint width;
            EGLint height;

        };

    public:

        explicit activity(struct android_app *state);

        virtual ~activity() = default;

    public:

        void run();

    private:

        activity_state m_activity_state;

    };

}