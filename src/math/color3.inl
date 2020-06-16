//
// clrstc micro engine
// Copyright (C) 2019-2020 Dmitrii Torkhov <dmitriitorkhov@gmail.com>
//

namespace cc {

    // -- Explicit basic constructors --

    inline color3::color3(unsigned char c) : r(c), g(c), b(c) {

    }

    inline color3::color3(unsigned char r, unsigned char g, unsigned char b)
            : r(r), g(g), b(b) {

    }

    // -- Component accesses --

    inline unsigned char &color3::operator[](size_t i) {
        switch (i) {
            default:
            case 0:
                return r;
            case 1:
                return g;
            case 2:
                return b;
        }
    }

    inline const unsigned char &color3::operator[](size_t i) const {
        switch (i) {
            default:
            case 0:
                return r;
            case 1:
                return g;
            case 2:
                return b;
        }
    }

}