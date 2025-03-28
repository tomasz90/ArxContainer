#pragma once

#ifndef ARX_CONTAINER_PAIR_H
#define ARX_CONTAINER_PAIR_H

namespace arx {
    namespace stdx {

        template<class T1, class T2>
        struct pair {
            T1 first;
            T2 second;
        };

        template<class T1, class T2>
        inline pair<T1, T2> make_pair(const T1 &t1, const T2 &t2) {
            return {t1, t2};
        };

    } // namespace arx
} // namespace stdx

template<typename T1, typename T2>
inline bool operator==(const arx::stdx::pair<T1, T2> &x, const arx::stdx::pair<T1, T2> &y) {
    return (x.first == y.first) && (x.second == y.second);
}

template<typename T1, typename T2>
inline bool operator!=(const arx::stdx::pair<T1, T2> &x, const arx::stdx::pair<T1, T2> &y) {
    return !(x == y);
}

#endif //ARX_CONTAINER_PAIR_H
