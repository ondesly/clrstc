//
// clrstc micro engine
// сopyright (C) 2019-2020 dmitrii torkhov <dmitriitorkhov@gmail.com>
//

#pragma once

#include <cstddef>

namespace cc {

    struct color3 {

        // -- Data --

        union {

            struct {

                unsigned char r, g, b;

            };

        };

        // -- Implicit basic constructors --

        color3() = default;

        color3(const color3 &) = default;

        // -- Explicit basic constructors --

        explicit color3(unsigned char c);

        color3(unsigned char r, unsigned char g, unsigned char b);

        // -- Component accesses --

        unsigned char &operator[](size_t i);

        const unsigned char &operator[](size_t i) const;

    };

}

#include "color3.inl"