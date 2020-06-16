//
// clrstc micro engine
// сopyright (C) 2019-2020 dmitrii torkhov <dmitriitorkhov@gmail.com>
//

#pragma once

template<class T>
class singleton {
public:

    static T *i() {
        if (!m_instance) {
            m_instance = new T();
        }

        return m_instance;
    }

    static void destroy_instance() {
        delete m_instance;
        m_instance = nullptr;
    }

protected:

    static T *m_instance;

};

template<class T> T *singleton<T>::m_instance = nullptr;