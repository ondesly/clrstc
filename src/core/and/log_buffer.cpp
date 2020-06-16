//
// clrstc micro engine
// сopyright (C) 2019-2020 dmitrii torkhov <dmitriitorkhov@gmail.com>
//

#include <streambuf>

#include <android/log.h>

#include "log_buffer.h"

const char *cc::log_buffer::TAG = "CLRSTC";

cc::log_buffer::log_buffer() {
    setp(buffer, buffer + c_buffer_size - 1);
}

int cc::log_buffer::overflow(int c) {
    if (c == traits_type::eof()) {
        *pptr() = traits_type::to_char_type(c);
        sbumpc();
    }
    return this->sync() ? traits_type::eof() : traits_type::not_eof(c);
}

int cc::log_buffer::sync() {
    __android_log_print(ANDROID_LOG_INFO, TAG, "");

    int rc = 0;
    if (pbase() != pptr()) {
        __android_log_print(ANDROID_LOG_INFO, TAG, "%s",
                            std::string(pbase(), pptr() - pbase()).c_str());
        rc = 0;
        setp(buffer, buffer + c_buffer_size - 1);
    }
    return rc;
}
