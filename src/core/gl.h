//
// clrstc micro engine
// сopyright (C) 2019-2020 dmitrii torkhov <dmitriitorkhov@gmail.com>
//

#pragma once

#if defined(__APPLE__)
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#endif

#if defined(ANDROID)
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif