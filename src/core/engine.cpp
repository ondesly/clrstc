//
// clrstc micro engine
// сopyright (C) 2019-2020 dmitrii torkhov <dmitriitorkhov@gmail.com>
//

#include <iostream>
#include <utility>

#include "renderer/renderer.h"

#include "engine.h"

cc::engine::engine() : m_renderer(renderer::i()) {
    std::cout << "clrstc micro engine: v0.0.1" << std::endl;
}

cc::engine::~engine() {
    cc::renderer::destroy_instance();
}

void cc::engine::set_screen_size(const cc::float2 &size) {
    if (m_screen_size != size) {
        m_screen_size = size;
        m_is_screen_size_changed = true;
    }
}

void cc::engine::run() {
    m_running = true;
}

bool cc::engine::is_running() const {
    return m_running;
}

void cc::engine::tick() {
    if (m_paused) {
        return;
    }

    // Screen size

    if (m_is_screen_size_changed) {
        m_is_screen_size_changed = false;

        m_renderer->set_screen_size(m_screen_size);
    }

    //

    m_renderer->render();
}

void cc::engine::resume() {
    m_paused = false;
}

void cc::engine::pause() {
    m_paused = true;
}
