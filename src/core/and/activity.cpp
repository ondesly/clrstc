//
// clrstc micro engine
// сopyright (C) 2019-2020 dmitrii torkhov <dmitriitorkhov@gmail.com>
//

#include <iostream>

#include <android_native_app_glue.h>
#include <GLES/gl.h>

#include "core/and/log_buffer.h"
#include "core/engine.h"
#include "math/float2.h"

#include "activity.h"

static void engine_term_display(cc::activity::activity_state *state) {
    if (state->display != EGL_NO_DISPLAY) {
        eglMakeCurrent(state->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (state->context != EGL_NO_CONTEXT) {
            eglDestroyContext(state->display, state->context);
        }
        if (state->surface != EGL_NO_SURFACE) {
            eglDestroySurface(state->display, state->surface);
        }
        eglTerminate(state->display);
    }

    state->animating = false;
    state->display = EGL_NO_DISPLAY;
    state->context = EGL_NO_CONTEXT;
    state->surface = EGL_NO_SURFACE;
}

static void onContentRectChanged(ANativeActivity *activity, const ARect *rect) {
    cc::engine::i()->set_screen_size(cc::float2(rect->right, rect->bottom));
}

static void engine_init_display(cc::activity::activity_state *state) {
    if (state->display == EGL_NO_DISPLAY) {

        // Make display

        state->display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

        eglInitialize(state->display, nullptr, nullptr);

        // Load configs

        static const EGLint attributes[] = {
                EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
                EGL_SURFACE_TYPE, EGL_WINDOW_BIT | EGL_SWAP_BEHAVIOR_PRESERVED_BIT,
                EGL_BLUE_SIZE, 8,
                EGL_GREEN_SIZE, 8,
                EGL_RED_SIZE, 8,
                EGL_NONE
        };

        EGLint num_config;
        eglChooseConfig(state->display, attributes, nullptr, 0, &num_config);
        EGLConfig configs[num_config];

        eglChooseConfig(state->display, attributes, configs, num_config, &num_config);

        // Choose config

        state->config = configs[0];
        for (auto i = 0; i < num_config; ++i) {
            auto &cfg = configs[i];
            EGLint r, g, b, d, surface_type = 0;
            if (eglGetConfigAttrib(state->display, cfg, EGL_RED_SIZE, &r) &&
                eglGetConfigAttrib(state->display, cfg, EGL_GREEN_SIZE, &g) &&
                eglGetConfigAttrib(state->display, cfg, EGL_BLUE_SIZE, &b) &&
                eglGetConfigAttrib(state->display, cfg, EGL_DEPTH_SIZE, &d) &&
                eglGetConfigAttrib(state->display, cfg, EGL_SURFACE_TYPE, &surface_type) &&
                r == 8 &&
                g == 8 &&
                b == 8 &&
                d == 0 &&
                surface_type & EGL_SWAP_BEHAVIOR_PRESERVED_BIT) {
                state->config = configs[i];
                break;
            }
        }
    }

    // Make context

    if (state->context == EGL_NO_CONTEXT) {
        EGLint attributes_list[] = {
                EGL_CONTEXT_CLIENT_VERSION, 2,
                EGL_NONE
        };

        state->context = eglCreateContext(state->display, state->config, nullptr, attributes_list);
    }

    // Make surface

    state->surface = eglCreateWindowSurface(state->display, state->config, state->app->window, nullptr);

    eglSurfaceAttrib(state->display, state->surface, EGL_SWAP_BEHAVIOR, EGL_BUFFER_PRESERVED);

    //

    if (eglMakeCurrent(state->display, state->surface, state->surface, state->context) == GL_FALSE) {
        engine_term_display(state);
        engine_init_display(state);
        return;
    }

    //

    eglQuerySurface(state->display, state->surface, EGL_WIDTH, &state->width);
    eglQuerySurface(state->display, state->surface, EGL_HEIGHT, &state->height);

    //

    cc::engine::i()->set_screen_size(cc::float2(state->width, state->height));

    if (!cc::engine::i()->is_running()) {
        cc::engine::i()->run();
    }

    state->app->activity->callbacks->onContentRectChanged = onContentRectChanged;
}

static void engine_draw_frame(cc::activity::activity_state *state) {
    if (state->display == nullptr) {
        return;
    }

    //

    cc::engine::i()->tick();

    //

    eglSwapBuffers(state->display, state->surface);
}

static void engine_handle_cmd(struct android_app *app, int32_t cmd) {
    auto state = static_cast<cc::activity::activity_state *>(app->userData);

    switch (cmd) {
        case APP_CMD_INIT_WINDOW:
            if (state->app->window) {
                engine_init_display(state);

                state->animating = true;
            }
            break;
        case APP_CMD_GAINED_FOCUS:
            cc::engine::i()->resume();

            state->animating = true;
            break;
        case APP_CMD_LOST_FOCUS:
            state->animating = false;

            cc::engine::i()->pause();
            break;
        case APP_CMD_DESTROY:
            engine_term_display(state);

            cc::engine::destroy_instance();
            break;
        default:
            break;
    }
}

cc::activity::activity(struct android_app *state) {
    std::cout.rdbuf(new log_buffer);

    //

    memset(&m_activity_state, 0, sizeof(m_activity_state));
    state->userData = &m_activity_state;
    state->onAppCmd = engine_handle_cmd;

    m_activity_state.app = state;
}

void cc::activity::run() {
    while (true) {
        int events;
        struct android_poll_source *source;
        while ((ALooper_pollAll(m_activity_state.animating ? 0 : -1, nullptr, &events, (void **) &source)) >= 0) {
            if (source) {
                source->process(m_activity_state.app, source);
            }

            if (m_activity_state.app->destroyRequested != 0) {
                engine_term_display(&m_activity_state);
                return;
            }
        }

        if (m_activity_state.animating) {
            engine_draw_frame(&m_activity_state);
        }
    }
}