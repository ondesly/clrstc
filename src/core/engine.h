//
// clrstc micro engine
// сopyright (C) 2019-2020 dmitrii torkhov <dmitriitorkhov@gmail.com>
//

#pragma once

#include <memory>

#include "core/singleton.h"
#include "math/float2.h"

namespace cc {

    class renderer;

    class engine : public singleton<engine> {

        friend class singleton<engine>;

    public:

        void set_screen_size(const cc::float2 &size);

        void run();

        bool is_running() const;

        void tick();

        void resume();

        void pause();

    private:

        bool m_running = false;
        bool m_paused = true;
        bool m_is_screen_size_changed = false;

        cc::renderer *m_renderer;

        cc::float2 m_screen_size = {};

    private:

        engine();

        ~engine();

    };

}
