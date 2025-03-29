#pragma once

#ifndef ARX_MOVE_H
#define ARX_MOVE_H

namespace arx {
    namespace stdx {

        template<class T>
        inline T &&move(T &t) { return static_cast<T &&>(t); }
    }
}

#endif //ARX_MOVE_H
