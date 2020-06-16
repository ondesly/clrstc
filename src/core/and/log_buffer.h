//
// clrstc micro engine
// сopyright (C) 2019-2020 dmitrii torkhov <dmitriitorkhov@gmail.com>
//

#pragma once

#include <iosfwd>

namespace cc {

    class log_buffer : public std::streambuf {
    public:

        log_buffer();

    private:

        static const char *TAG;

        static const size_t c_buffer_size = 128;

    private:

        char buffer[c_buffer_size] = {};

    private:

        int overflow(int c);

        int sync();

    };

}