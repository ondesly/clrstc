#
# clrstc micro engine
# сopyright (C) 2019-2020 dmitrii torkhov <dmitriitorkhov@gmail.com>
#

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake/)

find_package(Android)

#

set(CLRSTC_SRC_ROOT ${CMAKE_CURRENT_LIST_DIR}/../src)

#

include_directories(
        ${ANDROID_INCLUDE_DIRS}

        ${CLRSTC_SRC_ROOT})

#

set(CLRSTC_SRC
        ${CLRSTC_SRC_ROOT}/core/and/activity.cpp
        ${CLRSTC_SRC_ROOT}/core/and/log_buffer.cpp

        ${CLRSTC_SRC_ROOT}/core/engine.cpp

        ${CLRSTC_SRC_ROOT}/renderer/renderer.cpp)

#

add_library(clrstc STATIC ${CLRSTC_SRC})

target_link_libraries(clrstc
        ${ANDROID_LIBRARIES})